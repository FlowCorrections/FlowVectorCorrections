#ifndef QNCORRECTIONS_QNVECTOR_H
#define QNCORRECTIONS_QNVECTOR_H
/************************************************************************** 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>    * 
 * See cxx source for full Copyright notice                               * 
 *************************************************************************/ 
 
 

#include <TArrayF.h>
#include <TArrayC.h>
#include <TMath.h>
#include <TObject.h>
#include <iostream>

//const Int_t fgkEPMinHarmonics = 1;
//const Int_t fgkEPMaxHarmonics = 6;
//const Int_t QnCorrectionsConstants::nHarmonics = 10;
//const Int_t fgkNQnCorrectionSteps=6;

//_________________________________________________________________________
class QnCorrectionsQnVector : public TObject {

  
 public:
        
  QnCorrectionsQnVector();
  QnCorrectionsQnVector(Int_t nHarmonics);
  QnCorrectionsQnVector(const QnCorrectionsQnVector &c);
  ~QnCorrectionsQnVector();

  enum EventPlaneStatus {
    kRaw=0,
    kEqualized,
    kRecentered,
    kAligned,
    kDiagonalized,
    kRescaled,
    kUndefined,
    kNMaxFlowFlags
  };

  // setters
  void SetMaximumHarmonic(Int_t harmonic) { fQvectorX.Set(harmonic); fQvectorY.Set(harmonic); fEventPlaneStatus.Set(harmonic);}
  void SetQx(Int_t harmonic, Float_t qx) { fQvectorX[harmonic-1]= qx;}
  void SetQy(Int_t harmonic, Float_t qy) { fQvectorY[harmonic-1]= qy;}
  //void SetQx(Int_t harmonic, Double_t qx) { if(harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics) fQvecX[harmonic-fgkEPMinHarmonics]=(Float_t) qx;}
  //void SetQy(Int_t harmonic, Double_t qy) { if(harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics) fQvecY[harmonic-fgkEPMinHarmonics]=(Float_t) qy;}
  void SetBin(Int_t bin) { fBin=bin;}
  void SetEventPlaneStatus(Int_t harmonic, EventPlaneStatus status)   {fEventPlaneStatus[harmonic-1] |= (1<<status);}
  void SetEventPlaneStatus(Int_t harmonic, Int_t status)   {fEventPlaneStatus[harmonic-1] |= (1<<status);}
  void UnsetEventPlaneStatus(Int_t harmonic, EventPlaneStatus status) {fEventPlaneStatus[harmonic-1] |= (0<<status);}

  //  if(harmonic>0 && harmonic<=fgkEPMaxHarmonics) 
  //    fEventPlaneStatus[harmonic-fgkEPMinHarmonics] |= (1<<status);
  //}
  //void UnsetEventPlaneStatus(Int_t harmonic, EventPlaneStatus status) { 
  //  if(harmonic>0 && harmonic<=fgkEPMaxHarmonics) 
  //    fEventPlaneStatus[harmonic-fgkEPMinHarmonics] |= (0<<status);
  //}
  void SetSumOfWeights(Float_t mult)   {fSumW = mult;}
  void SetN(Int_t mult)                   {fN = mult;}
  void Set(QnCorrectionsQnVector* qvec);
  void Add(QnCorrectionsQnVector* qvec);
  void Add(Double_t phi, Double_t w=1.);
  void Normalize();
  void SetQoverM();
  void SetQoverSquareRootOfM();
  void Reset();


  // getters
  //Float_t Qx(Int_t harmonic) const { return (harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics ? fQvecX[harmonic-fgkEPMinHarmonics] : 0.0 );}
  //Float_t Qy(Int_t harmonic) const { return (harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics ? fQvecY[harmonic-fgkEPMinHarmonics] : 0.0 );}
  //Float_t Length(Int_t harmonic) const { return (harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics ? TMath::Sqrt(Qx(harmonic)*Qx(harmonic)+Qy(harmonic)*Qy(harmonic)) : 0.0 );}
  //Float_t QxNorm(Int_t harmonic) const { return  (harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics ? Qx(harmonic)/Length(harmonic) : 0.0 );}
  //Float_t QyNorm(Int_t harmonic) const { return  (harmonic>=fgkEPMinHarmonics && harmonic<=fgkEPMaxHarmonics ? Qy(harmonic)/Length(harmonic) : 0.0 );}
  //UChar_t GetEventPlaneStatus(Int_t h) const {return (h>=fgkEPMinHarmonics && h<=fgkEPMaxHarmonics ? fEventPlaneStatus[h-fgkEPMinHarmonics] : 999);} 
  Int_t nHarmonics() { return fQvectorX.GetSize();}
  TArrayF Qx() const { return fQvectorX;}
  TArrayF Qy() const { return fQvectorY;}
  Float_t Qx(Int_t harmonic) const { return fQvectorX[harmonic-1];}
  Float_t Qy(Int_t harmonic) const { return fQvectorY[harmonic-1];}
  Float_t Length(Int_t harmonic) const { return  TMath::Sqrt(Qx(harmonic)*Qx(harmonic)+Qy(harmonic)*Qy(harmonic));}
  Float_t QxNorm(Int_t harmonic) const { return  Qx(harmonic)/Length(harmonic);}
  Float_t QyNorm(Int_t harmonic) const { return  Qy(harmonic)/Length(harmonic);}
  Int_t Bin() const { return fBin;}
  Char_t GetEventPlaneStatus(Int_t h) const {return fEventPlaneStatus[h-1];} 
  TArrayC GetEventPlaneStatus() const {return fEventPlaneStatus;} 
  Bool_t  CheckEventPlaneStatus(Int_t h, EventPlaneStatus flag) const;
  Bool_t  CheckEventPlaneStatus(Int_t h, Int_t) const;
  Float_t SumOfWeights()   const {return fSumW;}
  Int_t    N()             const {return fN;}
  Double_t EventPlane(Int_t h) const;



 private:
  //Float_t fQvecX[QnCorrectionsConstants::nHarmonics];     // Qx vector components for n harmonics
  //Float_t fQvecY[QnCorrectionsConstants::nHarmonics];     // Qy vector components for n harmonics
  TArrayF fQvectorX;     // Qx vector components for n harmonics
  TArrayF fQvectorY;     // Qy vector components for n harmonics
  TArrayC fEventPlaneStatus;  // Bit maps for the event plane status (1 char per detector and per harmonic)
  Short_t fBin;
  //UChar_t fEventPlaneStatus[QnCorrectionsConstants::nHarmonics];  // Bit maps for the event plane status (1 char per detector and per harmonic)
  Float_t fSumW;                     // Sum of weights
  Int_t   fN;                        // Number of elements (tracks or sectors)

     
  QnCorrectionsQnVector& operator= (const QnCorrectionsQnVector &c);


  ClassDef(QnCorrectionsQnVector, 1);
};




//_______________________________________________________________________________
inline Bool_t QnCorrectionsQnVector::CheckEventPlaneStatus(Int_t h, EventPlaneStatus flag) const {
  //
  // Check the status of the event plane for a given detector and harmonic
  //
  //if(h<fgkEPMinHarmonics || h>fgkEPMaxHarmonics) return kFALSE;
  return (flag<kNMaxFlowFlags ? (fEventPlaneStatus[h-1]&(1<<flag)) : kFALSE);
}



//_______________________________________________________________________________
inline Bool_t QnCorrectionsQnVector::CheckEventPlaneStatus(Int_t h, Int_t flag) const {
  //
  // Check the status of the event plane for a given detector and harmonic
  //
  //if(h<fgkEPMinHarmonics || h>fgkEPMaxHarmonics) return kFALSE;
  return (flag<kNMaxFlowFlags ? (fEventPlaneStatus[h-1]&(1<<flag)) : kFALSE);
}



//_______________________________________________________________________________
inline Double_t QnCorrectionsQnVector::EventPlane(Int_t harmonic) const
{
  //
  // Event plane for harmonic "harmonic"
  //
  //if(harmonic<1 || harmonic>fgkEPMaxHarmonics) return -999.;
  if(TMath::Abs(Qx(harmonic))<1.E-6&&TMath::Abs(Qy(harmonic))<1.E-6) return 0.;
  return TMath::ATan2(Qy(harmonic), Qx(harmonic))/Double_t(harmonic);
}

//_______________________________________________________________________________
inline void QnCorrectionsQnVector::Set(QnCorrectionsQnVector* qvec) 
{
  //
  // Add a Q-vector
  //

  fSumW=qvec->SumOfWeights();
  fN=qvec->N();
  fBin=qvec->Bin();
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    //fQvecX[ih-1]=qvec->Qx(ih);
    //fQvecY[ih-1]=qvec->Qy(ih);
    fQvectorX[ih-1]=qvec->Qx(ih);
    fQvectorY[ih-1]=qvec->Qy(ih);
    fEventPlaneStatus[ih-1]=qvec->GetEventPlaneStatus(ih);
  }


}


//_______________________________________________________________________________
inline void QnCorrectionsQnVector::Reset() 
{
  //
  // Add a Q-vector
  //

  fSumW=0.0;
  fN=0;
  fBin=-1;
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    //fQvecX[ih-1]=0.0;
    //fQvecY[ih-1]=0.0;
    fQvectorX[ih-1]=0.0;
    fQvectorY[ih-1]=0.0;
    fEventPlaneStatus[ih-1]=0;
  }
}


//_______________________________________________________________________________
inline void QnCorrectionsQnVector::Add(QnCorrectionsQnVector* qvec) 
{
  //
  // Add a Q-vector
  //

  fSumW+=qvec->SumOfWeights();
  fN+=qvec->N();
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    //fQvecX[ih-1]+=qvec->Qx(ih);
    //fQvecY[ih-1]+=qvec->Qy(ih);
    fQvectorX[ih-1]+=qvec->Qx(ih);
    fQvectorY[ih-1]+=qvec->Qy(ih);
  }


}


//_______________________________________________________________________________
inline void QnCorrectionsQnVector::Add(Double_t phi, Double_t w) 
{
  //
  // Add a Q-vector
  //

  fSumW+=w;
  fN+=1;
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    //fQvecX[ih-1]+=w*TMath::Cos(ih*phi);
    //fQvecY[ih-1]+=w*TMath::Sin(ih*phi);
    fQvectorX[ih-1]+=w*TMath::Cos(ih*phi);
    fQvectorY[ih-1]+=w*TMath::Sin(ih*phi);
  }

}


//_______________________________________________________________________________
inline void QnCorrectionsQnVector::SetQoverM() 
{
  //
  // Set Q/M
  //

  Double_t qx,qy;
  if(SumOfWeights()<10E-6) return;
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    qx = Qx(ih)/SumOfWeights();
    qy = Qy(ih)/SumOfWeights();
    SetQx(ih,qx);
    SetQy(ih,qy);
  }
}



//_______________________________________________________________________________
inline void QnCorrectionsQnVector::SetQoverSquareRootOfM() 
{
  //
  // Set Q/M
  //

  Double_t qx,qy;
  if(SumOfWeights()<10E-6) return;
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    qx = Qx(ih)/TMath::Sqrt(SumOfWeights());
    qy = Qy(ih)/TMath::Sqrt(SumOfWeights());
    SetQx(ih,qx);
    SetQy(ih,qy);
  }
}



//_______________________________________________________________________________
inline void QnCorrectionsQnVector::Normalize() 
{
  //
  // Normalize Q-vector
  //

  Double_t qx,qy;
  if(SumOfWeights()<10E-6) return;
  for(Int_t ih=1; ih<=nHarmonics(); ++ih){
    qx = QxNorm(ih);
    qy = QyNorm(ih);
    SetQx(ih,qx);
    SetQy(ih,qy);
  }
}

   
#endif
