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

/// \file QnCorrectionsDetector.cxx
/// \brief Detector and detector configuration classes implementation

#include "QnCorrectionsDetector.h"
#include "QnCorrectionsLog.h"

/// The default initial size of data vectors banks
#define INITIALDATAVECTORBANKSIZE 100000
/// \cond CLASSIMP
ClassImp(QnCorrectionsDetector);
/// \endcond


/// Default constructor
QnCorrectionsDetector::QnCorrectionsDetector() : TNamed(),
    fConfigurations(),
    fDataVectorAcceptedConfigurations() {

  fDetectorId = -1;
  fDataVectorAcceptedConfigurations.SetOwner(kFALSE);
}

/// Normal constructor
/// \param name the name of the detector
/// \param id detector Id
QnCorrectionsDetector::QnCorrectionsDetector(const char *name, Int_t id) :
    TNamed(name,name),
    fConfigurations(),
    fDataVectorAcceptedConfigurations() {

  fDetectorId = id;
  fDataVectorAcceptedConfigurations.SetOwner(kFALSE);
}

/// Default destructor
/// The detector class does not own anything
QnCorrectionsDetector::~QnCorrectionsDetector() {

}

/// Asks for support data structures creation
///
/// The request is transmitted to the attached detector configurations
void QnCorrectionsDetector::CreateSupportDataStructures() {

  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->CreateSupportDataStructures();
  }
}

/// Asks for support histograms creation
///
/// The request is transmitted to the attached detector configurations
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetector::CreateSupportHistograms(TList *list) {
  Bool_t retValue = kTRUE;
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    retValue = retValue && (fConfigurations.At(ixConfiguration)->CreateSupportHistograms(list));
  }
  return retValue;
}

/// Asks for QA histograms creation
///
/// The request is transmitted to the attached detector configurations
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetector::CreateQAHistograms(TList *list) {
  Bool_t retValue = kTRUE;
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    retValue = retValue && (fConfigurations.At(ixConfiguration)->CreateQAHistograms(list));
  }
  return retValue;
}

/// Asks for attaching the needed input information to the correction steps
///
/// The request is transmitted to the attached detector configurations
/// \param list list where the input information should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetector::AttachCorrectionInputs(TList *list) {
  /* TODO: do we need to fine tune the list passed according to the detector? */
  Bool_t retValue = kTRUE;
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    Bool_t ret = fConfigurations.At(ixConfiguration)->AttachCorrectionInputs(list);
    retValue = retValue && ret;
  }
  return retValue;
}

/// Adds a new detector configuration to the current detector
///
/// Raise an execution error if the configuration detector reference
/// is not empty and if the detector configuration
/// is already incorporated to the detector.
/// \param detectorConfiguration pointer to the configuration to be added
void QnCorrectionsDetector::AddDetectorConfiguration(QnCorrectionsDetectorConfigurationBase *detectorConfiguration) {
  if (detectorConfiguration->GetDetector() != NULL) {
    QnCorrectionsFatal(Form("You are adding %s detector configuration of detector Id %d to detector Id %d. FIX IT, PLEASE.",
        detectorConfiguration->GetName(),
        detectorConfiguration->GetDetector()->GetId(),
        GetId()));
    return;
  }

  if (fConfigurations.FindObject(detectorConfiguration->GetName())) {
    QnCorrectionsFatal(Form("You are trying to add twice %s detector configuration to detector Id %d. FIX IT, PLEASE.",
        detectorConfiguration->GetName(),
        GetId()));
    return;
  }
  detectorConfiguration->SetDetectorOwner(this);
  fConfigurations.Add(detectorConfiguration);
}

/// Searches for a concrete detector configuration by name
/// \param name the name of the detector configuration to find
/// \return pointer to the found detector configuration (NULL if not found)
QnCorrectionsDetectorConfigurationBase *QnCorrectionsDetector::FindDetectorConfiguration(const char *name) {
  return (QnCorrectionsDetectorConfigurationBase *) fConfigurations.FindObject(name);
}

/// Include the the list of Qn vector associated to the detector
/// into the passed list
///
/// The request is transmitted to the attached detector configurations
/// \param list list where the corrected Qn vector should be added
void QnCorrectionsDetector::IncludeQnVectors(TList *list) {

  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->IncludeQnVectors(list);
  }
}

/// Include the name of each detector configuration into the passed list
///
/// \param list the list where to incorporate detector configurations name
void QnCorrectionsDetector::FillDetectorConfigurationNameList(TList *list) const {
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    list->Add(new TObjString(fConfigurations.At(ixConfiguration)->GetName()));
  }
}

/// Include the name of the input correction steps on each detector
/// configuration into the passed list
///
/// The request is transmitted to the attached detector configurations
/// \param list list where the corrected Qn vector should be added
void QnCorrectionsDetector::FillOverallInputCorrectionStepList(TList *list) const {
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->FillOverallInputCorrectionStepList(list) ;
  }
}

/// Include the name of the Qn vector correction steps on each detector
/// configuration into the passed list
///
/// The request is transmitted to the attached detector configurations
/// \param list list where the corrected Qn vector should be added
void QnCorrectionsDetector::FillOverallQnVectorCorrectionStepList(TList *list) const {
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->FillOverallQnVectorCorrectionStepList(list) ;
  }
}


/// Provide information about assigned corrections on each of the detector configurations
///
/// The request is transmitted to the attached detector configurations
/// \param steps list for incorporating the list of assigned correction steps
/// \param calib list for incorporating the list of steps in calibrating status
/// \param apply list for incorporating the list of steps in applying status
void QnCorrectionsDetector::ReportOnCorrections(TList *steps, TList *calib, TList *apply) const {
  for (Int_t ixConfiguration = 0; ixConfiguration < fConfigurations.GetEntriesFast(); ixConfiguration++) {
    fConfigurations.At(ixConfiguration)->ReportOnCorrections(steps, calib, apply) ;
  }
}


/// \cond CLASSIMP
ClassImp(QnCorrectionsDetectorConfigurationBase);
/// \endcond

const char *QnCorrectionsDetectorConfigurationBase::szPlainQnVectorName = "plain";


/// Default constructor
QnCorrectionsDetectorConfigurationBase::QnCorrectionsDetectorConfigurationBase() : TNamed(),
    fPlainQnVector(), fCorrectedQnVector(), fTempQnVector(), fQnVectorCorrections() {
  fDetector = NULL;
  fCuts = NULL;
  fDataVectorBank = NULL;
  fQnNormalizationMethod = QnCorrectionsQnVector::QVNORM_noCalibration;
  fEventClassVariables = NULL;
}

/// Normal constructor
/// \param name the name of the detector configuration
/// \param eventClassesVariables the set of event classes variables
/// \param nNoOfHarmonics the number of harmonics that must be handled
/// \param harmonicMap an optional ordered array with the harmonic numbers
QnCorrectionsDetectorConfigurationBase::QnCorrectionsDetectorConfigurationBase(const char *name,
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap) :
          TNamed(name,name),
          fPlainQnVector(szPlainQnVectorName,nNoOfHarmonics, harmonicMap),
          fCorrectedQnVector(szPlainQnVectorName,nNoOfHarmonics, harmonicMap),
          fTempQnVector("temp",nNoOfHarmonics, harmonicMap),
          fQnVectorCorrections() {

  fDetector = NULL;
  fCuts = NULL;
  fDataVectorBank = NULL;
  fQnNormalizationMethod = QnCorrectionsQnVector::QVNORM_noCalibration;
  fEventClassVariables = eventClassesVariables;
}

/// Default destructor
/// Releases the memory which was taken or passed
QnCorrectionsDetectorConfigurationBase::~QnCorrectionsDetectorConfigurationBase() {
  if (fDataVectorBank != NULL) {
    delete fDataVectorBank;
  }
  if (fCuts != NULL) {
    delete fCuts;
  }
}

/// Incorporates the passed correction to the set of Q vector corrections
/// \param correctionOnQn the correction to add
void QnCorrectionsDetectorConfigurationBase::AddCorrectionOnQnVector(QnCorrectionsCorrectionOnQvector *correctionOnQn) {
  correctionOnQn->SetConfigurationOwner(this);
  fQnVectorCorrections.AddCorrection(correctionOnQn);
}

/// Incorporates the passed correction to the set of input data corrections
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param correctionOnInputData the correction to add
void QnCorrectionsDetectorConfigurationBase::AddCorrectionOnInputData(QnCorrectionsCorrectionOnInputData *correctionOnInputData) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a non channelized detector configuration to calibrate input data. FIX IT, PLEASE.",
      "QnCorrectionsDetectorConfigurationBase::AddCorrectionOnInputData()"));
}

/// Checks if the current content of the variable bank applies to
/// the detector configuration
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param variableContainer pointer to the variable content bank
/// \return kTRUE if the current content applies to the configuration
Bool_t QnCorrectionsDetectorConfigurationBase::IsSelected(const Float_t *variableContainer) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a channelized detector configuration without passing the channel number. FIX IT, PLEASE.",
      "QnCorrectionsDetectorConfigurationBase::IsSelected()"));
  return kFALSE;
}

/// Checks if the current content of the variable bank applies to
/// the detector configuration for the passed channel.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param variableContainer pointer to the variable content bank
/// \param nChannel the interested external channel number
/// \return kTRUE if the current content applies to the configuration
Bool_t QnCorrectionsDetectorConfigurationBase::IsSelected(const Float_t *variableContainer, Int_t nChannel) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a non channelized detector configuration but passing a channel number. FIX IT, PLEASE.",
      "QnCorrectionsDetectorConfigurationBase::IsSelected()"));
  return kFALSE;
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsDetectorConfigurationTracks);
/// \endcond

/// Default constructor
QnCorrectionsDetectorConfigurationTracks::QnCorrectionsDetectorConfigurationTracks() : QnCorrectionsDetectorConfigurationBase() {

}

/// Normal constructor
/// Allocates the data vector bank.
/// \param name the name of the detector configuration
/// \param eventClassesVariables the set of event classes variables
/// \param nNoOfHarmonics the number of harmonics that must be handled
/// \param harmonicMap an optional ordered array with the harmonic numbers
QnCorrectionsDetectorConfigurationTracks::QnCorrectionsDetectorConfigurationTracks(const char *name,
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap) :
          QnCorrectionsDetectorConfigurationBase(name, eventClassesVariables, nNoOfHarmonics, harmonicMap) {

}

/// Default destructor
/// Memory taken is released by the parent class destructor
QnCorrectionsDetectorConfigurationTracks::~QnCorrectionsDetectorConfigurationTracks() {

}

/// Asks for support data structures creation
///
/// The input data vector bank is allocated and the request is
/// transmitted to the Q vector corrections.
void QnCorrectionsDetectorConfigurationTracks::CreateSupportDataStructures() {

  /* this is executed in the remote node so, allocate the data bank */
  fDataVectorBank = new TClonesArray("QnCorrectionsDataVector", INITIALDATAVECTORBANKSIZE);

  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->CreateSupportDataStructures();
  }
}

/// Asks for support histograms creation
///
/// The request is transmitted to the Q vector corrections.
///
/// A new histograms list is created for the detector configuration and incorporated
/// to the passed list. Then the new list is passed to the corrections.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetectorConfigurationTracks::CreateSupportHistograms(TList *list) {
  Bool_t retValue = kTRUE;
  TList *detectorConfigurationList = new TList();
  detectorConfigurationList->SetName(this->GetName());
  detectorConfigurationList->SetOwner(kTRUE);
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->CreateSupportHistograms(detectorConfigurationList));
  }
  /* if list is empty delete it if not incorporate it */
  if (detectorConfigurationList->GetEntries() != 0) {
    list->Add(detectorConfigurationList);
  }
  else {
    delete detectorConfigurationList;
  }
  return retValue;
}

/// Asks for QA histograms creation
///
/// The request is transmitted to the Q vector corrections.
///
/// A new histograms list is created for the detector and incorporated
/// to the passed list. Then the new list is passed to the corrections.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetectorConfigurationTracks::CreateQAHistograms(TList *list) {
  Bool_t retValue = kTRUE;
  TList *detectorConfigurationList = new TList();
  detectorConfigurationList->SetName(this->GetName());
  detectorConfigurationList->SetOwner(kTRUE);
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->CreateQAHistograms(detectorConfigurationList));
  }
  /* if list is empty delete it if not incorporate it */
  if (detectorConfigurationList->GetEntries() != 0) {
    list->Add(detectorConfigurationList);
  }
  else {
    delete detectorConfigurationList;
  }
  return retValue;
}

/// Asks for attaching the needed input information to the correction steps
///
/// The detector list is extracted from the passed list and then
/// the request is transmitted to the Q vector corrections with the found list.
/// \param list list where the input information should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetectorConfigurationTracks::AttachCorrectionInputs(TList *list) {
  TList *detectorConfigurationList = (TList *) list->FindObject(this->GetName());
  if (detectorConfigurationList != NULL) {
    Bool_t retValue = kTRUE;
    for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
      retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->AttachInput(detectorConfigurationList));
    }
    return retValue;
  }
  return kFALSE;
}

/// Include the the list of Qn vector associated to the detector configuration
/// into the passed list
///
/// A new list is created for the detector configuration and incorporated
/// to the passed list.
///
/// Always includes first the fully corrected Qn vector,
/// and then includes the plain Qn vector and asks to the different correction
/// steps to include their partially corrected Qn vectors.
/// The check if we are already there is because it could be late information
/// about the process name and then the correction histograms could still not
/// be attached and the constructed list does not contain the final Qn vectors.
/// \param list list where the corrected Qn vector should be added
void QnCorrectionsDetectorConfigurationTracks::IncludeQnVectors(TList *list) {

  /* we check whether we are already there and if so we clean it and go again */
  Bool_t bAlreadyThere;
  TList *detectorConfigurationList;
  if (list->FindObject(this->GetName())) {
    detectorConfigurationList = (TList*) list->FindObject(this->GetName());
    detectorConfigurationList->Clear();
    bAlreadyThere = kTRUE;
  }
  else {
    detectorConfigurationList = new TList();
    detectorConfigurationList->SetName(this->GetName());
    detectorConfigurationList->SetOwner(kFALSE);
    bAlreadyThere = kFALSE;
  }

  detectorConfigurationList->Add(&fCorrectedQnVector);
  detectorConfigurationList->Add(&fPlainQnVector);
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->IncludeCorrectedQnVector(detectorConfigurationList);
  }
  if (!bAlreadyThere)
    list->Add(detectorConfigurationList);
}

/// Include only one instance of each input correction step
/// in execution order
///
/// There are not input correction so we do nothing
/// \param list list where the correction steps should be incorporated
void QnCorrectionsDetectorConfigurationTracks::FillOverallInputCorrectionStepList(TList *list) const {

}

/// Include only one instance of each Qn vector correction step
/// in execution order
///
/// The request is transmitted to the set of Qn vector corrections
/// \param list list where the correction steps should be incorporated
void QnCorrectionsDetectorConfigurationTracks::FillOverallQnVectorCorrectionStepList(TList *list) const {

  fQnVectorCorrections.FillOverallCorrectionsList(list);
}

/// Provide information about assigned corrections
///
/// We create three list which items they own, incorporate info from the
/// correction steps and add them to the passed list
/// \param steps list for incorporating the list of assigned correction steps
/// \param calib list for incorporating the list of steps in calibrating status
/// \param apply list for incorporating the list of steps in applying status
void QnCorrectionsDetectorConfigurationTracks::ReportOnCorrections(TList *steps, TList *calib, TList *apply) const {
  TList *mysteps = new TList();
  mysteps->SetOwner(kTRUE);
  mysteps->SetName(GetName());
  TList *mycalib = new TList();
  mycalib->SetOwner(kTRUE);
  mycalib->SetName(GetName());
  TList *myapply = new TList();
  myapply->SetOwner(kTRUE);
  myapply->SetName(GetName());

  /* incorporate Qn vector corrections */
  Bool_t keepIncorporating = kTRUE;
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    mysteps->Add(new TObjString(fQnVectorCorrections.At(ixCorrection)->GetName()));
    /* incorporate additional info if the step will be reached */
    if (keepIncorporating) {
      Bool_t keep = fQnVectorCorrections.At(ixCorrection)->ReportUsage(mycalib,myapply);
      keepIncorporating = keepIncorporating && keep;
    }
  }
  steps->Add(mysteps);
  calib->Add(mycalib);
  apply->Add(myapply);
}


/// \cond CLASSIMP
ClassImp(QnCorrectionsDetectorConfigurationChannels);
/// \endcond

const char *QnCorrectionsDetectorConfigurationChannels::szRawQnVectorName = "raw";
const char *QnCorrectionsDetectorConfigurationChannels::szQAMultiplicityHistoName = "Multiplicity";

/// Default constructor
QnCorrectionsDetectorConfigurationChannels::QnCorrectionsDetectorConfigurationChannels() :
    QnCorrectionsDetectorConfigurationBase(), fRawQnVector(), fInputDataCorrections() {

  fNoOfChannels = 0;
  fUsedChannel = NULL;
  fChannelMap = NULL;
  fChannelGroup = NULL;
  fHardCodedGroupWeights = NULL;
  /* QA section */
  fQACentralityVarId = -1;
  fQAnBinsMultiplicity = 100;
  fQAMultiplicityMin = 0.0;
  fQAMultiplicityMax = 1000.0;
  fQAMultiplicityBefore3D = NULL;
  fQAMultiplicityAfter3D = NULL;
}

/// Normal constructor
/// Allocates the data vector bank.
/// \param name the name of the detector configuration
/// \param eventClassesVariables the set of event classes variables
/// \param nNoOfChannels the number of channels of the associated detector
/// \param nNoOfHarmonics the number of harmonics that must be handled
/// \param harmonicMap an optional ordered array with the harmonic numbers
QnCorrectionsDetectorConfigurationChannels::QnCorrectionsDetectorConfigurationChannels(const char *name,
      QnCorrectionsEventClassVariablesSet *eventClassesVariables,
      Int_t nNoOfChannels,
      Int_t nNoOfHarmonics,
      Int_t *harmonicMap) :
          QnCorrectionsDetectorConfigurationBase(name, eventClassesVariables, nNoOfHarmonics, harmonicMap),
          fRawQnVector(szRawQnVectorName, nNoOfHarmonics, harmonicMap),
          fInputDataCorrections() {
  fNoOfChannels = nNoOfChannels;
  fUsedChannel = NULL;
  fChannelMap = NULL;
  fChannelGroup = NULL;
  fHardCodedGroupWeights = NULL;
  /* QA section */
  fQACentralityVarId = -1;
  fQAnBinsMultiplicity = 100;
  fQAMultiplicityMin = 0.0;
  fQAMultiplicityMax = 1000.0;
  fQAMultiplicityBefore3D = NULL;
  fQAMultiplicityAfter3D = NULL;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsDetectorConfigurationChannels::~QnCorrectionsDetectorConfigurationChannels() {

  if (fUsedChannel != NULL) delete fUsedChannel;
  if (fChannelMap != NULL) delete fChannelMap;
  if (fChannelGroup != NULL) delete fChannelGroup;
  if (fHardCodedGroupWeights != NULL) delete fHardCodedGroupWeights;
}

/// Incorporates the channels scheme to the detector configuration
/// \param bUsedChannel array of booleans one per each channel
///        If NULL all channels in fNoOfChannels are allocated to the detector configuration
/// \param nChannelGroup array of group number for each channel
///        If NULL all channels in fNoOfChannels are assigned to a unique group
/// \param hardCodedGroupWeights array with hard coded weight for each group
///        If NULL no hard coded weight is assigned (i.e. weight = 1)
void QnCorrectionsDetectorConfigurationChannels::SetChannelsScheme(
    Bool_t *bUsedChannel,
    Int_t *nChannelGroup,
    Float_t *hardCodedGroupWeights) {
  /* TODO: there should be smart procedures on how to improve the channels scan for actual data */
  fUsedChannel = new Bool_t[fNoOfChannels];
  fChannelMap = new Int_t[fNoOfChannels];
  fChannelGroup = new Int_t[fNoOfChannels];

  Int_t nMinGroup = 0xFFFF;
  Int_t nMaxGroup = 0x0000;
  Int_t intChannelNo = 0;
  for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
    if (bUsedChannel != NULL)
      fUsedChannel[ixChannel] = bUsedChannel[ixChannel];
    else
      fUsedChannel[ixChannel] = kTRUE;
    if (fUsedChannel[ixChannel]) {
      fChannelMap[ixChannel] = intChannelNo;
      intChannelNo++;
      if (nChannelGroup != NULL) {
        fChannelGroup[ixChannel] = nChannelGroup[ixChannel];
        /* update min max group number */
        if (nChannelGroup[ixChannel] < nMinGroup)
          nMinGroup = nChannelGroup[ixChannel];
        if (nMaxGroup < nChannelGroup[ixChannel])
          nMaxGroup = nChannelGroup[ixChannel];
      }
      else {
        fChannelGroup[ixChannel] = 0;
        nMinGroup = 0;
        nMaxGroup = 0;
      }
    }
  }
  Bool_t bUseGroups = (hardCodedGroupWeights != NULL) && (nChannelGroup != NULL) && (nMinGroup != nMaxGroup);

  /* store the hard coded group weights assigned to each channel if applicable */
  if (bUseGroups) {
    fHardCodedGroupWeights = new Float_t[fNoOfChannels];
    for (Int_t i = 0; i < fNoOfChannels; i++) {
      fHardCodedGroupWeights[i] = 0.0;
    }
    for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
      if (fUsedChannel[ixChannel]) {
        fHardCodedGroupWeights[ixChannel] =
            hardCodedGroupWeights[fChannelGroup[ixChannel]];
      }
    }
  }
}

/// Asks for support data structures creation
///
/// The input data vector bank is allocated and the request is
/// transmitted to the input data corrections and then to the Q vector corrections.
void QnCorrectionsDetectorConfigurationChannels::CreateSupportDataStructures() {

  /* this is executed in the remote node so, allocate the data bank */
  fDataVectorBank = new TClonesArray("QnCorrectionsDataVectorChannelized", INITIALDATAVECTORBANKSIZE);

  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    fInputDataCorrections.At(ixCorrection)->CreateSupportDataStructures();
  }

  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->CreateSupportDataStructures();
  }
}

/// Asks for support histograms creation
///
/// A new histograms list is created for the detector and incorporated
/// to the passed list. Then the new list is passed first to the input data corrections
/// and then to the Q vector corrections.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetectorConfigurationChannels::CreateSupportHistograms(TList *list) {

  TList *detectorConfigurationList = new TList();
  detectorConfigurationList->SetName(this->GetName());
  detectorConfigurationList->SetOwner(kTRUE);
  Bool_t retValue = kTRUE;
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue && (fInputDataCorrections.At(ixCorrection)->CreateSupportHistograms(detectorConfigurationList));
  }

  /* if everything right propagate it to Q vector corrections */
  if (retValue) {
    for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
      retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->CreateSupportHistograms(detectorConfigurationList));
    }
  }
  /* if list is empty delete it if not incorporate it */
  if (detectorConfigurationList->GetEntries() != 0) {
    list->Add(detectorConfigurationList);
  }
  else {
    delete detectorConfigurationList;
  }
  return retValue;
}

/// Asks for QA histograms creation
///
/// A new histograms list is created for the detector and incorporated
/// to the passed list. The own QA histograms are then created and incorporated
/// to the new list. Then the new list is passed first to the input data corrections
/// and then to the Q vector corrections.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetectorConfigurationChannels::CreateQAHistograms(TList *list) {
  TList *detectorConfigurationList = new TList();
  detectorConfigurationList->SetName(this->GetName());
  detectorConfigurationList->SetOwner(kTRUE);

  /* first create our own QA histograms */
  TString beforeName = GetName();
  beforeName += szQAMultiplicityHistoName;
  beforeName += "Before";
  TString beforeTitle = GetName();
  beforeTitle += " ";
  beforeTitle += szQAMultiplicityHistoName;
  beforeTitle += " before input equalization";
  TString afterName = GetName();
  afterName += szQAMultiplicityHistoName;
  afterName += "After";
  TString afterTitle = GetName();
  afterTitle += " ";
  afterTitle += szQAMultiplicityHistoName;
  afterTitle += " after input equalization";

  /* let's pick the centrality variable and its binning */
  Int_t ixVarId = -1;
  for (Int_t ivar = 0; ivar < fEventClassVariables->GetEntries(); ivar++) {
    if (fEventClassVariables->At(ivar)->GetVariableId() != fQACentralityVarId) {
      continue;
    }
    else {
      ixVarId = ivar;
      break;
    }
  }

  /* let's get the effective number of channels */
  Int_t nNoOfChannels = 0;
  for (Int_t i = 0; i < fNoOfChannels; i++)
    if (fUsedChannel[i])
      nNoOfChannels++;

  if (ixVarId != -1) {
    fQAMultiplicityBefore3D = new TH3F(
        (const char *) beforeName,
        (const char *) beforeTitle,
        fEventClassVariables->At(ixVarId)->GetNBins(),
        fEventClassVariables->At(ixVarId)->GetLowerEdge(),
        fEventClassVariables->At(ixVarId)->GetUpperEdge(),
        nNoOfChannels,
        0.0,
        nNoOfChannels,
        fQAnBinsMultiplicity,
        fQAMultiplicityMin,
        fQAMultiplicityMax);
    fQAMultiplicityAfter3D = new TH3F(
        (const char *) afterName,
        (const char *) afterTitle,
        fEventClassVariables->At(ixVarId)->GetNBins(),
        fEventClassVariables->At(ixVarId)->GetLowerEdge(),
        fEventClassVariables->At(ixVarId)->GetUpperEdge(),
        nNoOfChannels,
        0.0,
        nNoOfChannels,
        fQAnBinsMultiplicity,
        fQAMultiplicityMin,
        fQAMultiplicityMax);
    /* now set the proper labels and titles */
    fQAMultiplicityBefore3D->GetXaxis()->SetTitle(fEventClassVariables->At(ixVarId)->GetVariableLabel());
    fQAMultiplicityBefore3D->GetYaxis()->SetTitle("channel");
    fQAMultiplicityBefore3D->GetZaxis()->SetTitle("M");
    fQAMultiplicityAfter3D->GetXaxis()->SetTitle(fEventClassVariables->At(ixVarId)->GetVariableLabel());
    fQAMultiplicityAfter3D->GetYaxis()->SetTitle("channel");
    fQAMultiplicityAfter3D->GetZaxis()->SetTitle("M");
    if (fNoOfChannels != nNoOfChannels) {
      Int_t bin = 1;
      for (Int_t i = 0; i < fNoOfChannels; i++)
        if (fUsedChannel[i]) {
          fQAMultiplicityBefore3D->GetYaxis()->SetBinLabel(bin, Form("%d", i));
          fQAMultiplicityAfter3D->GetYaxis()->SetBinLabel(bin, Form("%d", i));
          bin++;
        }
    }

    detectorConfigurationList->Add(fQAMultiplicityBefore3D);
    detectorConfigurationList->Add(fQAMultiplicityAfter3D);
  }

  /* now propagate it to the input data corrections */
  Bool_t retValue = kTRUE;
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    retValue = retValue && (fInputDataCorrections.At(ixCorrection)->CreateQAHistograms(detectorConfigurationList));
  }

  /* if everything right propagate it to Q vector corrections */
  if (retValue) {
    for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
      retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->CreateQAHistograms(detectorConfigurationList));
    }
  }
  /* now incorporate the list to the passed one */
  if (detectorConfigurationList->GetEntries() != 0)
    list->Add(detectorConfigurationList);
  else
    delete detectorConfigurationList;

  return retValue;
}

/// Asks for attaching the needed input information to the correction steps
///
/// The detector list is extracted from the passed list and then
/// the request is transmitted to the input data corrections
/// and then propagated to the Q vector corrections
/// \param list list where the input information should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsDetectorConfigurationChannels::AttachCorrectionInputs(TList *list) {
  TList *detectorConfigurationList = (TList *) list->FindObject(this->GetName());
  if (detectorConfigurationList != NULL) {
    Bool_t retValue = kTRUE;
    for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
      retValue = retValue && (fInputDataCorrections.At(ixCorrection)->AttachInput(detectorConfigurationList));
    }

    /* now propagate it to Q vector corrections */
    for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
      retValue = retValue && (fQnVectorCorrections.At(ixCorrection)->AttachInput(detectorConfigurationList));
    }
    return retValue;
  }
  return kFALSE;
}

/// Incorporates the passed correction to the set of input data corrections
/// \param correctionOnInputData the correction to add
void QnCorrectionsDetectorConfigurationChannels::AddCorrectionOnInputData(QnCorrectionsCorrectionOnInputData *correctionOnInputData) {
  correctionOnInputData->SetConfigurationOwner(this);
  fInputDataCorrections.AddCorrection(correctionOnInputData);
}

/// Fills the multiplicity histograms before and after input equalization
/// \param variableContainer pointer to the variable content bank
void QnCorrectionsDetectorConfigurationChannels::FillQAHistograms(const Float_t *variableContainer) {
  for(Int_t ixData = 0; ixData < fDataVectorBank->GetEntriesFast(); ixData++){
    QnCorrectionsDataVectorChannelized *dataVector =
        static_cast<QnCorrectionsDataVectorChannelized *>(fDataVectorBank->At(ixData));
    fQAMultiplicityBefore3D->Fill(variableContainer[fQACentralityVarId], fChannelMap[dataVector->GetId()], dataVector->Weight());
    fQAMultiplicityAfter3D->Fill(variableContainer[fQACentralityVarId], fChannelMap[dataVector->GetId()], dataVector->EqualizedWeight());
  }
}

/// Include the the list of Qn vector associated to the detector configuration
/// into the passed list
///
/// A new list is created for the detector configuration and incorporated
/// to the passed list.
///
/// Always includes first the fully corrected Qn vector,
/// and then includes the raw Qn vector and the plain Qn vector and then
/// asks to the different correction
/// steps to include their partially corrected Qn vectors.
///
/// The check if we are already there is because it could be late information
/// about the process name and then the correction histograms could still not
/// be attached and the constructed list does not contain the final Qn vectors.
/// \param list list where the corrected Qn vector should be added
inline void QnCorrectionsDetectorConfigurationChannels::IncludeQnVectors(TList *list) {

  /* we check whether we are already there and if so we clean it and go again */
  Bool_t bAlreadyThere;
  TList *detectorConfigurationList;
  if (list->FindObject(this->GetName())) {
    detectorConfigurationList = (TList*) list->FindObject(this->GetName());
    detectorConfigurationList->Clear();
    bAlreadyThere = kTRUE;
  }
  else {
    detectorConfigurationList = new TList();
    detectorConfigurationList->SetName(this->GetName());
    detectorConfigurationList->SetOwner(kFALSE);
    bAlreadyThere = kFALSE;
  }

  detectorConfigurationList->Add(&fCorrectedQnVector);
  detectorConfigurationList->Add(&fRawQnVector);
  detectorConfigurationList->Add(&fPlainQnVector);
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    fQnVectorCorrections.At(ixCorrection)->IncludeCorrectedQnVector(detectorConfigurationList);
  }
  if (!bAlreadyThere)
    list->Add(detectorConfigurationList);
}

/// Include only one instance of each input correction step
/// in execution order
///
/// The request is transmitted to the set of Qn vector corrections
/// \param list list where the correction steps should be incorporated
void QnCorrectionsDetectorConfigurationChannels::FillOverallInputCorrectionStepList(TList *list) const {

  fInputDataCorrections.FillOverallCorrectionsList(list);
}

/// Include only one instance of each Qn vector correction step
/// in execution order
///
/// The request is transmitted to the set of Qn vector corrections
/// \param list list where the correction steps should be incorporated
void QnCorrectionsDetectorConfigurationChannels::FillOverallQnVectorCorrectionStepList(TList *list) const {

  fQnVectorCorrections.FillOverallCorrectionsList(list);
}

/// Provide information about assigned corrections
///
/// We create three list which items they own, incorporate info from the
/// correction steps and add them to the passed list
/// \param steps list for incorporating the list of assigned correction steps
/// \param calib list for incorporating the list of steps in calibrating status
/// \param apply list for incorporating the list of steps in applying status
void QnCorrectionsDetectorConfigurationChannels::ReportOnCorrections(TList *steps, TList *calib, TList *apply) const {
  TList *mysteps = new TList();
  mysteps->SetOwner(kTRUE);
  mysteps->SetName(GetName());
  TList *mycalib = new TList();
  mycalib->SetOwner(kTRUE);
  mycalib->SetName(GetName());
  TList *myapply = new TList();
  myapply->SetOwner(kTRUE);
  myapply->SetName(GetName());

  /* first the input data corrections */
  Bool_t keepIncorporating = kTRUE;
  for (Int_t ixCorrection = 0; ixCorrection < fInputDataCorrections.GetEntries(); ixCorrection++) {
    mysteps->Add(new TObjString(fInputDataCorrections.At(ixCorrection)->GetName()));
    /* incorporate additional info if the step will be reached */
    if (keepIncorporating) {
      Bool_t keep = fInputDataCorrections.At(ixCorrection)->ReportUsage(mycalib,myapply);
      keepIncorporating = keepIncorporating && keep;
    }
  }
  /* now the Qn vector corrections */
  for (Int_t ixCorrection = 0; ixCorrection < fQnVectorCorrections.GetEntries(); ixCorrection++) {
    mysteps->Add(new TObjString(fQnVectorCorrections.At(ixCorrection)->GetName()));
    /* incorporate additional info if the step will be reached */
    if (keepIncorporating) {
      Bool_t keep = fQnVectorCorrections.At(ixCorrection)->ReportUsage(mycalib,myapply);
      keepIncorporating = keepIncorporating && keep;
    }
  }
  steps->Add(mysteps);
  calib->Add(mycalib);
  apply->Add(myapply);
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsDetectorConfigurationSet);
/// \endcond

/// Default constructor
QnCorrectionsDetectorConfigurationSet::QnCorrectionsDetectorConfigurationSet() :
    TObjArray() {

}

/// Default destructor
QnCorrectionsDetectorConfigurationSet::~QnCorrectionsDetectorConfigurationSet() {

}
