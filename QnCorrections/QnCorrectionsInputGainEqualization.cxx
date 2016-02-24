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

///< the minimum value that will be considered as meaningful for processing
const Float_t  QnCorrectionsInputGainEqualization::fMinimumSignificantValue = 1E-6;
///< the name of the correction step
const char *QnCorrectionsInputGainEqualization::szCorrectionName = "Gain equalization";
///< the key of the correction step for ordering purpose
const char *QnCorrectionsInputGainEqualization::szKey = "CCCC";


/// \cond CLASSIMP
ClassImp(QnCorrectionsInputGainEqualization);
/// \endcond

/// Default constructor
/// Passes to the base class the identity data for the Gain equalization correction step
QnCorrectionsInputGainEqualization::QnCorrectionsInputGainEqualization() :
    QnCorrectionsCorrectionOnInputData() {
  fInputHistograms = NULL;
  fCalibrationHistograms = NULL;
  fEqualizationMethod = QEQUAL_noEqualization;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsInputGainEqualization::~QnCorrectionsInputGainEqualization() {
  if (fInputHistograms != NULL)
    delete fInputHistograms;
  if (fCalibrationHistograms != NULL)
    delete fCalibrationHistograms;
}

/// Attaches the needed input information to the correction step
///
/// Pure virtual function
/// \param list list where the inputs should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsInputGainEqualization::AttachInput(TList *list) {
  QnCorrectionsChannelDetectorConfiguration *ownerConfiguration =
      static_cast<QnCorrectionsChannelDetectorConfiguration *>(fDetectorConfiguration);
  if (fInputHistograms->AttachHistograms(list,
      ownerConfiguration->GetUsedChannelsMask(), ownerConfiguration->GetChannelsGroups())) {
    fState = QCORRSTEP_applyCollect;
    return kTRUE;
  }
  return kFALSE;
}
/// Asks for support histograms creation
///
/// Allocates the histogram objects and creates the calibration histograms.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsInputGainEqualization::CreateSupportHistograms(TList *list) {
  QnCorrectionsChannelDetectorConfiguration *ownerConfiguration =
      static_cast<QnCorrectionsChannelDetectorConfiguration *>(fDetectorConfiguration);
  fInputHistograms = new QnCorrectionsProfileChannelizedIngress("TODOcal", "TODOcal",
      ownerConfiguration->GetEventClassVariablesSet(),ownerConfiguration->GetNoOfChannels());
  fCalibrationHistograms = new QnCorrectionsProfileChannelized("TODOcal", "TODOcal",
      ownerConfiguration->GetEventClassVariablesSet(),ownerConfiguration->GetNoOfChannels());
  fCalibrationHistograms->CreateProfileHistograms(list,
      ownerConfiguration->GetUsedChannelsMask(), ownerConfiguration->GetChannelsGroups());
  return kTRUE;
}
/// Processes the correction step
///
/// Pure virtual function
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsInputGainEqualization::Process(const Float_t *variableContainer) {
  switch (fState) {
  case QCORRSTEP_calibration:
    /* collect the data needed to further produce equalization parameters */
    for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
      QnCorrectionsChannelizedDataVector *dataVector =
          static_cast<QnCorrectionsChannelizedDataVector *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
      fCalibrationHistograms->Fill(variableContainer, dataVector->GetId(), dataVector->Weight());
    }
    return kFALSE;
    break;
  case QCORRSTEP_applyCollect:
    /* collect the data needed to further produce equalization parameters */
    for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
      QnCorrectionsChannelizedDataVector *dataVector =
          static_cast<QnCorrectionsChannelizedDataVector *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
      fCalibrationHistograms->Fill(variableContainer, dataVector->GetId(), dataVector->Weight());
    }
    /* and proceed to ... */
  case QCORRSTEP_apply: /* apply the equalization */
    /* store the equalized weights in the data vector bank according to equalization method */
    switch (fEqualizationMethod) {
    case QEQUAL_noEqualization:
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsChannelizedDataVector *dataVector =
            static_cast<QnCorrectionsChannelizedDataVector *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        dataVector->SetEqualizedWeight(dataVector->Weight());
      }
      break;
    case QEQUAL_averageEqualization:
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsChannelizedDataVector *dataVector =
            static_cast<QnCorrectionsChannelizedDataVector *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        Float_t average = fInputHistograms->GetBinContent(fInputHistograms->GetBin(variableContainer, dataVector->GetId()));
        Float_t width = fInputHistograms->GetBinError(fInputHistograms->GetBin(variableContainer, dataVector->GetId()));
        Float_t groupweight = fInputHistograms->GetGrpBinContent(fInputHistograms->GetGrpBin(variableContainer, dataVector->GetId()));
        if (fMinimumSignificantValue < average)
          dataVector->SetEqualizedWeight((1.0 + 0.1 * (dataVector->Weight() - average) / width) * groupweight);
        else
          dataVector->SetEqualizedWeight(0.0);
      }
      break;
    case QEQUAL_widthEqualization:
      for(Int_t ixData = 0; ixData < fDetectorConfiguration->GetInputDataBank()->GetEntriesFast(); ixData++){
        QnCorrectionsChannelizedDataVector *dataVector =
            static_cast<QnCorrectionsChannelizedDataVector *>(fDetectorConfiguration->GetInputDataBank()->At(ixData));
        Float_t average = fInputHistograms->GetBinContent(fInputHistograms->GetBin(variableContainer, dataVector->GetId()));
        Float_t groupweight = fInputHistograms->GetGrpBinContent(fInputHistograms->GetGrpBin(variableContainer, dataVector->GetId()));
        if (fMinimumSignificantValue < average)
          dataVector->SetEqualizedWeight((dataVector->Weight() / average) * groupweight);
        else
          dataVector->SetEqualizedWeight(0.0);
      }
      break;
    break;
    }
  }
  return kTRUE;
}

