#ifndef QNCORRECTIONS_STEPS_H
#define QNCORRECTIONS_STEPS_H
/************************************************************************** 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>    * 
 * See cxx source for full Copyright notice                               * 
 *************************************************************************/ 
 

#include <TClonesArray.h>

class QnCorrectionsHistograms;
class QnCorrectionsConfiguration;
class QnCorrectionsQnVector;


//_________________________________________________________________________
class QnCorrectionsSteps  {
  
  

 public: 

  QnCorrectionsSteps();
  ~QnCorrectionsSteps();


  enum CorrectionSteps {
    kNothing=-1,
    kPass0,
    kDataVectorEqualization,
    kRecentering,
    kAlignment,
    kTwist,
    kRescaling,
    kNcorrectionSteps,
    kUndefined,
    kCopy
  };


 //static void BuildQnVectors(QnCorrectionsQnVector* QvectorOut, TClonesArray* dataVectorArray, Int_t QnConfIndex, Int_t minHar, Int_t maxHar, Int_t EqualizationMethod=-1);
 static void CalibrateDataVector(TClonesArray* dataVectorArray, QnCorrectionsConfiguration* QnConf,  QnCorrectionsHistograms* inputHistos, Double_t* fillValues) ;
 static void RecenterQvec(QnCorrectionsQnVector* QvectorIn, QnCorrectionsQnVector* QvectorOut, QnCorrectionsHistograms* inputHistos, Int_t bin, Int_t useStep, Int_t minHar, Int_t maxHar) ;



  //void CalibrateChannels(Bool_t useFriendChain=kFALSE);
  //void RecenterQvec(Bool_t useFriendChain=kFALSE);
  //void RotateQvec();
  //void CorrelationTwistAndRescalingQvec(Float_t* values, Int_t corpar);
  //void TwoDetectorCorrelationTwistQvec(Float_t* values, Int_t corpar);
  //void CorrelationRescalingQvec(Float_t* values, Int_t corpar);
  //void ThreeDetectorCorrelationTPCTwistQvec(Float_t* values, Int_t corpar);
  //void ThreeDetectorCorrelationTPCRescalingQvec(Float_t* values, Int_t corpar);
  //void ThreeDetectorCorrelationTPCTwistAndRescalingQvec(Float_t* values, Int_t corpar);
  //void 2nTwistQvec(Float_t* values, Int_t corpar);
  //void 2nRescalingQvec(Float_t* values, Int_t corpar);
  //void U2nTwistAndRescalingQvec(Float_t* values, Int_t corpar);
  void RescaleQnVector(Int_t corpar);
  void TwistQnVector();


 private:

  ClassDef(QnCorrectionsSteps, 1);
};



#endif
