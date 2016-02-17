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
#include <TObjArray.h>
#include <TClonesArray.h>
#include "QnCorrectionsCuts.h"
#include "QnCorrectionsCorrectionSteps.h"
#include "QnCorrectionsEventClasses.h"
#include "QnCorrectionsQnVector.h"
#include "QnCorrectionsDataVector.h"

class QnCorrectionsDetectorConfigurationSet;
class QnCorrectionsDetectorConfigurationBase;
class QnCorrectionsDetector;

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
/// It receives at construction time the set of event classes variables and the
/// detector reference. The reference of the detector should only be obtained at
/// creation time and the detector configuration object, once created, does not
/// allow its modification.
///
/// The class is a base class for further refined detector configurations.
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
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap = NULL);
  virtual ~QnCorrectionsDetectorConfigurationBase();

  /// Sets the set of cuts for the detector configuration
  /// \param cuts the set of cuts
  void SetCuts(QnCorrectionsCutsSet *cuts) { fCuts = cuts; }

  /// Gets the detector reference
  ///
  /// \return detector pointer
  QnCorrectionsDetector *GetDetector() { return fDetector; }

  virtual Bool_t CreateSupportHistograms(TList *list);

  virtual Bool_t AttachCorrectionInputs(TList *list);
  virtual Bool_t ProcessCorrections();

  virtual void AddCorrectionOnQnVector(QnCorrectionsCorrectionOnQvector *correctionOnQn);
  virtual void AddCorrectionOnInputData(QnCorrectionsCorrectionOnInputData *correctionOnInputData);

  /// New data vector for the detector configuration
  /// Pure virtual function
  /// \param variableContainer pointer to the variable content bank
  /// \param phi azimuthal angle
  /// \param weight the weight of the data vector
  /// \param channelId the channel Id that originates the data vector
  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1) = 0;

  virtual Bool_t IsSelected(const Float_t *variableContainer);
  virtual Bool_t IsSelected(const Float_t *variableContainer, Int_t nChannel);

  /// Clean the configuration to accept a new event
  /// Pure virtual function
  virtual void ClearConfiguration() = 0;

private:
  QnCorrectionsDetector *fDetector;    ///< pointer to the detector that owns the configuration
protected:
  QnCorrectionsCutsSet *fCuts;         ///< set of cuts that define the detector configuration
  TClonesArray *fDataVectorBank;        ///< input data for the current process / event
  QnCorrectionsQnVector fQnVector;     ///< Q vector from the post processed input data
  QnCorrectionsSetOfCorrectionsOnQvector fQnVectorCorrections; ///< set of corrections to apply on Q vectors
  QnCorrectionsEventClassVariablesSet    *fEventClassVariables; ///< set of variables that define event classes

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDetectorConfigurationBase, 1);
/// \endcond
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
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap = NULL);
  virtual ~QnCorrectionsTrackDetectorConfiguration();

  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1);

  /// Checks if the current content of the variable bank applies to
  /// the detector configuration
  /// \param variableContainer pointer to the variable content bank
  /// \return kTRUE if the current content applies to the configuration
  virtual Bool_t IsSelected(const Float_t *variableContainer)
    { return ((fCuts != NULL) ? fCuts->IsSelected(variableContainer) : kTRUE); }
  /// wrong call for this class invoke base class behavior
  virtual Bool_t IsSelected(const Float_t *variableContainer, Int_t nChannel)
  { return QnCorrectionsDetectorConfigurationBase::IsSelected(variableContainer,nChannel); }

  virtual void ClearConfiguration();

/// \cond CLASSIMP
  ClassDef(QnCorrectionsTrackDetectorConfiguration, 1);
/// \endcond
};

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
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfChannels,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap = NULL);
  virtual ~QnCorrectionsChannelDetectorConfiguration();

  void SetChannelsScheme(Bool_t *bUsedChannel, Int_t *nChannelGroup);

  virtual Bool_t CreateSupportHistograms(TList *list);

  virtual Bool_t AttachCorrectionInputs(TList *list);
  virtual Bool_t ProcessCorrections();

  virtual void AddCorrectionOnInputData(QnCorrectionsCorrectionOnInputData *correctionOnInputData);

  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1);

  /// Checks if the current content of the variable bank applies to
  /// the detector configuration for the passed channel.
  /// \param variableContainer pointer to the variable content bank
  /// \param nChannel the interested external channel number
  /// \return kTRUE if the current content applies to the configuration
  virtual Bool_t IsSelected(const Float_t *variableContainer, Int_t nChannel)
    { return ((fUsedChannel[nChannel]) ? ((fCuts != NULL) ? fCuts->IsSelected(variableContainer) : kTRUE) : kFALSE); }
  /// wrong call for this class invoke base class behavior
  virtual Bool_t IsSelected(const Float_t *variableContainer)
  { return QnCorrectionsDetectorConfigurationBase::IsSelected(variableContainer); }

  virtual void ClearConfiguration();

private:
  Bool_t *fUsedChannel;  ///< array, which of the detector channels is used for this configuration
  Int_t *fChannelGroup; ///< array, the group to which the channel pertains
  Int_t fNoOfChannels;  ///< The number of channels associated
  QnCorrectionsSetOfCorrectionsOnInputData fInputDataCorrections; ///< set of corrections to apply on input data vectors

/// \cond CLASSIMP
  ClassDef(QnCorrectionsChannelDetectorConfiguration, 1);
/// \endcond
};

/// \class QnCorrectionsDetectorConfigurationSet
/// \brief Array of detector configurations within Q vector correction framework
///
/// Each detector within the Q vector correction framework is
/// able to be configured in different ways by assigning set of cuts,
/// ordering its data within channels or even channel groups, etc.
/// Each of these ways constitute a detector configuration. This class
/// stores the whole set of detector configurations assigned to a concrete
/// detector.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 09, 2016

class QnCorrectionsDetectorConfigurationSet : public TObjArray {
public:
  QnCorrectionsDetectorConfigurationSet();
  virtual ~QnCorrectionsDetectorConfigurationSet();

  /// Access the detector configuration at the passed position
  /// \param i position in the list (starting at zero)
  /// \return the detector configuration object a position i
  virtual QnCorrectionsDetectorConfigurationBase *At(Int_t i) const
    { return (QnCorrectionsDetectorConfigurationBase *) TObjArray::At(i);}

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDetectorConfigurationSet, 1);
/// \endcond
};

/// \class QnCorrectionsDetector
/// \brief Detector class within Q vector correction framework
///
/// The roles of the detector class are: to store its unique name and Id,
/// and to store and handle the list of the different
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

class QnCorrectionsDetector : public TNamed {
public:
  QnCorrectionsDetector();
  QnCorrectionsDetector(const char *name, Int_t id);
  virtual ~QnCorrectionsDetector();

  /// Gets the detector Id
  ///
  /// \return detector Id
  Int_t GetId() { return fDetectorId; }

  Bool_t CreateSupportHistograms(TList *list);
  Bool_t AttachCorrectionInputs(TList *list);
  Bool_t ProcessCorrections();

  void AddDetectorConfiguration(QnCorrectionsDetectorConfigurationBase *detectorConfiguration);
  QnCorrectionsDetectorConfigurationBase *FindDetectorConfiguration(const char *name);

  void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1);

  virtual void ClearDetector();

private:
  Int_t fDetectorId;            ///< detector Id
  QnCorrectionsDetectorConfigurationSet fConfigurations;  ///< the set of configurations defined for this detector

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDetector, 1);
/// \endcond
};

/// Ask for processing corrections for the involved detector configuration
///
/// As for a base class the request is transmitted to the Q vector correction steps.
/// \return kTRUE if everything went OK
inline Bool_t QnCorrectionsDetectorConfigurationBase::ProcessCorrections() {
  Bool_t retValue = kFALSE;
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue || (fQnVectorCorrections.At(ixCorrection)->Process());
  }
  return retValue;
}

/// New data vector for the detector configuration.
/// A check is made to see if the current variable bank content passes
/// the associated cuts. If so, the data vector is stored.
/// \param variableContainer pointer to the variable content bank
/// \param phi azimuthal angle
/// \param weight the weight of the data vector. Ignored for track detector configurations.
/// \param channelId the channel Id that originates the data vector. Ignored for track detector configurations.
inline void QnCorrectionsTrackDetectorConfiguration::AddDataVector(
    const Float_t *variableContainer, Double_t phi, Double_t, Int_t) {
  if (IsSelected(variableContainer)) {
    /// add the data vector to the bank
    QnCorrectionsDataVector *dataVector =
        new (fDataVectorBank->ConstructedAt(fDataVectorBank->GetEntriesFast()))
            QnCorrectionsDataVector(phi);
  }
}

/// Clean the configuration to accept a new event
///
/// Transfers the order to the Q vector correction steps and
/// cleans the own Q vector and the input data vector bank
/// for accepting the next event.
inline void QnCorrectionsTrackDetectorConfiguration::ClearConfiguration() {
  /* transfer the order to the Q vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->ClearCorrectionStep();
  }
  /* clean the own Q vector */
  fQnVector.Reset();
  /* and now clear the the input data bank */
  fDataVectorBank->Clear("C");
}


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
    if (IsSelected(variableContainer, channelId)) {
      /// add the data vector to the bank
      QnCorrectionsChannelizedDataVector *channelizedDataVector =
          new (fDataVectorBank->ConstructedAt(fDataVectorBank->GetEntriesFast()))
            QnCorrectionsChannelizedDataVector(channelId, phi, weight);
    }
  }
}

/// Ask for processing corrections for the involved detector configuration
///
/// The request is transmitted to the incoming data correction steps
/// and to then to Q vector correction steps via base class
/// \return kTRUE if everything went OK
inline Bool_t QnCorrectionsChannelDetectorConfiguration::ProcessCorrections() {
  Bool_t retValue = kFALSE;
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue || (fInputDataCorrections.At(ixCorrection)->Process());
  }

  /* if everything right propagate it to Q vector corrections via base class */
  if (retValue) {
    return QnCorrectionsDetectorConfigurationBase::ProcessCorrections();
  }
  return retValue;
}

/// Clean the configuration to accept a new event
///
/// Transfers the order to the Q vector correction steps then
/// to the input data correction steps and finally
/// cleans the own Q vector and the input data vector bank
/// for accepting the next event.
inline void QnCorrectionsChannelDetectorConfiguration::ClearConfiguration() {
  /* transfer the order to the Q vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->ClearCorrectionStep();
  }
  /* transfer the order to the data vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    fInputDataCorrections.At(ixCorrection)->ClearCorrectionStep();
  }
  /* clean the own Q vector */
  fQnVector.Reset();
  /* and now clear the the input data bank */
  fDataVectorBank->Clear("C");
}


/// New data vector for the detector
/// The request is transmitted to the attached detector configurations.
/// The current content of the variable bank is passed in order to check
/// for optional cuts tha define the detector configurations.
/// \param variableContainer pointer to the variable content bank
/// \param phi azimuthal angle
/// \param weight the weight of the data vector
/// \param channelId the channel Id that originates the data vector
inline void QnCorrectionsDetector::AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight, Int_t channelId) {
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->AddDataVector(variableContainer, phi, weight, channelId);
  }
}

/// Ask for processing corrections for the involved detector configuration
///
/// The request is transmitted to the attached detector configurations
/// \return kTRUE if everything went OK
inline Bool_t QnCorrectionsDetector::ProcessCorrections() {
  Bool_t retValue = kFALSE;
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    retValue = retValue || (fConfigurations.At(ixConfiguration)->ProcessCorrections());
  }
  return retValue;
}

/// Clean the detector to accept a new event
///
/// Transfers the order to the detector configurations
inline void QnCorrectionsDetector::ClearDetector() {
  /* transfer the order to the Q vector corrections */
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->ClearConfiguration();
  }
}

#endif // QNCORRECTIONS_DETECTOR_H
