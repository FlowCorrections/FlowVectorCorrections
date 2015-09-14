/************************************************************************************************* 
 * Authors:                                                                                       * 
 * Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch                                             * 
 * Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com                                            * 
 * Contributors are mentioned in the code where appropriate.                                      * 
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
 **************************************************************************************************/ 
  
/*
***********************************************************
    Event plane class that contains correction setting for specific event plane
***********************************************************
*/


#include "QnCorrectionsSteps.h"
#include "QnCorrectionsConfiguration.h"
#include "QnCorrectionsAxes.h"
#include "QnCorrectionsCuts.h"
#include <iostream>


ClassImp(QnCorrectionsConfiguration)


//_______________________________________________________________________________
QnCorrectionsConfiguration::QnCorrectionsConfiguration() :
  TObject(),
  fQnConfigurationCorrelationIndices(),
  fCuts(0x0),
  fQnNormalization(-1),
  fEqualizationMethod(-1),
  fTwistAndRescalingMethod(-1),
  fAlignmentHarmonic(-1),
  fLocalIndex(-1),
  fGlobalIndex(-1),
  fCalibrationStep(-1),
  fMinimumHarmonic(2),
  fMaximumHarmonic(2),
  fDetectorType(-1),
  fChannelList(0x0),
  fChannelGroups(0x0),
  fCalibrationDetectorNames(""),
  fEqualizationDetectorNames(""),
  fAlignmentReferenceDetector(""),
  fQnConfigurationName(""),
  fQnConfigurationCorrelationNames(),
  fCommonCorrectionAxes(0x0),
  fDataVectorEqualizationAxes(0x0),
  fRecenteringAxes(0x0),
  fAlignmentAxes(0x0),
  fTwistAndRescalingAxes(0x0),
  fChannelEqualization(kFALSE),
  fRecenterQvec(kFALSE),
  fRotateQvec(kFALSE),
  fTwistQvec(kFALSE),
  fScaleQvec(kFALSE),
  fIsTracking(kFALSE),
  fUseLabel(kTRUE),
  fRequestedCorrectionMap(),
  fApplyCorrectionMap(),
  fRequestedHistogramMap(),
  fFillHistogramMap()
  //fCorrectionPasses()
{   
  //
  // Constructor
  //


    for(Int_t ic=0; ic<2; ++ic){
      fQnConfigurationCorrelationNames[ic]="";
      fQnConfigurationCorrelationIndices[ic]=-1;
    }


  for(Int_t is=0; is<QnCorrectionsConstants::nCorrectionSteps; is++) {fRequestedCorrectionMap[is]=kFALSE;fRequestedHistogramMap[is]=kFALSE;fApplyCorrectionMap[is]=kFALSE;fFillHistogramMap[is]=kFALSE;}


}



//_______________________________________________________________________________
QnCorrectionsConfiguration::~QnCorrectionsConfiguration()
{
  //
  // De-Constructor
  //

}


//_______________________________________________________________________________
QnCorrectionsConfiguration::QnCorrectionsConfiguration(const QnCorrectionsConfiguration &c) :
  TObject(),
  fQnNormalization(c.GetQnNormalizationMethod()),
//   fEqualizationMethod(c.EqualizationMethod()),
//   fTwistAndRescalingMethod(c.TwistAndRescalingMethod()),
  fLocalIndex(c.LocalIndex()),
  fGlobalIndex(c.GlobalIndex()),
  fCalibrationStep(c.CalibrationStep()),
  fDetectorType(c.DetectorType()),
  fCalibrationDetectorNames(c.CalibrationDetectorName()),
  fEqualizationDetectorNames(c.EqualizationDetectorName()),
  fQnConfigurationName(c.QnConfigurationName()),
  //fChannelEqualization(c.doChannelEqualization()),
  //fRecenterQvec(c.doRecentering()),
  //fRotateQvec(c.doRotation()),
  //fTwistQvec(c.doTwist()),
  //fScaleQvec(c.doScaling()),
  fUseLabel(c.CorrectWithEventLabel()),
  fChannelList(c.ChannelList()),
  fChannelGroups(c.ChannelGroups()),
  fCuts(c.Cuts()),
  fDataVectorEqualizationAxes(c.EqualizationBinning()),
  fCommonCorrectionAxes(c.CalibrationBinning())
{   
  //
  // Constructor
  //
  fQnConfigurationCorrelationNames[0] = c.QnConfigurationCorrelationName(0);
  fQnConfigurationCorrelationNames[1] = c.QnConfigurationCorrelationName(1);
  fQnConfigurationCorrelationIndices[0] = c.QnConfigurationCorrelationIndex(0);
  fQnConfigurationCorrelationIndices[1] = c.QnConfigurationCorrelationIndex(1);


}


////_______________________________________________________________________________
//void QnCorrectionsConfiguration::SetCorrectionSteps(QnCorrectionsSteps::CorrectionSteps flag1, QnCorrectionsSteps::CorrectionSteps flag2, QnCorrectionsSteps::CorrectionSteps flag3, QnCorrectionsSteps::CorrectionSteps flag4, QnCorrectionsSteps::CorrectionSteps flag5, QnCorrectionsSteps::CorrectionSteps flag6){
////
//// fCorrectionMap[0] = (Int_t) QnCorrectionsSteps::kPass0;
//// fCorrectionMap[1] = (Int_t) flag1;
//// fCorrectionMap[2] = (Int_t) flag2;
//// fCorrectionMap[3] = (Int_t) flag3;
//// fCorrectionMap[4] = (Int_t) flag4;
//// fCorrectionMap[5] = (Int_t) flag5;
////
////
//// SetCorrectionFlag(flag1);
//// SetCorrectionFlag(flag2);
//// SetCorrectionFlag(flag3);
//// SetCorrectionFlag(flag4);
//// SetCorrectionFlag(flag5);
//// SetCorrectionFlag(flag6);
//
// return;
//
//}

////_______________________________________________________________________________
//void QnCorrectionsConfiguration::SetPassNumbers(){
//
//   
//  fCorrectionPasses[QnCorrectionsSteps::kPass0]            = 0 ;
//  fCorrectionPasses[QnCorrectionsSteps::kDataVectorEqualization] = 1 ;
//  fCorrectionPasses[QnCorrectionsSteps::kRecentering]      = 2 ;
//  fCorrectionPasses[QnCorrectionsSteps::kAlignment]        = 3 ;
//
//  if(fTwistAndRescalingMethod==0) {
//    fCorrectionPasses[QnCorrectionsSteps::kTwist]     = fCorrectionPasses[QnCorrectionsSteps::kRecentering];
//    fCorrectionPasses[QnCorrectionsSteps::kRescaling] = fCorrectionPasses[QnCorrectionsSteps::kRecentering];
//  }
//  else {
//    fCorrectionPasses[QnCorrectionsSteps::kTwist]     = 4 ;
//    fCorrectionPasses[QnCorrectionsSteps::kRescaling] = fCorrectionPasses[QnCorrectionsSteps::kTwist];
//  }
//   
//// Int_t passNumber=-1;
//// for(Int_t i=0; i<QnCorrectionsSteps::kNcorrectionSteps; i++){
////   
////  switch (fCorrectionMap[i]){
////    case QnCorrectionsSteps::kPass0:
////      fCorrectionPasses[i]=++passNumber;
////      break;
////    case QnCorrectionsSteps::kDataVectorEqualization:
////      fCorrectionPasses[i]=++passNumber;
////      break;
////    case QnCorrectionsSteps::kRecentering:
////      fCorrectionPasses[i]=++passNumber;
////      break;
////    case QnCorrectionsSteps::kAlignment:
////      fCorrectionPasses[i]=++passNumber;
////      break;
////    case QnCorrectionsSteps::kTwist:
////      if(fTwistAndRescalingMethod==0) fCorrectionPasses[i]=passNumber;
////      else fCorrectionPasses[i]=++passNumber;
////      break;
////    case QnCorrectionsSteps::kRescaling:
////      if(!fTwistQvec&&fTwistAndRescalingMethod!=0) fCorrectionPasses[i]=++passNumber;
////      else fCorrectionPasses[i]=passNumber;
////      break;
////  }
////
//// }
// 
//
//
//}
//
//
//
//
//
////_______________________________________________________________________________
//void QnCorrectionsConfiguration::SetCorrectionFlag(QnCorrectionsSteps::CorrectionSteps flag){
//
//  switch (flag){
//    case QnCorrectionsSteps::kDataVectorEqualization:
//      fChannelEqualization=kTRUE;
//      break;
//    case QnCorrectionsSteps::kRecentering:
//      fRecenterQvec=kTRUE;
//      break;
//    case QnCorrectionsSteps::kAlignment:
//      fRotateQvec=kTRUE;
//      break;
//    case QnCorrectionsSteps::kTwist:
//      fTwistQvec=kTRUE;
//      break;
//    case QnCorrectionsSteps::kRescaling:
//      fScaleQvec=kTRUE;
//      break;
//  }
//
//}

//_______________________________________________________________________________
Bool_t QnCorrectionsConfiguration::PassCuts(Float_t* values) {
  return fCuts->IsSelected(values);
}


