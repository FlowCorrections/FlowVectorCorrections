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

/// \file QnCorrectionsInputGainEqualization.cxx
/// \brief Implementation of procedures for gain equalization on input data.
#include "QnCorrectionsEventClasses.h"
#include "QnCorrectionsHistograms.h"
#include "QnCorrectionsCorrectionSteps.h"
#include "QnCorrectionsDetector.h"
#include "QnCorrectionsInputGainEqualization.h"

const Float_t  QnCorrectionsInputGainEqualization::fMinimumSignificantValue = 1E-6;
const char *QnCorrectionsInputGainEqualization::szCorrectionName = "Gain equalization";
const char *QnCorrectionsInputGainEqualization::szKey = "CCCC";
const char *QnCorrectionsInputGainEqualization::szSupportHistogramName = "Multiplicity";
const char *QnCorrectionsInputGainEqualization::szQAHistogramName = "QA Multiplicity";


/// \cond CLASSIMP
ClassImp(QnCorrectionsInputGainEqualization);
/// \endcond

/// Default constructor
/// Passes to the base class the identity data for the Gain equalization correction step
QnCorrectionsInputGainEqualization::QnCorrectionsInputGainEqualization() :
    QnCorrectionsCorrectionOnInputData() {
  fInputHistograms = NULL;
  fCalibrationHistograms = NULL;
  fQAMultiplicityBefore = NULL;
  fQAMultiplicityAfter = NULL;
  fEqualizationMethod = QEQUAL_noEqualization;
  fA = 0.0;
  fB = 1.0;
  fUseChannelGroupsWeights = kFALSE;
  fHardCodedWeights = NULL;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsInputGainEqualization::~QnCorrectionsInputGainEqualization() {
  if (fInputHistograms != NULL)
    delete fInputHistograms;
  if (fCalibrationHistograms != NULL)
    delete fCalibrationHistograms;
  if (fQAMultiplicityBefore != NULL)
    delete fQAMultiplicityBefore;
  if (fQAMultiplicityAfter != NULL)
    delete fQAMultiplicityAfter;
}

/// Attaches the needed input information to the correction step
///
/// If the attachment succeeded asks for hard coded group weights to
/// the detector configuration
/// \param list list where the inputs should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsInputGainEqualization::AttachInput(TList *list) {
  QnCorrectionsDetectorConfigurationChannels *ownerConfiguration =
      static_cast<QnCorrectionsDetectorConfigurationChannels *>(fDetectorConfiguration);
  if (fInputHistograms->AttachHistograms(list,
      ownerConfiguration->GetUsedChannelsMask(), ownerConfiguration->GetChannelsGroups())) {
    fState = QCORRSTEP_applyCollect;
    fHardCodedWeights = ownerConfiguration->GetHardCodedGroupWeights();
    return kTRUE;
  }
  return kFALSE;
}

/// Asks for support histograms creation
///
/// Allocates the histogram objects and creates the calibration histograms.
/// The histograms are constructed with standard deviation error calculation
/// for the proper behavior of the gain equalization.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsInputGainEqualization::CreateSupportHistograms(TList *list) {
  QnCorrectionsDetectorConfigurationChannels *ownerConfiguration =
      static_cast<QnCorrectionsDetectorConfigurationChannels *>(fDetectorConfiguration);
  fInputHistograms = new QnCorrectionsProfileChannelizedIngress(szSupportHistogramName, szSupportHistogramName,
      ownerConfiguration->GetEventClassVariablesSet(),ownerConfiguration->GetNoOfChannels(), "s");
  fCalibrationHistograms = new QnCorrectionsProfileChannelized(szSupportHistogramName, szSupportHistogramName,
      ownerConfiguration->GetEventClassVariablesSet(),ownerConfiguration->GetNoOfChannels(), "s");
  fCalibrationHistograms->CreateProfileHistograms(list,
      ownerConfiguration->GetUsedChannelsMask(), ownerConfiguration->GetChannelsGroups());
  return kTRUE;
}

/// Asks for QA histograms creation
///
/// Allocates the histogram objects and creates the QA histograms.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsInputGainEqualization::CreateQAHistograms(TList *list) {
  TString beforeName = szSupportHistogramName;
  beforeName += "Before";
  TString beforeTitle = szSupportHistogramName;
  beforeTitle += " before gain equalization";
  TString afterName = szSupportHistogramName;
  afterName += "After";
  TString afterTitle = szSupportHistogramName;
  afterTitle += " after gain equalization";
  QnCorrectionsDetectorConfigurationChannels *ownerConfiguration =
      static_cast<QnCorrectionsDetectorConfigurationChannels *>(fDetectorConfiguration);
  fQAMultiplicityBefore = new QnCorrectionsProfileChannelized(
      (const char *) beforeName,
      (const char *) beforeTitle,
      ownerConfiguration->GetEventClassVariablesSet(),ownerConfiguration->GetNoOfChannels());
  fQAMultiplicityBefore->CreateProfileHistograms(list,
      ownerConfiguration->GetUsedChannelsMask(), ownerConfiguration->GetChannelsGroups());
  fQAMultiplicityAfter = new QnCorrectionsProfileChannelized(
      (const char *) afterName,
      (const char *) afterTitle,
      ownerConfiguration->GetEventClassVariablesSet(),ownerConfiguration->GetNoOfChannels());
  fQAMultiplicityAfter->CreateProfileHistograms(list,
      ownerConfiguration->GetUsedChannelsMask(), ownerConfiguration->GetChannelsGroups());
  return kTRUE;
}

/// Processes the correction step
///
/// Pure virtual function
/// \return kTRUE if the correction step was applied
Bool_t QnCorrectionsInputGainEqualization::Process(const Float_t *variableContainer) {
  switch (fState) {
  case QCORRSTEP_calibration:
    /* collect the data needed to further produce equalization parameters */
    for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
      QnCorrectionsDataVectorChannelized *dataVector =
          static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
      fCalibrationHistograms->Fill(variableContainer, dataVector->GetId(), dataVector->Weight());
    }
    return kFALSE;
    break;
  case QCORRSTEP_applyCollect:
    /* collect the data needed to further produce equalization parameters */
    for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
      QnCorrectionsDataVectorChannelized *dataVector =
          static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
      fCalibrationHistograms->Fill(variableContainer, dataVector->GetId(), dataVector->Weight());
    }
    /* and proceed to ... */
  case QCORRSTEP_apply: /* apply the equalization */
    /* collect QA data if asked */
    if (fQAMultiplicityBefore != NULL) {
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsDataVectorChannelized *dataVector =
            static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        fQAMultiplicityBefore->Fill(variableContainer, dataVector->GetId(), dataVector->Weight());
      }
    }
    /* store the equalized weights in the data vector bank according to equalization method */
    switch (fEqualizationMethod) {
    case QEQUAL_noEqualization:
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsDataVectorChannelized *dataVector =
            static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        dataVector->SetEqualizedWeight(dataVector->Weight());
      }
      break;
    case QEQUAL_averageEqualization:
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsDataVectorChannelized *dataVector =
            static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        Float_t average = fInputHistograms->GetBinContent(fInputHistograms->GetBin(variableContainer, dataVector->GetId()));
        /* let's handle the potential group weights usage */
        Float_t groupweight = 1.0;
        if (fUseChannelGroupsWeights) {
          groupweight = fInputHistograms->GetGrpBinContent(fInputHistograms->GetGrpBin(variableContainer, dataVector->GetId()));
        }
        else {
          if (fHardCodedWeights != NULL) {
            groupweight = fHardCodedWeights[dataVector->GetId()];
          }
        }
        if (fMinimumSignificantValue < average)
          dataVector->SetEqualizedWeight((dataVector->Weight() / average) * groupweight);
        else
          dataVector->SetEqualizedWeight(0.0);
      }
      break;
    case QEQUAL_widthEqualization:
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsDataVectorChannelized *dataVector =
            static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        Float_t average = fInputHistograms->GetBinContent(fInputHistograms->GetBin(variableContainer, dataVector->GetId()));
        Float_t width = fInputHistograms->GetBinError(fInputHistograms->GetBin(variableContainer, dataVector->GetId()));
        /* let's handle the potential group weights usage */
        Float_t groupweight = 1.0;
        if (fUseChannelGroupsWeights) {
          groupweight = fInputHistograms->GetGrpBinContent(fInputHistograms->GetGrpBin(variableContainer, dataVector->GetId()));
        }
        else {
          if (fHardCodedWeights != NULL) {
            groupweight = fHardCodedWeights[dataVector->GetId()];
          }
        }
        if (fMinimumSignificantValue < average)
          dataVector->SetEqualizedWeight((fA + fB * (dataVector->Weight() - average) / width) * groupweight);
        else
          dataVector->SetEqualizedWeight(0.0);
      }
      break;
    }
    /* collect QA data if asked */
    if (fQAMultiplicityAfter != NULL) {
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsDataVectorChannelized *dataVector =
            static_cast<QnCorrectionsDataVectorChannelized *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        fQAMultiplicityAfter->Fill(variableContainer, dataVector->GetId(), dataVector->EqualizedWeight());
      }
    }
    break;
  }
  return kTRUE;
}

