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
  
  }


}
