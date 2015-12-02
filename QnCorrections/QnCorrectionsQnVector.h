#ifndef QNCORRECTIONS_QNVECTOR_H
#define QNCORRECTIONS_QNVECTOR_H
/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2015                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/
 
 
 

#include <TArrayF.h>
#include <TArrayC.h>
#include <TMath.h>
#include <TObject.h>
#include <iostream>

#include "QnCorrectionsConstants.h"

//_________________________________________________________________________
class QnCorrectionsQnVector : public TObject {

  
 public:
        
  QnCorrectionsQnVector();
  QnCorrectionsQnVector(Int_t nHarmonics);
  ~QnCorrectionsQnVector();
  QnCorrectionsQnVector(const QnCorrectionsQnVector &c);

  // setters
  void SetMaximumHarmonic(Int_t harmonic) { fQvectorX.Set(harmonic); fQvectorY.Set(harmonic); fQnVectorStatus.Set(harmonic);}
  void SetQx(Int_t harmonic, Float_t qx) { fQvectorX[harmonic-1]= qx;}
  void SetQy(Int_t harmonic, Float_t qy) { fQvectorY[harmonic-1]= qy;}
  void SetBin(Int_t bin) { fBin=bin;}
  void SetQnVectorStatus(Int_t harmonic, Int_t status)   {fQnVectorStatus[harmonic-1] |= (1<<status);}
  void UnsetQnVectorStatus(Int_t harmonic, Int_t status) {fQnVectorStatus[harmonic-1] |= (0<<status);}
  void SetQvectorNormalization(UChar_t n) {fQvectorNormalization=n;}

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
  Int_t nHarmonics() { return fQvectorX.GetSize();}
  TArrayF Qx() const { return fQvectorX;}
  TArrayF Qy() const { return fQvectorY;}
  Float_t Qx(Int_t harmonic) const { return fQvectorX[harmonic-1];}
  Float_t Qy(Int_t harmonic) const { return fQvectorY[harmonic-1];}
  Float_t Length(Int_t harmonic) const { return  TMath::Sqrt(Qx(harmonic)*Qx(harmonic)+Qy(harmonic)*Qy(harmonic));}
  Float_t QxNorm(Int_t harmonic) const { return  Qx(harmonic)/Length(harmonic);}
  Float_t QyNorm(Int_t harmonic) const { return  Qy(harmonic)/Length(harmonic);}
  Int_t Bin() const { return fBin;}
  Char_t GetQnVectorStatus(Int_t h) const {return fQnVectorStatus[h-1];} 
  TArrayC GetQnVectorStatus() const {return fQnVectorStatus;} 
  Bool_t  CheckQnVectorStatus(Int_t h, Int_t i) const;
  Float_t SumOfWeights()   const {return fSumW;}
  Int_t    N()             const {return fN;}
  Double_t EventPlane(Int_t h) const;
  UChar_t QvectorNormalization() const {return fQvectorNormalization;}



 private:
  TArrayF fQvectorX;     // Qx vector components for n harmonics
  TArrayF fQvectorY;     // Qy vector components for n harmonics
  TArrayC fQnVectorStatus;  // Bit maps for the event plane status (1 char per detector and per harmonic)
  Short_t fBin;
  UChar_t fQvectorNormalization; //  0: Q/sqrt(M)  ,  1: Q/M  , 2:  Q/|Q|,   3: Q 
  //UChar_t fQnVectorStatus[QnCorrectionsConstants::nHarmonics];  // Bit maps for the event plane status (1 char per detector and per harmonic)
  Float_t fSumW;                     // Sum of weights
  Int_t   fN;                        // Number of elements (tracks or sectors)

     
  QnCorrectionsQnVector& operator= (const QnCorrectionsQnVector &c);


  ClassDef(QnCorrectionsQnVector, 1);
};



//_______________________________________________________________________________
inline Bool_t QnCorrectionsQnVector::CheckQnVectorStatus(Int_t h, Int_t flag) const {
  //
  // Check the status of the event plane for a given detector and harmonic
  //
  //if(h<fgkEPMinHarmonics || h>fgkEPMaxHarmonics) return kFALSE;
  return (flag<(QnCorrectionsConstants::kUndefined+1) ? (fQnVectorStatus[h-1]&(1<<flag)) : kFALSE);
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
    fQvectorX[ih-1]=qvec->Qx(ih);
    fQvectorY[ih-1]=qvec->Qy(ih);
    fQnVectorStatus[ih-1]=qvec->GetQnVectorStatus(ih);
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
    fQvectorX[ih-1]=0.0;
    fQvectorY[ih-1]=0.0;
    fQnVectorStatus[ih-1]=0;
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
