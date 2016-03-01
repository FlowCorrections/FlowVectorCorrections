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
  friend class QnCorrectionsCorrectionStepBase;
  friend class QnCorrectionsDetector;
  QnCorrectionsDetectorConfigurationBase();
  QnCorrectionsDetectorConfigurationBase(const char *name,
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap = NULL);
  virtual ~QnCorrectionsDetectorConfigurationBase();

  /// Sets the set of cuts for the detector configuration
  /// \param cuts the set of cuts
  void SetCuts(QnCorrectionsCutsSet *cuts)
  { fCuts = cuts; }
  /// Sets the calibration method for Q vectors
  void SetQVectorCalibrationMethod(QnVectorCalibrationMethod method)
  { fQnCalibrationMethod = method; }

protected:
  /// Stores the detector reference
  /// \param detector the detector owner
  void SetDetectorOwner(QnCorrectionsDetector *detector)
  { fDetector = detector; }
  /// Gets the detector reference
  ///
  /// \return detector pointer
  QnCorrectionsDetector *GetDetector() { return fDetector; }
protected:
  /// Get the input data bank.
  /// Makes it available for input corrections steps.
  /// \return pointer to the input data bank
  TClonesArray *GetInputDataBank()
  { return fDataVectorBank; }
  /// Get the event class variables set
  /// Makes it available for corrections steps
  /// \return pointer to the event class variables set
  QnCorrectionsEventClassVariablesSet &GetEventClassVariablesSet()
  { return *fEventClassVariables; }
public:
  /// Asks for support histograms creation
  ///
  /// The request is transmitted to the different corrections.
  /// Pure virtual function
  /// \param list list where the histograms should be incorporated for its persistence
  /// \return kTRUE if everything went OK
  virtual Bool_t CreateSupportHistograms(TList *list) = 0;

  /// Asks for attaching the needed input information to the correction steps
  ///
  /// The request is transmitted to the different corrections.
  /// Pure virtual function
  /// \param list list where the input information should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachCorrectionInputs(TList *list) = 0;
  /// Ask for processing corrections for the involved detector configuration
  ///
  /// Pure virtual function.
  /// The request is transmitted to the correction steps
  /// and to then to Q vector correction steps
  /// \return kTRUE if everything went OK
  virtual Bool_t ProcessCorrections(const Float_t *variableContainer) = 0;

  virtual void AddCorrectionOnQnVector(QnCorrectionsCorrectionOnQvector *correctionOnQn);
  virtual void AddCorrectionOnInputData(QnCorrectionsCorrectionOnInputData *correctionOnInputData);

  /// Builds Qn vector before Q vector corrections but
  /// considering the chosen calibration method.
  /// Pure virtual function
  virtual void BuildQnVector() = 0;

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
  QnVectorCalibrationMethod fQnCalibrationMethod; ///< the method for Q vector calibration
  QnCorrectionsCorrectionsSetOnQvector fQnVectorCorrections; ///< set of corrections to apply on Q vectors
  QnCorrectionsEventClassVariablesSet    *fEventClassVariables; ///< set of variables that define event classes

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDetectorConfigurationBase, 1);
/// \endcond
};

/// \class QnCorrectionsDetectorConfigurationTracks
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

class QnCorrectionsDetectorConfigurationTracks :
    public QnCorrectionsDetectorConfigurationBase {
public:
  QnCorrectionsDetectorConfigurationTracks();
  QnCorrectionsDetectorConfigurationTracks(const char *name,
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap = NULL);
  virtual ~QnCorrectionsDetectorConfigurationTracks();

  virtual Bool_t CreateSupportHistograms(TList *list);
  virtual Bool_t AttachCorrectionInputs(TList *list);

  /// Ask for processing corrections for the involved detector configuration
  ///
  /// The request is transmitted to the Q vector correction steps
  /// \return kTRUE if everything went OK
  virtual Bool_t ProcessCorrections(const Float_t *variableContainer);
  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight = 1.0, Int_t channelId = -1);

  virtual void BuildQnVector();

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
  ClassDef(QnCorrectionsDetectorConfigurationTracks, 1);
/// \endcond
};

/// \class QnCorrectionsDetectorConfigurationChannels
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

class QnCorrectionsDetectorConfigurationChannels :
    public QnCorrectionsDetectorConfigurationBase {
public:
  QnCorrectionsDetectorConfigurationChannels();
  QnCorrectionsDetectorConfigurationChannels(const char *name,
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfChannels,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap = NULL);
  virtual ~QnCorrectionsDetectorConfigurationChannels();

  /// Gets the number of channels
  /// \return the number of channels of the associated detector
  Int_t GetNoOfChannels() { return fNoOfChannels; }
  /// Gets the used channels mask
  /// \return the used channels mask
  const Bool_t *GetUsedChannelsMask() const { return fUsedChannel; }
  /// Gets the channels groups
  /// \return the group associated to each channel
  const Int_t *GetChannelsGroups() const { return fChannelGroup; }

  void SetChannelsScheme(Bool_t *bUsedChannel, Int_t *nChannelGroup);

  virtual Bool_t CreateSupportHistograms(TList *list);

  virtual Bool_t AttachCorrectionInputs(TList *list);
  virtual Bool_t ProcessCorrections(const Float_t *variableContainer);

  virtual void AddCorrectionOnInputData(QnCorrectionsCorrectionOnInputData *correctionOnInputData);

  virtual void AddDataVector(const Float_t *variableContainer, Double_t phi, Double_t weight, Int_t channelId);

  virtual void BuildQnVector();
  void BuildRawQnVector();

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
  QnCorrectionsQnVector fRawQnVector;     ///< Q vector from input data before pre-processing
  Bool_t *fUsedChannel;  ///< array, which of the detector channels is used for this configuration
  Int_t *fChannelGroup; ///< array, the group to which the channel pertains
  Int_t fNoOfChannels;  ///< The number of channels associated
  QnCorrectionsCorrectionsSetOnInputData fInputDataCorrections; ///< set of corrections to apply on input data vectors

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDetectorConfigurationChannels, 1);
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
  Bool_t ProcessCorrections(const Float_t *variableContainer);

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

/// New data vector for the detector configuration.
/// A check is made to see if the current variable bank content passes
/// the associated cuts. If so, the data vector is stored.
/// \param variableContainer pointer to the variable content bank
/// \param phi azimuthal angle
/// \param weight the weight of the data vector. Ignored for track detector configurations.
/// \param channelId the channel Id that originates the data vector. Ignored for track detector configurations.
inline void QnCorrectionsDetectorConfigurationTracks::AddDataVector(
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
inline void QnCorrectionsDetectorConfigurationTracks::ClearConfiguration() {
  /* transfer the order to the Q vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->ClearCorrectionStep();
  }
  /* clean the own Q vector */
  fQnVector.Reset();
  /* and now clear the the input data bank */
  fDataVectorBank->Clear("C");
}

/// Builds Qn vector before Q vector corrections but
/// considering the chosen calibration method.
/// Remember, this configuration does not have a channelized
/// approach so, the built Q vector is the one to be used for
/// subsequent corrections.
inline void QnCorrectionsDetectorConfigurationTracks::BuildQnVector() {
  QnCorrectionsQnVectorBuild qVectorTemp(fQnVector);
  qVectorTemp.Reset();

  for(Int_t ixData = 0; ixData < fDataVectorBank->GetEntriesFast(); ixData++){
    QnCorrectionsDataVector *dataVector = static_cast<QnCorrectionsDataVector *>(fDataVectorBank->At(ixData));
    qVectorTemp.Add(dataVector->Phi(), dataVector->Weight());
  }
  qVectorTemp.Calibrate(fQnCalibrationMethod);
  fQnVector.Set(&qVectorTemp);
}


/// Ask for processing corrections for the involved detector configuration
///
/// The request is transmitted to the Q vector correction steps.
/// \return kTRUE if everything went OK
inline Bool_t QnCorrectionsDetectorConfigurationTracks::ProcessCorrections(const Float_t *variableContainer) {
  /* first we build the Q vector with the chosen calibration */
  BuildQnVector();

  /* then we transfer the request to the Q vector correction steps */
  Bool_t retValue = kTRUE;
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->Process(variableContainer));
  }
  return retValue;
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
inline void QnCorrectionsDetectorConfigurationChannels::AddDataVector(
    const Float_t *variableContainer, Double_t phi, Double_t weight, Int_t channelId) {
  if (IsSelected(variableContainer, channelId)) {
    /// add the data vector to the bank
    QnCorrectionsDataVectorChannelized *channelizedDataVector =
        new (fDataVectorBank->ConstructedAt(fDataVectorBank->GetEntriesFast()))
          QnCorrectionsDataVectorChannelized(channelId, phi, weight);
  }
}

/// Builds raw Qn vector before Q vector corrections and before input
/// data corrections but considering the chosen calibration method.
/// This is a channelized configuration so this Q vector will NOT be
/// the one to be used for subsequent Q vector corrections.
inline void QnCorrectionsDetectorConfigurationChannels::BuildRawQnVector() {
  QnCorrectionsQnVectorBuild qVectorTemp(fRawQnVector);
  qVectorTemp.Reset();

  for(Int_t ixData = 0; ixData < fDataVectorBank->GetEntriesFast(); ixData++){
    QnCorrectionsDataVectorChannelized *dataVector = static_cast<QnCorrectionsDataVectorChannelized *>(fDataVectorBank->At(ixData));
    qVectorTemp.Add(dataVector->Phi(), dataVector->Weight());
  }
  qVectorTemp.Calibrate(fQnCalibrationMethod);
  fRawQnVector.Set(&qVectorTemp);
}

/// Builds raw Qn vector before Q vector corrections and before input
/// data corrections but considering the chosen calibration method.
/// This is a channelized configuration so this Q vector will NOT be
/// the one to be used for subsequent Q vector corrections.
/// Builds Qn vector before Q vector corrections and after input corrections
/// and considering the chosen calibration method.
/// The built Q vector is the one to be used for
/// subsequent Q vector corrections.
inline void QnCorrectionsDetectorConfigurationChannels::BuildQnVector() {
  QnCorrectionsQnVectorBuild qVectorTemp(fQnVector);
  qVectorTemp.Reset();

  for(Int_t ixData = 0; ixData < fDataVectorBank->GetEntriesFast(); ixData++){
    QnCorrectionsDataVectorChannelized *dataVector = static_cast<QnCorrectionsDataVectorChannelized *>(fDataVectorBank->At(ixData));
    qVectorTemp.Add(dataVector->Phi(), dataVector->EqualizedWeight());
  }
  qVectorTemp.Calibrate(fQnCalibrationMethod);
  fQnVector.Set(&qVectorTemp);
}


/// Ask for processing corrections for the involved detector configuration
///
/// The request is transmitted to the incoming data correction steps
/// and to then to Q vector correction steps via
/// \return kTRUE if everything went OK
inline Bool_t QnCorrectionsDetectorConfigurationChannels::ProcessCorrections(const Float_t *variableContainer) {

  /* first we build the raw Q vector with the chosen calibration */
  BuildRawQnVector();

  /* then we transfer the request to the input data correction steps */
  Bool_t retValue = kFALSE;
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    retValue = kTRUE;
    retValue = retValue && (fInputDataCorrections.At(ixCorrection)->Process(variableContainer));
    if (retValue)
      continue;
    else
      break;
  }

  if (retValue) {
    /* input corrections were applied so let's build the Q vector with the chosen calibration */
    BuildQnVector();

    /* now lt's propagate it to Q vector corrections */
    for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
      retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->Process(variableContainer));
      if (retValue)
        continue;
      else
        break;
    }
  }
  return retValue;
}

/// Clean the configuration to accept a new event
///
/// Transfers the order to the Q vector correction steps then
/// to the input data correction steps and finally
/// cleans the own Q vector and the input data vector bank
/// for accepting the next event.
inline void QnCorrectionsDetectorConfigurationChannels::ClearConfiguration() {
  /* transfer the order to the Q vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->ClearCorrectionStep();
  }
  /* transfer the order to the data vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    fInputDataCorrections.At(ixCorrection)->ClearCorrectionStep();
  }
  /* clean the raw Q vector */
  fRawQnVector.Reset();
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

/// Ask for processing corrections for the involved detector
///
/// The request is transmitted to the attached detector configurations
/// \return kTRUE if everything went OK
inline Bool_t QnCorrectionsDetector::ProcessCorrections(const Float_t *variableContainer) {
  Bool_t retValue = kTRUE;

  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    Bool_t ret = fConfigurations.At(ixConfiguration)->ProcessCorrections(variableContainer);
    retValue = retValue && ret;
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
