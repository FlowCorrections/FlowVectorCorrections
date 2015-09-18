/**************************************************************************************************
 *                                                                                                *
 * Package:       FlowVectorCorrections                                                           *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch                              *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com                             *
 *                Contributors are mentioned in the code where appropriate.                       *
 * Development:   2012-2015                                                                       *
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
 
 
  
 
#include "QnCorrectionsConstants.h"
#include "QnCorrectionsCuts.h"

#include <TMath.h>
#include <TClonesArray.h>
#include <TClass.h>

ClassImp(QnCorrectionsCuts)



//_______________________________________________________________________________
QnCorrectionsCuts::QnCorrectionsCuts() :
  TObject(),
  fCuts(),
  fNcuts(0),
  fName("")
{   
  //
  // Constructor
  //

  for(Int_t i=0; i<QnCorrectionsConstants::nCuts; ++i)
    for(Int_t j=0; j<4; ++j){
      fCuts[i][j]=0.0;
    }
}




//_______________________________________________________________________________
QnCorrectionsCuts::~QnCorrectionsCuts()
{
  //
  // De-Constructor
  //
}


//_______________________________________________________________________________
void QnCorrectionsCuts::CopyCuts(QnCorrectionsCuts* cuts){
  for(Int_t i=0; i<QnCorrectionsConstants::nCuts; ++i){
    fCuts[i][0] = cuts->Type(i);
    fCuts[i][1] = cuts->Min(i);
    fCuts[i][2] = cuts->Max(i);
    fCuts[i][3] = cuts->ExcludeRange(i);
  }
  fNcuts = cuts->Ncuts();
}


