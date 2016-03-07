#ifndef QNCORRECTIONS_MANAGER_H
#define QNCORRECTIONS_MANAGER_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsManager.h
/// \brief Class that orchestrates the Q vector correction framework

/// \class QnCorrectionsManager
/// \brief Class orchestrates the Q vector correction framework
///
/// It should be only one instance of it and behaves as the anchor
/// point between the Q vector correction framework and the external
/// run time environment.
///
/// It owns the list of the detectors incorporated to the framework
/// and distributes among them the different commands according to
/// the analysis phase.
///
/// To improve performance a mapping between internal detector address
/// and external detector id is maintained.
///
/// When the framework is in the calibration phase there are no complete
/// calibration information available to fully implement the desired
/// correction on the input data and on the subsequent Q vector. During
/// this phase a list of support histograms is kept to build the
/// needed quantities to produce the intended calibration information.
/// At the same time a list of the available calibration histograms is
/// as well kept to perform the feasible corrections.
///
/// When, finally, the whole calibration information were available the
/// support histograms list will not need to be present and the framework
/// will be performing just the intended corrections on input data and on
/// the subsequent Q vector.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 16, 2016


#include <TObject.h>
#include "QnCorrectionsDetector.h"

class QnCorrectionsManager : public TObject {
public:
  QnCorrectionsManager();
  virtual ~QnCorrectionsManager();

  /// Set the name of the list that should be considered as assigned to the current process
  /// \param name the name of the list
  void SetCurrentProcessListName(const char *name)
  { fProcessListName = name; }

  void SetCalibrationHistogramsList(TFile *calibrationFile);

  void AddDetector(QnCorrectionsDetector *detector);

  QnCorrectionsDetector *FindDetector(const char *name) const;
  QnCorrectionsDetector *FindDetector(Int_t id) const;
  QnCorrectionsDetectorConfigurationBase *FindDetectorConfiguration(const char *name) const;


  /// Gets a pointer to the data variables bank
  /// \return the pointer to the data container
  Float_t *GetDataContainer() { return fDataContainer; }

  /// Gets the output histograms list
  TList *GetOutputHistogramsList() const { return fSupportHistogramsList; }

  void InitializeQnCorrectionsFramework();
  Int_t AddDataVector(Int_t detectorId, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1);
  const char *GetAcceptedDataDetectorConfigurationName(Int_t detectorId, Int_t index) const;
  void ProcessEvent();
  void FinalizeQnCorrectionsFramework();

private:
  void ClearEvent();

  static const Int_t nMaxNoOfDetectors; ///< the highest detector id currently supported by the framework
  static const Int_t nMaxNoOfDataVariables; ///< the maximum number of variables currently supported by the framework
  static const char *szCalibrationHistogramsKeyName; ///< the name of the key under which calibration histograms lists are stored
  TList fDetectorsSet;                  ///< the list of detectors
  ///< map between external detector Id and internal detector
  QnCorrectionsDetector **fDetectorsIdMap; //[nMaxNoOfDetectors]
  ///< the data variables bank
  Float_t *fDataContainer;                 //[nMaxNoOfDataVariables]
  TList *fCalibrationHistogramsList;    //!<! the list of the calibration histograms
  TList *fSupportHistogramsList;        //!<! the list of the support histograms
  TString fProcessListName;             ///< the name of the list associated to the current process
/// \cond CLASSIMP
  ClassDef(QnCorrectionsManager, 1);
/// \endcond
};

/// New data vector for the framework
/// The request is transmitted to the passed detector together with
/// the current content of the variable bank.
/// \param detectorId id of the involved detector
/// \param phi azimuthal angle
/// \param weight the weight of the data vector
/// \param channelId the channel Id that originates the data vector
/// \return the number of detector configurations that accepted and stored the data vector
inline Int_t QnCorrectionsManager::AddDataVector(Int_t detectorId, Double_t phi, Double_t weight, Int_t channelId) {
  return fDetectorsIdMap[detectorId]->AddDataVector(fDataContainer, phi, weight, channelId);
}

/// Gets the name of the detector configuration at index that accepted last data vector
/// \param detectorId id of the involved detector
/// \param index the position in the list of accepted data vector configuration
/// \return the configuration name
inline const char *QnCorrectionsManager::GetAcceptedDataDetectorConfigurationName(Int_t detectorId, Int_t index) const {
  return fDetectorsIdMap[detectorId]->GetAcceptedDataDetectorConfigurationName(index);
}

/// Process the current event
///
/// The request is transmitted to the different detectors.
///
/// Must be called only when the whole data vectors for the event
/// have been incorporated to the framework.
inline void QnCorrectionsManager::ProcessEvent() {
  for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
    ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->ProcessCorrections(fDataContainer);
  }
  ClearEvent();
}

/// Clear the current event
///
/// The request is transmitted to the different detectors.
///
/// Must be called only at the end of each event to start processing the next one
inline void QnCorrectionsManager::ClearEvent() {
  for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
    ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->ClearDetector();
  }
}

#endif // QNCORRECTIONS_MANAGER_H
