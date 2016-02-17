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

  void AddDetector(QnCorrectionsDetector *detector);

  QnCorrectionsDetector *FindDetector(const char *name) const;
  QnCorrectionsDetector *FindDetector(Int_t id) const;
  QnCorrectionsDetectorConfigurationBase *FindDetectorConfiguration(const char *name) const;

  /// Gets a pointer to the data variables bank
  /// \return the pointer to the data container
  Float_t *GetDataContainer() { return fDataContainer; }

  void InitializeQnCorrectionsFramework();
  void SetWorkingHistogramList(TList *);
  void AddDataVector(Int_t detectorId, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1);
  void ProcessEvent();
  void FinalizeQnCorrectionsFramework();

private:
  void ClearEvent();

  static const Int_t nMaxNoOfDetectors; ///< the highest detector id currently supported by the framework
  static const Int_t nMaxNoOfDataVariables; ///< the maximum number of variables currently supported by the framework
  TList fDetectorsSet;                  ///< the list of detectors
  QnCorrectionsDetector **fDetectorsIdMap; ///< map between external detector Id and internal detector
  Float_t *fDataContainer;              ///< the data variables bank
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
inline void QnCorrectionsManager::AddDataVector(Int_t detectorId, Double_t phi, Double_t weight, Int_t channelId) {
  fDetectorsIdMap[detectorId]->AddDataVector(fDataContainer, phi, weight, channelId);
}

/// Process the current event
///
/// The request is transmitted to the different detectors.
///
/// Must be called only when the whole data vectors for the event
/// have been incorporated to the framework.
inline void QnCorrectionsManager::ProcessEvent() {
  for (Int_t ixDetector = 0; ixDetector < fDetectorsSet.GetEntries(); ixDetector++) {
    ((QnCorrectionsDetector *) fDetectorsSet.At(ixDetector))->ProcessCorrections();
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
