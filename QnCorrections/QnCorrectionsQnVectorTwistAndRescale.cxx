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

/// \file QnCorrectionsQnVectorTwistAndRescale.cxx
/// \brief Implementation of procedures for Qn vector twist and rescale corrections.
#include "QnCorrectionsEventClassVariablesSet.h"
#include "QnCorrectionsProfileComponents.h"
#include "QnCorrectionsProfile3DCorrelations.h"
#include "QnCorrectionsHistogramSparse.h"
#include "QnCorrectionsDetector.h"
#include "QnCorrectionsManager.h"
#include "QnCorrectionsLog.h"
#include "QnCorrectionsQnVectorTwistAndRescale.h"

const Int_t QnCorrectionsQnVectorTwistAndRescale::fDefaultMinNoOfEntries = 2;
const Double_t QnCorrectionsQnVectorTwistAndRescale::fMaxThreshold = 99999999.0;
const char *QnCorrectionsQnVectorTwistAndRescale::szTwistCorrectionName = "Twist";
const char *QnCorrectionsQnVectorTwistAndRescale::szRescaleCorrectionName = "Rescale";
const char *QnCorrectionsQnVectorTwistAndRescale::szKey = "HHHH";
const char *QnCorrectionsQnVectorTwistAndRescale::szDoubleHarmonicSupportHistogramName = "DH Qn";
const char *QnCorrectionsQnVectorTwistAndRescale::szCorrelationsSupportHistogramName = "3D QnQn";
const char *QnCorrectionsQnVectorTwistAndRescale::szTwistCorrectedQnVectorName = "twist";
const char *QnCorrectionsQnVectorTwistAndRescale::szRescaleCorrectedQnVectorName = "rescale";
const char *QnCorrectionsQnVectorTwistAndRescale::szQANotValidatedHistogramName = "TwScale NvE";


/// \cond CLASSIMP
ClassImp(QnCorrectionsQnVectorTwistAndRescale);
/// \endcond


/// Default constructor
/// Passes to the base class the identity data for the recentering and width equalization correction step
QnCorrectionsQnVectorTwistAndRescale::QnCorrectionsQnVectorTwistAndRescale() :
    QnCorrectionsCorrectionOnQvector(Form("%sAnd%s",szTwistCorrectionName,szRescaleCorrectionName), szKey),
    fBDetectorConfigurationName(),
    fCDetectorConfigurationName() {
  fDoubleHarmonicInputHistograms = NULL;
  fDoubleHarmonicCalibrationHistograms = NULL;
  fCorrelationsInputHistograms = NULL;
  fCorrelationsCalibrationHistograms = NULL;
  fQANotValidatedBin = NULL;
  fTwistAndRescaleMethod = TWRESCALE_doubleHarmonic;
  fApplyTwist = kTRUE;
  fApplyRescale = kTRUE;
  fBDetectorConfiguration = NULL;
  fCDetectorConfiguration = NULL;
  fMinNoOfEntriesToValidate = fDefaultMinNoOfEntries;
  fTwistCorrectedQnVector = NULL;
  fRescaleCorrectedQnVector = NULL;
  fPlaneQnVector = NULL;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsQnVectorTwistAndRescale::~QnCorrectionsQnVectorTwistAndRescale() {
  if (fDoubleHarmonicInputHistograms != NULL)
    delete fDoubleHarmonicInputHistograms;
  if (fDoubleHarmonicCalibrationHistograms != NULL)
    delete fDoubleHarmonicCalibrationHistograms;
  if (fCorrelationsInputHistograms != NULL)
    delete fCorrelationsInputHistograms;
  if (fCorrelationsCalibrationHistograms != NULL)
    delete fCorrelationsCalibrationHistograms;
  if (fQANotValidatedBin != NULL)
    delete fQANotValidatedBin;
  if (fTwistCorrectedQnVector != NULL)
    delete fTwistCorrectedQnVector;
  if (fRescaleCorrectedQnVector != NULL)
    delete fRescaleCorrectedQnVector;
}

/// Set the detector configurations used as reference for twist and rescaling
/// The detector configurations names are stored for further use.
/// If the step is already attached to the framework the reference detector configurations are located and stored
/// \param nameB the name of the B detector configuration
/// \param nameC the name of the C detector configuration
void QnCorrectionsQnVectorTwistAndRescale::SetReferenceConfigurationsForTwistAndRescale(const char *nameB, const char *nameC) {
  QnCorrectionsInfo(Form("Detector configurations: %s and %s, attached?: %s",
      nameB, nameC,
      ((fDetectorConfiguration != NULL) ? "yes" : "no")));

  fBDetectorConfigurationName = nameB;
  fCDetectorConfigurationName = nameC;

  /* we could be in different situations of framework attachment */
  if (fDetectorConfiguration != NULL) {
    if (fDetectorConfiguration->GetCorrectionsManager() != NULL) {
      /* the correction step is already attached to the framework */
      if (fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fBDetectorConfigurationName.Data()) != NULL) {
        fBDetectorConfiguration = fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fBDetectorConfigurationName.Data());
      }
      else {
        QnCorrectionsFatal(Form("Wrong B detector configuration %s for %s twist and rescaling correction step",
            fBDetectorConfigurationName.Data(),
            fDetectorConfiguration->GetName()));
      }
      if (fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fCDetectorConfigurationName.Data()) != NULL) {
        fCDetectorConfiguration = fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fCDetectorConfigurationName.Data());
      }
      else {
        QnCorrectionsFatal(Form("Wrong C detector configuration %s for %s twist and rescaling correction step",
            fCDetectorConfigurationName.Data(),
            fDetectorConfiguration->GetName()));
      }
      QnCorrectionsInfo(Form("Attached! B and C detector configurations for twist and rescaling: %s and %s",
          fBDetectorConfigurationName.Data(),
          fCDetectorConfigurationName.Data()));
    }
  }
}

/// Informs when the detector configuration has been attached to the framework manager
/// Basically this allows interaction between the different framework sections at configuration time
/// Locates the reference detector configurations for twist and rescaling if their names have been previously stored
void QnCorrectionsQnVectorTwistAndRescale::AttachedToFrameworkManager() {
  QnCorrectionsInfo(Form("Attached! B and C detector configurations for twist and rescaling: %s and %s",
      fBDetectorConfigurationName.Data(),
      fCDetectorConfigurationName.Data()));

  if (fBDetectorConfigurationName.Length() != 0) {
    if (fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fBDetectorConfigurationName.Data()) != NULL) {
      fBDetectorConfiguration = fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fBDetectorConfigurationName.Data());
    }
    else {
      QnCorrectionsFatal(Form("Wrong B detector configuration %s for %s twist and rescaling correction step",
          fBDetectorConfigurationName.Data(),
          fDetectorConfiguration->GetName()));
    }
  }
  if (fCDetectorConfigurationName.Length() != 0) {
    if (fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fCDetectorConfigurationName.Data()) != NULL) {
      fCDetectorConfiguration = fDetectorConfiguration->GetCorrectionsManager()->FindDetectorConfiguration(fCDetectorConfigurationName.Data());
    }
    else {
      QnCorrectionsFatal(Form("Wrong C detector configuration %s for %s twist and rescaling correction step",
          fCDetectorConfigurationName.Data(),
          fDetectorConfiguration->GetName()));
    }
  }
}

/// Asks for support data structures creation
/// Creates the corrected Qn vectors
void QnCorrectionsQnVectorTwistAndRescale::CreateSupportDataStructures() {

  Int_t nNoOfHarmonics = fDetectorConfiguration->GetNoOfHarmonics();
  Int_t *harmonicsMap = new Int_t[nNoOfHarmonics];

  fDetectorConfiguration->GetHarmonicMap(harmonicsMap);
  /* now create the corrected Qn vectors */
  fCorrectedQnVector = new QnCorrectionsQnVector(szTwistCorrectionName, nNoOfHarmonics, harmonicsMap);
  fTwistCorrectedQnVector = new QnCorrectionsQnVector(szTwistCorrectionName, nNoOfHarmonics, harmonicsMap);
  fRescaleCorrectedQnVector = new QnCorrectionsQnVector(szRescaleCorrectionName, nNoOfHarmonics, harmonicsMap);
  /* get the input vectors we need */
  fInputQnVector = fDetectorConfiguration->GetPreviousCorrectedQnVector(this);
  fPlaneQnVector = fDetectorConfiguration->GetPreviousCorrectedQnVector(NULL);
  delete [] harmonicsMap;
}

/// Asks for support histograms creation
///
/// Allocates the histogram objects and creates the calibration histograms.
///
/// Process concurrency requires Calibration Histograms creation for all
/// concurrent processes but not for Input Histograms so, we delete previously
/// allocated ones.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorTwistAndRescale::CreateSupportHistograms(TList *list) {

  TString histoDoubleHarmonicNameAndTitle = Form("%s %s ",
      szDoubleHarmonicSupportHistogramName,
      fDetectorConfiguration->GetName());

  TString histoCorrelationsNameandTitle = szCorrelationsSupportHistogramName;

  fDoubleHarmonicInputHistograms = NULL;
  fDoubleHarmonicCalibrationHistograms = NULL;
  fCorrelationsInputHistograms = NULL;
  fCorrelationsCalibrationHistograms = NULL;


  if (fDoubleHarmonicInputHistograms != NULL) delete fDoubleHarmonicInputHistograms;
  if (fCorrelationsInputHistograms != NULL) delete fCorrelationsInputHistograms;

  switch (fTwistAndRescaleMethod) {
  case TWRESCALE_doubleHarmonic:
    /* TODO: basically we are re producing half of the information already produce for recentering correction. Re use it! */
    fDoubleHarmonicInputHistograms = new QnCorrectionsProfileComponents((const char *) histoDoubleHarmonicNameAndTitle, (const char *) histoDoubleHarmonicNameAndTitle,
        fDetectorConfiguration->GetEventClassVariablesSet());
    fDoubleHarmonicInputHistograms->SetNoOfEntriesThreshold(fMinNoOfEntriesToValidate);
    fDoubleHarmonicCalibrationHistograms = new QnCorrectionsProfileComponents((const char *) histoDoubleHarmonicNameAndTitle, (const char *) histoDoubleHarmonicNameAndTitle,
        fDetectorConfiguration->GetEventClassVariablesSet());
    Int_t *harmonicsMap = new Int_t[fCorrectedQnVector->GetNoOfHarmonics()];
    fCorrectedQnVector->GetHarmonicsMap(harmonicsMap);
    fDoubleHarmonicCalibrationHistograms->CreateComponentsProfileHistograms(list, fCorrectedQnVector->GetNoOfHarmonics(), harmonicsMap);
    delete [] harmonicsMap;
    break;
  case TWRESCALE_correlations:
    fCorrelationsInputHistograms = new QnCorrectionsProfile3DCorrelations((const char *) histoDoubleHarmonicNameAndTitle, (const char *) histoDoubleHarmonicNameAndTitle,
        fDetectorConfiguration->GetName(),
        fBDetectorConfiguration->GetName(),
        fCDetectorConfiguration->GetName(),
        fDetectorConfiguration->GetEventClassVariablesSet());
    fCorrelationsInputHistograms->SetNoOfEntriesThreshold(fMinNoOfEntriesToValidate);
    fCorrelationsCalibrationHistograms = new QnCorrectionsProfile3DCorrelations((const char *) histoDoubleHarmonicNameAndTitle, (const char *) histoDoubleHarmonicNameAndTitle,
        fDetectorConfiguration->GetName(),
        fBDetectorConfiguration->GetName(),
        fCDetectorConfiguration->GetName(),
        fDetectorConfiguration->GetEventClassVariablesSet());
    Int_t *harmonicsMap = new Int_t[fCorrectedQnVector->GetNoOfHarmonics()];
    fCorrectedQnVector->GetHarmonicsMap(harmonicsMap);
    fCorrelationsCalibrationHistograms->CreateCorrelationComponentsProfileHistograms(list, fCorrectedQnVector->GetNoOfHarmonics(), 1 /* harmonic multiplier */, harmonicsMap);
    delete [] harmonicsMap;
    break;
  default:
    QnCorrectionsFatal(Form("Wrong stored twist and rescale method: %d. FIX IT, PLEASE", fTwistAndRescaleMethod));
  }
  return kTRUE;
}

/// Attaches the needed input information to the correction step
/// \param list list where the inputs should be found
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorTwistAndRescale::AttachInput(TList *list) {

  switch (fTwistAndRescaleMethod) {
  case TWRESCALE_doubleHarmonic:
    /* TODO: basically we are re producing half of the information already produce for recentering correction. Re use it! */
    if (fDoubleHarmonicInputHistograms->AttachHistograms(list)) {
      fState = QCORRSTEP_applyCollect;
      return kTRUE;
    }
    break;
  case TWRESCALE_correlations:
    if (fCorrelationsInputHistograms->AttachHistograms(list)) {
      fState = QCORRSTEP_applyCollect;
      return kTRUE;
    }
    break;
  default:
    QnCorrectionsFatal(Form("Wrong stored twist and rescale method: %d. FIX IT, PLEASE", fTwistAndRescaleMethod));
  }
  return kFALSE;
}

/// Asks for QA histograms creation
///
/// Allocates the histogram objects and creates the QA histograms.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorTwistAndRescale::CreateQAHistograms(TList *list) {

  return kTRUE;
}

/// Asks for non validated entries QA histograms creation
///
/// Allocates the histogram objects and creates the non validated entries QA histograms.
/// \param list list where the histograms should be incorporated for its persistence
/// \return kTRUE if everything went OK
Bool_t QnCorrectionsQnVectorTwistAndRescale::CreateNveQAHistograms(TList *list) {

  switch (fTwistAndRescaleMethod) {
  case TWRESCALE_doubleHarmonic:
    fQANotValidatedBin = new QnCorrectionsHistogramSparse(
        Form("%s%s %s", szQANotValidatedHistogramName, "DH", fDetectorConfiguration->GetName()),
        Form("%s%s %s", szQANotValidatedHistogramName, "DH", fDetectorConfiguration->GetName()),
        fDetectorConfiguration->GetEventClassVariablesSet());
    fQANotValidatedBin->CreateHistogram(list);
    break;
  case TWRESCALE_correlations:
    fQANotValidatedBin = new QnCorrectionsHistogramSparse(
        Form("%s%s %s", szQANotValidatedHistogramName, "CORR", fDetectorConfiguration->GetName()),
        Form("%s%s %s", szQANotValidatedHistogramName, "CORR", fDetectorConfiguration->GetName()),
        fDetectorConfiguration->GetEventClassVariablesSet());
    fQANotValidatedBin->CreateHistogram(list);
    break;
  default:
    QnCorrectionsFatal(Form("Wrong stored twist and rescale method: %d. FIX IT, PLEASE", fTwistAndRescaleMethod));
  }
  return kTRUE;
}

/// Processes the correction step
///
/// Apply the correction step
/// \return kTRUE if the correction step was applied
Bool_t QnCorrectionsQnVectorTwistAndRescale::ProcessCorrections(const Float_t *variableContainer) {
  Int_t harmonic;
  switch (fState) {
  case QCORRSTEP_calibration:
    /* collect the data needed to further produce correction parameters if Qn vectors are good enough */
    /* we have not perform any correction yet */
    return kFALSE;
    break;
  case QCORRSTEP_applyCollect:
    /* collect the data needed to further produce correction parameters if Qn vectors are good enough */
    /* and proceed to ... */
  case QCORRSTEP_apply: /* apply the correction if the current Qn vector is good enough */
    /* logging */
    switch (fTwistAndRescaleMethod) {
    case TWRESCALE_doubleHarmonic:
      /* TODO: basically we are re producing half of the information already produce for recentering correction. Re use it! */
      QnCorrectionsInfo(Form("Twist and rescale in detector %s with double harmonic method.",
          fDetectorConfiguration->GetName()));
      if (fDetectorConfiguration->GetCurrentQnVector()->IsGoodQuality()) {
        /* we cannot set them because they have different harmonic structures */
        fCorrectedQnVector->SetN(fDetectorConfiguration->GetCurrentQnVector()->GetN());
        harmonic = fCorrectedQnVector->GetFirstHarmonic();
        while (harmonic != -1) {
          fCorrectedQnVector->SetQx(harmonic,fDetectorConfiguration->GetCurrentQnVector()->Qx(harmonic));
          fCorrectedQnVector->SetQy(harmonic,fDetectorConfiguration->GetCurrentQnVector()->Qy(harmonic));
          harmonic = fCorrectedQnVector->GetNextHarmonic(harmonic);
        }
        fCorrectedQnVector->SetGood(kTRUE);
        fTwistCorrectedQnVector->Set(fCorrectedQnVector, kFALSE);
        fRescaleCorrectedQnVector->Set(fCorrectedQnVector, kFALSE);

        /* let's check the correction histograms */
        Long64_t bin = fDoubleHarmonicInputHistograms->GetBin(variableContainer);
        if (fDoubleHarmonicInputHistograms->BinContentValidated(bin)) {
          harmonic = fCorrectedQnVector->GetFirstHarmonic();
          while (harmonic != -1) {
            Double_t X2n = fDoubleHarmonicInputHistograms->GetXBinContent(harmonic,bin);
            Double_t Y2n = fDoubleHarmonicInputHistograms->GetYBinContent(harmonic,bin);

            Double_t Aplus = 1 + X2n;
            Double_t Aminus = 1 - X2n;
            Double_t LambdaPlus = Y2n / Aplus;
            Double_t LambdaMinus = Y2n / Aminus;

            if (TMath::Abs(Aplus) > fMaxThreshold) continue;
            if (TMath::Abs(Aminus) > fMaxThreshold) continue;
            if (TMath::Abs(LambdaPlus) > fMaxThreshold) continue;
            if (TMath::Abs(LambdaMinus) > fMaxThreshold) continue;

            Double_t Qx = fTwistCorrectedQnVector->Qx(harmonic);
            Double_t Qy = fTwistCorrectedQnVector->Qy(harmonic);
            Double_t newQx = (Qx - LambdaMinus * Qy)/(1 - LambdaMinus * LambdaPlus);
            Double_t newQy = (Qy - LambdaPlus * Qx)/(1 - LambdaMinus * LambdaPlus);

            if (fApplyTwist) {
              fCorrectedQnVector->SetQx(harmonic, newQx);
              fCorrectedQnVector->SetQy(harmonic, newQy);
              fTwistCorrectedQnVector->SetQx(harmonic, newQx);
              fTwistCorrectedQnVector->SetQy(harmonic, newQy);
              fRescaleCorrectedQnVector->SetQx(harmonic, newQx);
              fRescaleCorrectedQnVector->SetQy(harmonic, newQy);
            }
            newQx = newQx / Aplus;
            newQy = newQy / Aminus;
            if (fApplyRescale) {
              fCorrectedQnVector->SetQx(harmonic, newQx);
              fCorrectedQnVector->SetQy(harmonic, newQy);
              fRescaleCorrectedQnVector->SetQx(harmonic, newQx);
              fRescaleCorrectedQnVector->SetQy(harmonic, newQy);
            }
            harmonic = fCorrectedQnVector->GetNextHarmonic(harmonic);
          }
        }
        else {
          if (fQANotValidatedBin != NULL) fQANotValidatedBin->Fill(variableContainer, 1.0);
        }
      }
      else {
        /* not done! input Q vector with bad quality */
        fCorrectedQnVector->SetGood(kFALSE);
      }
      /* TODO: UPDATE THE CURRENT Qn VECTOR */
      break;
    case TWRESCALE_correlations:
      QnCorrectionsInfo(Form("Twist and rescale in detector %s with correlations with %s and %s method.",
          fDetectorConfiguration->GetName(),
          fBDetectorConfiguration->GetName(),
          fCDetectorConfiguration->GetName()));
      if (fDetectorConfiguration->GetCurrentQnVector()->IsGoodQuality()) {
        /* we cannot set them because they have different harmonic structures */
        fCorrectedQnVector->SetN(fDetectorConfiguration->GetCurrentQnVector()->GetN());
        harmonic = fCorrectedQnVector->GetFirstHarmonic();
        while (harmonic != -1) {
          fCorrectedQnVector->SetQx(harmonic,fDetectorConfiguration->GetCurrentQnVector()->Qx(harmonic));
          fCorrectedQnVector->SetQy(harmonic,fDetectorConfiguration->GetCurrentQnVector()->Qy(harmonic));
          harmonic = fCorrectedQnVector->GetNextHarmonic(harmonic);
        }
        fCorrectedQnVector->SetGood(kTRUE);
        fTwistCorrectedQnVector->Set(fCorrectedQnVector, kFALSE);
        fRescaleCorrectedQnVector->Set(fCorrectedQnVector, kFALSE);

        /* let's check the correction histograms */
        Long64_t bin = fCorrelationsInputHistograms->GetBin(variableContainer);
        if (fCorrelationsInputHistograms->BinContentValidated(bin)) {
          harmonic = fCorrectedQnVector->GetFirstHarmonic();
          while (harmonic != -1) {
            Double_t XAXC = fCorrelationsInputHistograms->GetXXBinContent("AC",harmonic,bin);
            Double_t YAYB = fCorrelationsInputHistograms->GetYYBinContent("AB",harmonic,bin);
            Double_t XAXB = fCorrelationsInputHistograms->GetXXBinContent("AB",harmonic,bin);
            Double_t XBXC = fCorrelationsInputHistograms->GetXXBinContent("BC",harmonic,bin);
            Double_t XAYB = fCorrelationsInputHistograms->GetXYBinContent("AB",harmonic,bin);
            Double_t XBYC = fCorrelationsInputHistograms->GetXYBinContent("BC",harmonic,bin);

            Double_t Aplus = TMath::Sqrt(2.0*XAXC) * XAXB / TMath::Sqrt(XAXB * XBXC + XAYB * XBYC);
            Double_t Aminus = TMath::Sqrt(2.0*XAXC) * YAYB / TMath::Sqrt(XAXB * XBXC + XAYB * XBYC);
            Double_t LambdaPlus = XAYB / XAXB;
            Double_t LambdaMinus = XAYB / YAYB;

            if (TMath::Abs(Aplus) > fMaxThreshold) continue;
            if (TMath::Abs(Aminus) > fMaxThreshold) continue;
            if (TMath::Abs(LambdaPlus) > fMaxThreshold) continue;
            if (TMath::Abs(LambdaMinus) > fMaxThreshold) continue;

            Double_t Qx = fTwistCorrectedQnVector->Qx(harmonic);
            Double_t Qy = fTwistCorrectedQnVector->Qy(harmonic);
            Double_t newQx = (Qx - LambdaMinus * Qy)/(1 - LambdaMinus * LambdaPlus);
            Double_t newQy = (Qy - LambdaPlus * Qx)/(1 - LambdaMinus * LambdaPlus);

            if (fApplyTwist) {
              fCorrectedQnVector->SetQx(harmonic, newQx);
              fCorrectedQnVector->SetQy(harmonic, newQy);
              fTwistCorrectedQnVector->SetQx(harmonic, newQx);
              fTwistCorrectedQnVector->SetQy(harmonic, newQy);
              fRescaleCorrectedQnVector->SetQx(harmonic, newQx);
              fRescaleCorrectedQnVector->SetQy(harmonic, newQy);
            }
            newQx = newQx / Aplus;
            newQy = newQy / Aminus;
            if (fApplyRescale) {
              fCorrectedQnVector->SetQx(harmonic, newQx);
              fCorrectedQnVector->SetQy(harmonic, newQy);
              fRescaleCorrectedQnVector->SetQx(harmonic, newQx);
              fRescaleCorrectedQnVector->SetQy(harmonic, newQy);
            }
            harmonic = fCorrectedQnVector->GetNextHarmonic(harmonic);
          }
        }
        else {
          if (fQANotValidatedBin != NULL) fQANotValidatedBin->Fill(variableContainer, 1.0);
        }
      }
      else {
        /* not done! input Q vector with bad quality */
        fCorrectedQnVector->SetGood(kFALSE);
      }
      /* TODO: UPDATE THE CURRENT Qn VECTOR */
      break;
    default:
      QnCorrectionsFatal(Form("Wrong stored twist and rescale method: %d. FIX IT, PLEASE", fTwistAndRescaleMethod));
    }
    /* and update the current Qn vector */
    /* TODO: THIS IS NOT VALID! DIFFERENT HARMONIC STRUCTURES
    fDetectorConfiguration->UpdateCurrentQnVector(fCorrectedQnVector); */
    break;
  }
  /* if we reached here is because we applied the correction */
  return kTRUE;
}

/// Processes the correction step data collection
///
/// Collect data for the correction step.
/// \return kTRUE if the correction step was applied
Bool_t QnCorrectionsQnVectorTwistAndRescale::ProcessDataCollection(const Float_t *variableContainer) {
  switch (fState) {
  case QCORRSTEP_calibration:
    /* logging */
    switch (fTwistAndRescaleMethod) {
    case TWRESCALE_doubleHarmonic:
      QnCorrectionsInfo(Form("Twist and rescale in detector %s with double harmonic method. Collecting data",
          fDetectorConfiguration->GetName()));
      /* remember, we only consider the harmonics in the corrected Qn vectors, i.e. the n ones for which 2n is available */
      Int_t harmonic2n = fCorrectedQnVector->GetFirstHarmonic();
      if (fPlaneQnVector->IsGoodQuality()) {
        while (harmonic2n != -1) {
          fDoubleHarmonicCalibrationHistograms->FillX(harmonic2n,variableContainer,fPlaneQnVector->Qx(harmonic2n));
          fDoubleHarmonicCalibrationHistograms->FillY(harmonic2n,variableContainer,fPlaneQnVector->Qy(harmonic2n));
          harmonic2n = fCorrectedQnVector->GetNextHarmonic(harmonic2n);
        }
      }
      break;
    case TWRESCALE_correlations:
      QnCorrectionsInfo(Form("Twist and rescale in detector %s with correlations with %s and %s method. Collecting data",
          fDetectorConfiguration->GetName(),
          fBDetectorConfiguration->GetName(),
          fCDetectorConfiguration->GetName()));
      /* collect the data needed to further produce correction parameters if both current Qn vectors are good enough */
      if ((fInputQnVector->IsGoodQuality()) &&
          (fBDetectorConfiguration->GetCurrentQnVector()->IsGoodQuality()) &&
          (fCDetectorConfiguration->GetCurrentQnVector()->IsGoodQuality())) {
        fCorrelationsCalibrationHistograms->Fill(fInputQnVector,
            fBDetectorConfiguration->GetCurrentQnVector(),
            fCDetectorConfiguration->GetCurrentQnVector(),
            variableContainer);
      }
      break;
    default:
      QnCorrectionsFatal(Form("Wrong stored twist and rescale method: %d. FIX IT, PLEASE", fTwistAndRescaleMethod));
    }
    /* we have not perform any correction yet */
    return kFALSE;
    break;
  case QCORRSTEP_applyCollect:
    /* logging */
    switch (fTwistAndRescaleMethod) {
    case TWRESCALE_doubleHarmonic:
      QnCorrectionsInfo(Form("Twist and rescale in detector %s with double harmonic method. Collecting data",
          fDetectorConfiguration->GetName()));
      /* remember, we only consider the harmonics in the corrected Qn vectors, i.e. the n ones for which 2n is available */
      Int_t harmonic2n = fCorrectedQnVector->GetFirstHarmonic();
      if (fPlaneQnVector->IsGoodQuality()) {
        while (harmonic2n != -1) {
          fDoubleHarmonicCalibrationHistograms->FillX(harmonic2n,variableContainer,fPlaneQnVector->Qx(harmonic2n));
          fDoubleHarmonicCalibrationHistograms->FillY(harmonic2n,variableContainer,fPlaneQnVector->Qy(harmonic2n));
          harmonic2n = fCorrectedQnVector->GetNextHarmonic(harmonic2n);
        }
      }
      break;
    case TWRESCALE_correlations:
      QnCorrectionsInfo(Form("Twist and rescale in detector %s with correlations with %s and %s method. Collecting data",
          fDetectorConfiguration->GetName(),
          fBDetectorConfiguration->GetName(),
          fCDetectorConfiguration->GetName()));
      /* collect the data needed to further produce correction parameters if both current Qn vectors are good enough */
      if ((fInputQnVector->IsGoodQuality()) &&
          (fBDetectorConfiguration->GetCurrentQnVector()->IsGoodQuality()) &&
          (fCDetectorConfiguration->GetCurrentQnVector()->IsGoodQuality())) {
        fCorrelationsCalibrationHistograms->Fill(fInputQnVector,
            fBDetectorConfiguration->GetCurrentQnVector(),
            fCDetectorConfiguration->GetCurrentQnVector(),
            variableContainer);
      }
      break;
    default:
      QnCorrectionsFatal(Form("Wrong stored twist and rescale method: %d. FIX IT, PLEASE", fTwistAndRescaleMethod));
    }
    /* and proceed to ... */
  case QCORRSTEP_apply: /* apply the correction if the current Qn vector is good enough */
    break;
  }
  /* if we reached here is because we applied the correction */
  return kTRUE;
}

/// Clean the correction to accept a new event
void QnCorrectionsQnVectorTwistAndRescale::ClearCorrectionStep() {

  fCorrectedQnVector->Reset();
}

/// Report on correction usage
/// Correction step should incorporate its name in calibration
/// list if it is producing information calibration in the ongoing
/// step and in the apply list if it is applying correction in
/// the ongoing step.
/// \param calibrationList list containing the correction steps producing calibration information
/// \param applyList list containing the correction steps applying corrections
/// \return kTRUE if the correction step is being applied
Bool_t QnCorrectionsQnVectorTwistAndRescale::ReportUsage(TList *calibrationList, TList *applyList) {
  switch (fState) {
  case QCORRSTEP_calibration:
    /* we are collecting */
    calibrationList->Add(new TObjString(szCorrectionName));
    /* but not applying */
    return kFALSE;
    break;
  case QCORRSTEP_applyCollect:
    /* we are collecting */
    calibrationList->Add(new TObjString(szCorrectionName));
  case QCORRSTEP_apply:
    /* and applying */
    applyList->Add(new TObjString(szCorrectionName));
    break;
  }
  return kTRUE;
}

