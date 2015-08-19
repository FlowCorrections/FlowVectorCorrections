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

 

#include "QnCorrectionsQnVector.h"

ClassImp(QnCorrectionsQnVector)



//_______________________________________________________________________________
QnCorrectionsQnVector::QnCorrectionsQnVector() :
  //fQvecX(),
  //fQvecY(),
  fQvectorX(0x0),
  fQvectorY(0x0),
  fBin(-1),
  fSumW(0.0),
  fN(0),
  fEventPlaneStatus()
{
  //
  // Constructor
  //


  fQvectorX = TArrayF();
  fQvectorY = TArrayF();
  fEventPlaneStatus = TArrayC();
        

}


//_______________________________________________________________________________
QnCorrectionsQnVector::QnCorrectionsQnVector(Int_t nHarmonics) :
  //fQvecX(),
  //fQvecY(),
  fQvectorX(0x0),
  fQvectorY(0x0),
  fBin(-1),
  fSumW(0.0),
  fN(0),
  fEventPlaneStatus()
{
  //
  // Constructor
  //

  fQvectorX = TArrayF(nHarmonics);
  fQvectorY = TArrayF(nHarmonics);
  fEventPlaneStatus = TArrayC(nHarmonics);
        

}


//_______________________________________________________________________________
QnCorrectionsQnVector::~QnCorrectionsQnVector()
{
  //
  // De-Constructor
  //
}



//_______________________________________________________________________________
QnCorrectionsQnVector::QnCorrectionsQnVector(const QnCorrectionsQnVector &c) :
  TObject()
{
  //
  // Add a Q-vector
  //

  fSumW=c.SumOfWeights();
  fN=c.N();
  fBin=c.Bin();
  fQvectorX=TArrayF(c.Qx());
  fQvectorY=TArrayF(c.Qy());
  fEventPlaneStatus=TArrayC(c.GetEventPlaneStatus());
  //fEventPlaneStatus.Set(c.Qx().GetSize());
  ////for(Int_t ih=1; ih<=c.Qx().GetSize(); ++ih){
  //  fQvecX[ih-1]=c.Qx(ih);
  //  fQvecY[ih-1]=c.Qy(ih);
  //  fEventPlaneStatus[ih-1]=c.GetEventPlaneStatus(ih);
  //}


}

////_______________________________________________________________________________
//void QnCorrectionsQnVector::SetEventPlaneStatus(Int_t harmonic, EventPlaneStatus status) { 
//  if(harmonic>0 && harmonic<=fgkEPMaxHarmonics) 
//    fEventPlaneStatus[harmonic-fgkEPMinHarmonics] |= (1<<status);
//}
//
////_______________________________________________________________________________
//void QnCorrectionsQnVector::UnsetEventPlaneStatus(Int_t harmonic, EventPlaneStatus status) { 
//  if(harmonic>0 && harmonic<=fgkEPMaxHarmonics) 
//    fEventPlaneStatus[harmonic-fgkEPMinHarmonics] |= (0<<status);
//}


