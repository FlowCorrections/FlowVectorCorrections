#ifndef QNCORRECTIONS_DETECTOR_H
#define QNCORRECTIONS_DETECTOR_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsDetector.h
/// \brief Detector and detector configuration classes for Q vector correction framework
///

#include <TObject.h>
#include <TList.h>
#include "QnCorrectionsCuts.h"
#include "QnCorrectionsCorrectionSteps.h"

/// \class QnCorrectionsDetector
/// \brief Detector class within Q vector correction framework
///
/// Basically the role of the detector class is, apart of storing
/// the detector name and Id, to store and handle the list of the different
/// configurations defined for the involved detector.
///
/// The detector Id should only be obtained at creation time and
/// the object, once created, does not allow its modification.
///
/// The detector object is in the path of the whole control flow and
/// as such it should distribute the different commands to the
/// defined detector configurations.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 08, 2016

class QnCorrectionsDetectorConfigurationSet;

class QnCorrectionsDetector : public TNamed {
public:
  QnCorrectionsDetector();
  QnCorrectionsDetector(const char *name, Int_t id);
  virtual ~QnCorrectionsDetector();

  /// Gets the detector Id
  ///
  /// \return detector Id
  Int_t GetId() { return fDetectorId; }

  /// Asks for attaching the needed input information to the correction steps
  ///
  /// The request is transmitted to the attached detector configurations
  /// \param list list where the input information should be found
  /// \return kTRUE if everything went OK
  Bool_t AttachCorrectionInputs(TList *list);
  /// Ask for processing corrections for the involved detector
  ///
  /// The request is transmitted to the attached detector configurations
  /// \return kTRUE if everything went OK
  Bool_t ProcessCorrections();

  /// New data vector for the detector
  /// The request is transmitted to the attached detector configurations.
  /// The current content of the variable bank is passed in order to check
  /// for optional cuts tha define the detector configurations.
  /// \param variableContainer pointer to the variable content bank
  /// \param phi azimuthal angle
  /// \param weight the weight of the data vector
  /// \param channelId the channel Id that originates the data vector
  void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 0.1, Int_t channelId = -1);

private:
  Int_t fDetectorId;            ///< detector Id
  QnCorrectionsDetectorConfigurationSet fConfigurations;  ///< the set of configurations defined for this detector
};

/// \class QnCorrectionsDetectorConfigurationBase
/// \brief The base of a concrete detector configuration within Q vector correction framework
///
/// The detector configuration shapes a detector with a concrete
/// set of cuts to make it the target of a Q vector correction process.
///
/// It receives the data input stream and build the corresponding Q vector associated
/// to it for each processing request.
///
/// As such, it incorporates the set of corrections to carry on the input data
/// and the set of corrections to perform on the produced Q vector.
///
/// The reference of the detector should only be obtained at creation time and
/// the object, once created, does not allow its modification.
///
/// The class is s base class for further refined detector configurations.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 08, 2016

class QnCorrectionsDetectorConfigurationBase : public TNamed {
public:
  QnCorrectionsDetectorConfigurationBase();
  QnCorrectionsDetectorConfigurationBase(const char *name,
      QnCorrectionsDetector *detector,
      QnCorrectionsCutsSet *cuts = NULL);
  virtual ~QnCorrectionsDetectorConfigurationBase();

  /// Gets the detector reference
  ///
  /// \return detector pointer
  QnCorrectionsDetector *GetDetector() { return fDetector; }

  /// Asks for attaching the needed input information to the correction steps
  ///
  /// The request is transmitted to the incoming data correction steps
  /// and to the Q vector correction steps.
  /// \param list list where the input information should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachCorrectionInputs(TList *list);
  /// Ask for processing corrections for the involved detector configuration
  ///
  /// The request is transmitted to the incoming data correction steps
  /// and to the Q vector correction steps.
  /// \return kTRUE if everything went OK
  virtual Bool_t ProcessCorrections();

  /// New data vector for the detector configuration
  /// Pure virtual function
  /// \param variableContainer pointer to the variable content bank
  /// \param phi azimuthal angle
  /// \param weight the weight of the data vector
  /// \param channelId the channel Id that originates the data vector
  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 0.1, Int_t channelId = -1) = 0;

  /// Checks if the current content of the variable bank applies to
  /// the detector configuration
  /// \param variableContainer pointer to the variable content bank
  /// \return kTRUE if the current content applies to the configuration
  virtual Bool_t IsSelected(const Float_t *variableContainer)
    { return fCuts->IsSelected(variableContainer); }

private:
  QnCorrectionsDetector *fDetector;    ///< pointer to the detector that owns the configuration
  QnCorrectionsCutsSet *fCuts;         ///< set of cuts that define the detector configuration
protected:
  TClonesArray *dataVectorBank;        ///< input data for the current process / event
  QnCorrectionsQnVector fQnVector;     ///< Q vector from the post processed input data
  QnCorrectionsSetOfCorrectionsOnQvector *fQnVectorCorrections; ///< set of corrections to apply on Q vectors
};

/// \class QnCorrectionsTrackDetectorConfiguration
/// \brief Track detector configuration within Q vector correction framework
///
/// A track detector within the Q vector correction framework is defined
/// as one for which its data vectors only involve azimuthal angles. All
/// its contribution to build a Q vector have the same weight.
///
/// According to that, the proper data vector is used and no extra need
/// for a Q vector builder appears.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 08, 2016

class QnCorrectionsTrackDetectorConfiguration :
    public QnCorrectionsDetectorConfigurationBase {
public:
  QnCorrectionsTrackDetectorConfiguration();
  QnCorrectionsTrackDetectorConfiguration(const char *name,
      QnCorrectionsDetector *detector,
      QnCorrectionsCutsSet *cuts = NULL);
  virtual ~QnCorrectionsTrackDetectorConfiguration();

  /// New data vector for the detector configuration.
  /// A check is made to see if the current variable bank content passes
  /// the associated cuts. If so, the data vector is stored.
  /// \param variableContainer pointer to the variable content bank
  /// \param phi azimuthal angle
  /// \param weight the weight of the data vector. Ignored for track detector configurations.
  /// \param channelId the channel Id that originates the data vector. Ignored for track detector configurations.
  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 0.1, Int_t channelId = -1);
};

inline void QnCorrectionsTrackDetectorConfiguration::AddDataVector(
    const Float_t *variableContainer, Double_t phi, Double_t, Int_t) {
  if (IsSelected(variableContainer)) {
    /// add the data vector to the bank
    QnCorrectionsDataVector *dataVector =
        new (dataVectorBank->ConstructedAt(dataVectorBank->GetEntriesFast()))
            QnCorrectionsDataVector(phi);
  }
}

/// \class QnCorrectionsChannelDetectorConfiguration
/// \brief Channel detector configuration within Q vector correction framework
///
/// A channel detector within the Q vector correction framework is defined
/// as one for which its data vectors only involve azimuthal angles and channels
/// susceptible of weighting and / or grouping and / or calibration, etc.
///
/// According to that, the proper channelized data vector is used and an extra
/// Q vector builder is incorporated.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 08, 2016

class QnCorrectionsChannelDetectorConfiguration :
    public QnCorrectionsDetectorConfigurationBase {
public:
  QnCorrectionsChannelDetectorConfiguration();
  QnCorrectionsChannelDetectorConfiguration(const char *name,
      QnCorrectionsDetector *detector,
      QnCorrectionsCutsSet *cuts = NULL);
  virtual ~QnCorrectionsChannelDetectorConfiguration();

  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 0.1, Int_t channelId = -1);

private:
  Bool_t *fUsedChannel;  ///< array, which of the detector channels is used for this configuration
  Int_t *fChannelGroup; ///< array, the group to which the channel pertains
};

/// New data vector for the detector configuration.
/// A check is made to match the channel Id with the ones assigned
/// to the detector configuration and then an additional one to
/// see if the current variable bank content passes
/// the associated cuts. If so, the data vector is stored.
/// \param variableContainer pointer to the variable content bank
/// \param phi azimuthal angle
/// \param weight the weight of the data vector. Ignored for track detector configurations.
/// \param channelId the channel Id that originates the data vector. Ignored for track detector configurations.
inline void QnCorrectionsChannelDetectorConfiguration::AddDataVector(
    const Float_t *variableContainer, Double_t phi, Double_t weight, Int_t channelId) {
  if (fUsedChannel[channelId]) {
    if (IsSelected(variableContainer)) {
      /// add the data vector to the bank
      QnCorrectionsChannelizedDataVector *channelizedDataVector =
          new (dataVectorBank->ConstructedAt(dataVectorBank->GetEntriesFast()))
            QnCorrectionsChannelizedDataVector(channelId, phi, weight);
    }
  }
}


#endif // QNCORRECTIONS_DETECTOR_H
