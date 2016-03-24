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
///< the name of the key under which QA calibration histograms lists are stored
const char *QnCorrectionsManager::szCalibrationQAHistogramsKeyName = "CalibrationQAHistograms";
///< accepted temporary name before getting the definitive one
const char *QnCorrectionsManager::szDummyProcessListName = "dummyprocess";
/// Default constructor.
/// The class owns the detectors and will be destroyed with it
QnCorrectionsManager::QnCorrectionsManager() :
    TObject(), fDetectorsSet(), fProcessListName(szDummyProcessListName) {

  fDetectorsSet.SetOwner(kTRUE);
  fDetectorsIdMap = NULL;
  fDataContainer = NULL;
  fCalibrationHistogramsList = NULL;
  fSupportHistogramsList = NULL;
  fQAHistogramsList = NULL;
  fQnVectorTree = NULL;
  fQnVectorList = NULL;
  fFillOutputHistograms = kFALSE;
  fFillQAHistograms = kFALSE;
  fFillQnVectorTree = kFALSE;
  fProcessesNames = NULL;
}

/// Default destructor
/// Deletes the memory taken
QnCorrectionsManager::~QnCorrectionsManager() {

  if (fDetectorsIdMap != NULL) delete fDetectorsIdMap;
  if (fDataContainer != NULL) delete fDataContainer;
  if (fCalibrationHistogramsList != NULL) delete fCalibrationHistogramsList;
  if (fProcessesNames != NULL) delete fProcessesNames;
}

/// Sets the base list that will own the input calibration histograms
/// \param calibrationFile the file
void QnCorrectionsManager::SetCalibrationHistogramsList(TFile *calibrationFile) {
  if (calibrationFile) {
    if (calibrationFile->GetListOfKeys()->GetEntries() > 0) {
      fCalibrationHistogramsList = (TList*)((TKey*)calibrationFile->GetListOfKeys()->FindObject(szCalibrationHistogramsKeyName))->ReadObj()->Clone();
      if (fCalibrationHistogramsList != NULL)
        /* we need the histograms ownership once we go to the GRID */
        fCalibrationHistogramsList->SetOwner(kTRUE);
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

  /* let's build the detectors map */
  fDetectorsIdMap = new QnCorrectionsDetector *[nMaxNoOfDetectors];
  QnCorrectionsDetector *detector = NULL;
  for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
    detector = (QnCorrectionsDetector *) fDetectorsSet.At(ixDetector);
    fDetectorsIdMap[detector->GetId()] = detector;
  }


  /* build the support histograms list */
  fSupportHistogramsList = new TList();
  fSupportHistogramsList->SetName(szCalibrationHistogramsKeyName);
  fSupportHistogramsList->SetOwner(kTRUE);

  /* build the support for the list of concurrent processes */
  if (fProcessesNames != NULL && fProcessesNames->GetEntries() != 0) {
    for (Int_t i = 0; i < fProcessesNames->GetEntries(); i++) {
      TList *newList = new TList();
      newList->SetName(((TObjString *) fProcessesNames->At(i))->GetName());
      newList->SetOwner(kTRUE);
      fSupportHistogramsList->Add(newList);

      /* build the support histograms list associated to the process */
      /* leave the selected process list name for a the latter time */
      if (!fProcessListName.EqualTo(fProcessesNames->At(i)->GetName())) {
        for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
          ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->CreateSupportHistograms(newList);
        }
      }
    }
  }

  /* build the support histograms list associated to this process */
  /* and pass it to the detectors for support histograms creation */
  if (fProcessListName.Length() != 0) {
    /* let's see first whether we have the current process name within the processes names list */
    TList *processList;
    if (fProcessesNames != NULL && fProcessesNames->GetEntries() != 0 && fSupportHistogramsList->FindObject(fProcessListName) != NULL) {
      processList = (TList *) fSupportHistogramsList->FindObject(fProcessListName);
    }
    else {
      processList = new TList();
      processList->SetName((const char *) fProcessListName);
      processList->SetOwner(kTRUE);
      /* we add it but probably temporarily */
      fSupportHistogramsList->Add(processList);
    }
    /* now transfer the order to the defined detectors */
    /* so, we always create the histograms to use the latest ones */
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
  /* are in calibrating phase. We should TODO this. */
  if (fCalibrationHistogramsList != NULL) {
    TList *processList = (TList *)fCalibrationHistogramsList->FindObject((const char *)fProcessListName);
    if (processList != NULL) {
      /* now transfer the order to the defined detectors */
      for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
        ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->AttachCorrectionInputs(processList);
      }
    }
  }

  /* build the QA histograms list if needed */
  if (GetShouldFillQAHistograms()) {
    fQAHistogramsList = new TList();
    fQAHistogramsList->SetName(szCalibrationQAHistogramsKeyName);
    fQAHistogramsList->SetOwner(kTRUE);

    /* and pass it to the detectors for QA histograms creation */
    for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
      ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->CreateQAHistograms(fQAHistogramsList);
    }
  }
}

/// Set the name of the list that should be considered as assigned to the current process
/// If the stored process list name is the default one and the support histograms are
/// already created, change the list name and store the new name and get the new process
/// list on the calibration histograms list if any and pass it to the detectors for input
/// calibration histograms attachment.
/// Changing process list name on the fly during a  running process is not supported.
/// If the list of concurrent processes names is not empty, the new process name should be
/// in the list. If not a run time error is raised.
/// \param name the name of the list
void QnCorrectionsManager::SetCurrentProcessListName(const char *name) {
  if (fProcessListName.EqualTo(szDummyProcessListName)) {
    if (fSupportHistogramsList != NULL) {
      /* check the list of concurrent processes */
      if (fProcessesNames != NULL && fProcessesNames->GetEntries() != 0) {
        /* the new process name should be in the list of processes names */
        if (fSupportHistogramsList->FindObject(name) != NULL) {
          /* now we have to substitute the provisional process name list with the temporal one but renamed */
          TList *previousempty = (TList*) fSupportHistogramsList->FindObject(name);
          Int_t finalindex = fSupportHistogramsList->IndexOf(previousempty);
          fSupportHistogramsList->RemoveAt(finalindex);
          delete previousempty;
          TList *previoustemp = (TList *) fSupportHistogramsList->FindObject((const char *)fProcessListName);
          fSupportHistogramsList->Remove(previoustemp);
          previoustemp->SetName(name);
          fSupportHistogramsList->AddAt(previoustemp, finalindex);
        }
        else {
          /* nop! we raise an execution error */
          QnCorrectionsFatal(Form("The name of the process you want to run: %s, is not in the list of concurrent processes", name));
        }
      }
      else {
        TList *processList = (TList *) fSupportHistogramsList->FindObject((const char *)fProcessListName);
        processList->SetName(name);
      }

      fProcessListName = name;
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
  }
  else {
    QnCorrectionsFatal(Form("Changing process list name on the fly is not supported." \
        " Current name: %s, new name: %s", (const char *)fProcessListName, name));
  }
}

/// Produce the final output and release the framework
void QnCorrectionsManager::FinalizeQnCorrectionsFramework() {

}




