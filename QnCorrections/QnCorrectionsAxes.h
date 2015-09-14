#ifndef QNCORRECTIONS_AXES_H
#define QNCORRECTIONS_AXES_H
/************************************************************************** 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>    * 
 * See cxx source for full Copyright notice                               * 
 *************************************************************************/ 
 

//#include <TClonesArray.h>
//#include <TBits.h>
//#include <TMath.h>
//#include <TList.h>
//#include <iostream>
//#include <Rtypes.h>
//#include <TArrayS.h>
//#include <THn.h>
//#include <TVector.h>
#include "QnCorrectionsConstants.h"
#include <TObject.h>
#include <TArrayI.h>
#include <TArrayD.h>
#include <TAxis.h>
//#include <TProfile.h>

//const Int_t fgkEPMaxHarmonics = 6;
//const Int_t QnCorrectionsConstants::nQnConfigurations = 20;

//const Int_t QnCorrectionsConstants::nHistogramDimensions=10;

//_________________________________________________________________________
class QnCorrectionsAxes : public TObject {

 public:
  //QnCorrectionsAxes();
  QnCorrectionsAxes(Int_t dim=1);
  QnCorrectionsAxes(const QnCorrectionsAxes &c);
  ~QnCorrectionsAxes();

  // setters
  void SetDim(Int_t dim)  {fDim=dim;}
  void SetVar(Int_t dim, Int_t* var) {for(Int_t i=0; i<dim; i++) fVar[i]=var[i];}
  void SetAxis(Int_t axis, Int_t var, Double_t binArray[][2], TString label);
  void SetAxis(Int_t axis, Int_t var, Int_t nwidths, Int_t * nbins, Double_t * edges, TString label="");
  void SetAxis(Int_t axis, Int_t var, TAxis ax, TString label="");
  void SetAxisLabel(Int_t axis, TString label) {fAxesLabels[axis]=label;}
  void SetNchannels(Int_t nchan);

  // getters
  Int_t Dim()  const {return fDim;}
  const Int_t* Var() const  {return fVar;}
  Int_t Var(Int_t var) const  {return fVar[var];}
  Int_t  Nbins(Int_t ax) const  {return fAxes[ax].GetNbins();}
  TAxis* Axes() {return fAxes;}
  TAxis Axis(Int_t ax) const {return fAxes[ax];}
  const Double_t* Bins(Int_t ax)const  {return fAxes[ax].GetXbins()->GetArray();}
  TString AxisLabel(Int_t ax) const {return fAxesLabels[ax];}

  Double_t GetLowEdge(Int_t ax) const {return Bins(ax)[0];}
  Double_t GetUpEdge(Int_t ax) const {return Bins(ax)[Nbins(ax)];}
  TAxis MakeAxis(Double_t binArray[][2]);

 private:


  Int_t  fDim;
  Int_t fVar[QnCorrectionsConstants::nHistogramDimensions];
  TAxis fAxes[QnCorrectionsConstants::nHistogramDimensions];
  TString fAxesLabels[QnCorrectionsConstants::nHistogramDimensions];

  ClassDef(QnCorrectionsAxes, 1);
};

#endif
