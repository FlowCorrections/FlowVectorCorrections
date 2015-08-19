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


