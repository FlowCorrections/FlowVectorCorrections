/************************************************************************** 
 * Copyright(c) 1998-2015, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
 
 /* 
 Contact:
 Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch 
 Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com
*/

 

#include "QnCorrectionsDataVector.h"
#include "QnCorrectionsQnVector.h"
#include <TClonesArray.h>
#include <TIterator.h>


ClassImp(QnCorrectionsDataVector)


//_______________________________________________________________________________
QnCorrectionsDataVector::QnCorrectionsDataVector() :
  TObject(),
  fPhi(0.0),
  fX(0.0),
  fY(0.0),
  fWeight(0.0),
  fEqualizedWeight(),
  fId(0),
  fEventPlaneDetectorMask(0),
  fBin(0)
{   
  //
  // Constructor
  //
  //for(Int_t idet=0; idet<QnCorrectionsConstants::nQnConfigurations; ++idet)
  //   for(Int_t imeth=0; imeth<2; ++imeth) {
  //  fEqualizedWeight[idet][imeth]=0.0;
  //}
}



//_______________________________________________________________________________
QnCorrectionsDataVector::~QnCorrectionsDataVector()
{
  //
  // De-Constructor
  //
}




//_______________________________________________________________________________
Bool_t QnCorrectionsDataVector::CheckEventPlaneDetector(Int_t det) const {
  //
  // Check the status of the event plane for a given detector and harmonic
  //
  return (det<64 ? (fEventPlaneDetectorMask&(1<<(det))) : kFALSE);
}


//_______________________________________________________________________________
void QnCorrectionsDataVector::FillQvector(TClonesArray* dataVectorArray, Int_t ep, QnCorrectionsQnVector* q, Int_t weight) {

  //TIter nextEntry(dataVectorArray);
  QnCorrectionsDataVector* dataVector=0x0;
  Float_t w=0.0;
  
  for(Int_t idata=0; idata<dataVectorArray->GetEntriesFast(); idata++){
    dataVector = static_cast<QnCorrectionsDataVector*>(dataVectorArray->At(idata));

  //while((dataVectors=static_cast<QnCorrectionsDataVector*>(nextEntry()))) {
      //if(!dataVectors) continue;
      if(!dataVector->CheckEventPlaneDetector(ep)) continue;



       weight==-1 ? w=dataVector->Weight() : w=dataVector->Weight(weight);
      
      if(w>1E-6) q->Add(dataVector->Phi(), w );
      //std::cout<<idata<<"  "<<dataVector->Id()<<"  "<<dataVector->Weight()<<"  "<<dataVector->Weight(ep,weight)<<"  "<<dataVector->Phi()<<"  "<<ep<<"  "<<weight<<"  "<<q->Qx(2)<<std::endl;
  
  }


}



//_______________________________________________________________________________
void QnCorrectionsDataVector::FillQvector(TClonesArray* dataVectorArray, QnCorrectionsQnVector* q, Int_t weight) {

  //TIter nextEntry(dataVectorArray);
  QnCorrectionsDataVector* dataVector=0x0;
  Float_t w=0.0;
  
  for(Int_t idata=0; idata<dataVectorArray->GetEntriesFast(); idata++){
    dataVector = static_cast<QnCorrectionsDataVector*>(dataVectorArray->At(idata));

  //while((dataVectors=static_cast<QnCorrectionsDataVector*>(nextEntry()))) {
      //if(!dataVectors) continue;
       weight==-1 ? w=dataVector->Weight() : w=dataVector->Weight(weight);
      
       q->Add(dataVector->Phi(), w );
      //std::cout<<idata<<"  "<<dataVector->Id()<<"  "<<dataVector->Weight()<<"  "<<dataVector->Weight(ep,weight)<<"  "<<dataVector->Phi()<<"  "<<ep<<"  "<<weight<<"  "<<q->Qx(2)<<std::endl;
  
  }


}
