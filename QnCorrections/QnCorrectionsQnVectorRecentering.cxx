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
#include "QnCorrectionsQnVectorRecentering.h"

const char *QnCorrectionsQnVectorRecentering::szCorrectionName = "Recentering and width equalization";
const char *QnCorrectionsQnVectorRecentering::szKey = "CCCC";
const char *QnCorrectionsQnVectorRecentering::szSupportHistogramName = "Qn Components";


/// \cond CLASSIMP
ClassImp(QnCorrectionsQnVectorRecentering);
/// \endcond

/// Default constructor
/// Passes to the base class the identity data for the recentering and width equalization correction step
QnCorrectionsQnVectorRecentering::QnCorrectionsQnVectorRecentering() :
    QnCorrectionsCorrectionOnQvector(szCorrectionName, szKey) {
  fInputHistograms = NULL;
  fCalibrationHistograms = NULL;
  fApplyWidthEqualization = kFALSE;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsQnVectorRecentering::~QnCorrectionsQnVectorRecentering() {
  if (fInputHistograms != NULL)
    delete fInputHistograms;
  if (fCalibrationHistograms != NULL)
    delete fCalibrationHistograms;
}

/// Asks for support histograms creation
///
/// Allocates the histogram objects and creates the calibration histograms.
/// The histograms are constructed with standard deviation error calculation
/// for the proper behavior of optional gain equalization step.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorRecentering::CreateSupportHistograms(TList *list) {
  fInputHistograms = new QnCorrectionsProfileComponents(szSupportHistogramName, szSupportHistogramName,
      fDetectorConfiguration->GetEventClassVariablesSet(), "s");
  fCalibrationHistograms = new QnCorrectionsProfileComponents(szSupportHistogramName, szSupportHistogramName,
      fDetectorConfiguration->GetEventClassVariablesSet(), "s");

  /* get information about the configured harmonics to pass it for histogram creation */
  Int_t nNoOfHarmonics = fDetectorConfiguration->GetNoOfHarmonics();
  Int_t *harmonicsMap = new Int_t[nNoOfHarmonics];
  fDetectorConfiguration->GetHarmonicMap(harmonicsMap);
  fCalibrationHistograms->CreateComponentsProfileHistograms(list,nNoOfHarmonics, harmonicsMap);
  fCorrectedQnVector = new QnCorrectionsQnVector(nNoOfHarmonics, harmonicsMap);
  delete harmonicsMap;
  return kTRUE;
}

/// Attaches the needed input information to the correction step
/// \param list list where the inputs should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorRecentering::AttachInput(TList *list) {

  if (fInputHistograms->AttachHistograms(list)) {
    fState = QCORRSTEP_applyCollect;
    return kTRUE;
  }
  return kFALSE;
}

/// Asks for QA histograms creation
///
/// Allocates the histogram objects and creates the QA histograms.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorRecentering::CreateQAHistograms(TList *list) {
  return kTRUE;
}

/// Processes the correction step
///
/// Pure virtual function
/// \return kTRUE if the correction step was applied
Bool_t QnCorrectionsQnVectorRecentering::Process(const Float_t *variableContainer) {
  Int_t harmonic;
  switch (fState) {
  case QCORRSTEP_calibration:
    /* collect the data needed to further produce correction parameters */
    harmonic = fDetectorConfiguration->GetCurrentQnVector()->GetFirstHarmonic();
    while (harmonic != -1) {
      fCalibrationHistograms->FillX(harmonic,variableContainer,fDetectorConfiguration->GetCurrentQnVector()->Qx(harmonic));
      fCalibrationHistograms->FillY(harmonic,variableContainer,fDetectorConfiguration->GetCurrentQnVector()->Qy(harmonic));
      harmonic = fDetectorConfiguration->GetCurrentQnVector()->GetNextHarmonic(harmonic);
    }
    /* we have not perform any correction yet */
    return kFALSE;
    break;
  case QCORRSTEP_applyCollect:
    /* collect the data needed to further produce correction parameters */
    harmonic = fDetectorConfiguration->GetCurrentQnVector()->GetFirstHarmonic();
    while (harmonic != -1) {
      fCalibrationHistograms->FillX(harmonic,variableContainer,fDetectorConfiguration->GetCurrentQnVector()->Qx(harmonic));
      fCalibrationHistograms->FillY(harmonic,variableContainer,fDetectorConfiguration->GetCurrentQnVector()->Qy(harmonic));
      harmonic = fDetectorConfiguration->GetCurrentQnVector()->GetNextHarmonic(harmonic);
    }
    /* and proceed to ... */
  case QCORRSTEP_apply: /* apply the correction */
    harmonic = fDetectorConfiguration->GetCurrentQnVector()->GetFirstHarmonic();
    while (harmonic != -1) {
      Float_t widthX = 1.0;
      Float_t widthY = 1.0;
      if (fApplyWidthEqualization) {
        widthX = fInputHistograms->GetXBinError(harmonic, fInputHistograms->GetBin(variableContainer));
      }
      fCorrectedQnVector->SetQx(harmonic, (fDetectorConfiguration->GetCurrentQnVector()->Qx(harmonic)
          - fInputHistograms->GetXBinContent(harmonic, fInputHistograms->GetBin(variableContainer)))
          / widthX);
      fCorrectedQnVector->SetQy(harmonic, (fDetectorConfiguration->GetCurrentQnVector()->Qy(harmonic)
          - fInputHistograms->GetYBinContent(harmonic, fInputHistograms->GetBin(variableContainer)))
          / widthY);
      harmonic = fDetectorConfiguration->GetCurrentQnVector()->GetNextHarmonic(harmonic);
    }
    /* and update the current Qn vector */
    fDetectorConfiguration->UpdateCurrentQnVector(fCorrectedQnVector);
    break;
  }
  /* if we reached here is because we applied the correction */
  return kTRUE;
}

/// Clean the correction to accept a new event
void QnCorrectionsQnVectorRecentering::ClearCorrectionStep() {

  fCorrectedQnVector->Reset();
}
