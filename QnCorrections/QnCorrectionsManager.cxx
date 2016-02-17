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

#include "QnCorrectionsManager.h"
#include "QnCorrectionsLog.h"

/// \cond CLASSIMP
ClassImp(QnCorrectionsManager);
/// \endcond

/// the highest detector id currently supported by the framework
const Int_t QnCorrectionsManager::nMaxNoOfDetectors = 32;
/// the maximum number of variables currently supported by the framework
const Int_t QnCorrectionsManager::nMaxNoOfDataVariables = 2048;

/// Default constructor.
/// The class owns the detectors and will be destroyed with it
QnCorrectionsManager::QnCorrectionsManager() : TObject(), fDetectorsSet() {

  fDetectorsSet.SetOwner(kTRUE);
  fDetectorsIdMap = new QnCorrectionsDetector *[nMaxNoOfDetectors];
  fDataContainer = new Float_t[nMaxNoOfDataVariables];
}

/// Default destructor
/// Deletes the memory taken
QnCorrectionsManager::~QnCorrectionsManager() {

  delete fDetectorsIdMap;
  delete fDataContainer;
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

}

/// Set the list of histograms associated to the current run of the framework
void QnCorrectionsManager::SetWorkingHistogramList(TList *) {

}

/// Produce the final output and release the framework
void QnCorrectionsManager::FinalizeQnCorrectionsFramework() {

}




