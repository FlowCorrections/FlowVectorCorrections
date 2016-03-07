/**************************************************************************************************
 *                                                                                                *
 * Package:       FlowVectorCorrections                                                           *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch                              *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com                             *
 *                Víctor González, UCM, victor.gonzalez@cern.ch                                   *
 *                Contributors are mentioned in the code where appropriate.                       *
 * Development:   2012-2016                                                                       *
 *                                                                                                *
 * This file is part of FlowVectorCorrections, a software package that corrects Q-vector          *
 * measurements for effects of nonuniform detector acceptance. The corrections in this package    *
 * are based on publication:                                                                      *
 *                                                                                                *
 *  [1] "Effects of non-uniform acceptance in anisotropic flow measurements"                      *
 *  Ilya Selyuzhenkov and Sergei Voloshin                                                         *
 *  Phys. Rev. C 77, 034904 (2008)                                                                *
 *                                                                                                *
 * The procedure proposed in [1] is extended with the following steps:                            *
 * (*) alignment correction between subevents                                                     *
 * (*) possibility to extract the twist and rescaling corrections                                 *
 *      for the case of three detector subevents                                                  *
 *      (currently limited to the case of two “hit-only” and one “tracking” detectors)            *
 * (*) (optional) channel equalization                                                            *
 * (*) flow vector width equalization                                                             *
 *                                                                                                *
 * FlowVectorCorrections is distributed under the terms of the GNU General Public License (GPL)   *
 * (https://en.wikipedia.org/wiki/GNU_General_Public_License)                                     *
 * either version 3 of the License, or (at your option) any later version.                        *
 *                                                                                                *
 **************************************************************************************************/
/// \file QnCorrectionsManager.cxx
/// \brief Implementation of the class QnCorrectionsManager

#include <TFile.h>
#include <TList.h>
#include <TKey.h>
#include "QnCorrectionsManager.h"
#include "QnCorrectionsLog.h"

/// \cond CLASSIMP
ClassImp(QnCorrectionsManager);
/// \endcond

/// the highest detector id currently supported by the framework
const Int_t QnCorrectionsManager::nMaxNoOfDetectors = 32;
/// the maximum number of variables currently supported by the framework
const Int_t QnCorrectionsManager::nMaxNoOfDataVariables = 2048;
///< the name of the key under which calibration histograms lists are stored
const char *QnCorrectionsManager::szCalibrationHistogramsKeyName = "CalibrationHistograms";
/// Default constructor.
/// The class owns the detectors and will be destroyed with it
QnCorrectionsManager::QnCorrectionsManager() :
    TObject(), fDetectorsSet(), fProcessListName() {

  fDetectorsSet.SetOwner(kTRUE);
  fDetectorsIdMap = new QnCorrectionsDetector *[nMaxNoOfDetectors];
  fDataContainer = NULL;
  fCalibrationHistogramsList = NULL;
  fSupportHistogramsList = NULL;
  fQAHistogramsList = NULL;
  fQnVectorTree = NULL;
  fQnVectorList = NULL;
  fFillOutputHistograms = kFALSE;
  fFillQAHistograms = kFALSE;
  fFillQnVectorTree = kFALSE;
}

/// Default destructor
/// Deletes the memory taken
QnCorrectionsManager::~QnCorrectionsManager() {

  if (fDetectorsIdMap != NULL) delete fDetectorsIdMap;
  if (fDataContainer != NULL) delete fDataContainer;
}

/// Sets the base list that will own the input calibration histograms
/// \param calibrationFile the file
void QnCorrectionsManager::SetCalibrationHistogramsList(TFile *calibrationFile) {
  if (calibrationFile) {
    if (calibrationFile->GetListOfKeys()->GetEntries() > 0) {
      fCalibrationHistogramsList = (TList*)((TKey*)calibrationFile->GetListOfKeys()->FindObject(szCalibrationHistogramsKeyName))->ReadObj()->Clone();
    }
  }
}



/// Adds a new detector
/// Checks for an already added detector and for a detector id
/// out of range. If so, gives a runtime error to inform of misuse.
/// \param detector the new detector to incorporate to the framework
void QnCorrectionsManager::AddDetector(QnCorrectionsDetector *detector) {
  if (detector->GetId() < nMaxNoOfDetectors) {
    if (fDetectorsSet.FindObject(detector->GetName())) {
      QnCorrectionsFatal(Form("You are trying to add twice %s detector with detector Id %d. FIX IT, PLEASE.",
          detector->GetName(),
          detector->GetId()));
      return;
    }
    fDetectorsSet.Add(detector);
    fDetectorsIdMap[detector->GetId()] = detector;
  }
  else {
    QnCorrectionsFatal(Form("You are trying to add %s detector with detector Id %d " \
        "while the highest Id supported is %d. FIX IT, PLEASE.",
        detector->GetName(),
        detector->GetId(),
        nMaxNoOfDetectors - 1));
    return;
  }
}

/// Searches for a concrete detector by name
/// \param name the name of the detector to find
/// \return pointer to the found detector (NULL if not found)
QnCorrectionsDetector *QnCorrectionsManager::FindDetector(const char *name) const {
  return (QnCorrectionsDetector *) fDetectorsSet.FindObject(name);
}

/// Searches for a concrete detector by detector id
/// \param id the id of the detector to find
/// \return pointer to the found detector (NULL if not found)
QnCorrectionsDetector *QnCorrectionsManager::FindDetector(Int_t id) const {
  QnCorrectionsDetector *detector = NULL;
  for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
    detector = (QnCorrectionsDetector *) fDetectorsSet.At(ixDetector);
    if (detector->GetId() != id) {
      detector = NULL;
      continue;
    }
    else {
      break;
    }
  }
  return detector;
}


/// Searches for a concrete detector configuration by name
/// \param name the name of the detector configuration to find
/// \return pointer to the found detector configuration (NULL if not found)
QnCorrectionsDetectorConfigurationBase *QnCorrectionsManager::FindDetectorConfiguration(const char *name) const {
  QnCorrectionsDetectorConfigurationBase *detectorConfiguration;
  for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
    detectorConfiguration = ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->FindDetectorConfiguration(name);
    if (detectorConfiguration != NULL) {
      return detectorConfiguration;
    }
  }
  return NULL;
}

/// Initializes the correction framework
void QnCorrectionsManager::InitializeQnCorrectionsFramework() {

  /* the data bank */
  fDataContainer = new Float_t[nMaxNoOfDataVariables];

  /* build the support histograms list */
  fSupportHistogramsList = new TList();
  fSupportHistogramsList->SetName(szCalibrationHistogramsKeyName);
  fSupportHistogramsList->SetOwner(kTRUE);

  /* build the support histograms list associated to this process */
  /* and pass it to the detectors for support histograms creation */
  if (fProcessListName.Length() != 0) {
    TList *processList = new TList();
    processList->SetName((const char *) fProcessListName);
    processList->SetOwner(kTRUE);
    fSupportHistogramsList->Add(processList);
    /* now transfer the order to the defined detectors */
    Bool_t retvalue = kTRUE;
    for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
      retvalue = retvalue && ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->CreateSupportHistograms(processList);
      if (!retvalue)
        break;
    }
    if (!retvalue) {
      QnCorrectionsFatal("Failed to build the necessary support histograms.");
    }
  }
  else {
    QnCorrectionsFatal("The process label is missing.");
  }

  /* now get the process list on the calibration histograms list if any */
  /* and pass it to the detectors for input calibration histograms attachment, */
  /* we accept no calibration histograms list and no process list in case we */
  /* are in calibratin phase. We should TODO this. */
  if (fCalibrationHistogramsList != NULL) {
    TList *processList = (TList *)fCalibrationHistogramsList->FindObject((const char *)fProcessListName);
    if (processList != NULL) {
      /* now transfer the order to the defined detectors */
      for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
        ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->AttachCorrectionInputs(processList);
      }
    }
  }
}

/// Produce the final output and release the framework
void QnCorrectionsManager::FinalizeQnCorrectionsFramework() {

}




