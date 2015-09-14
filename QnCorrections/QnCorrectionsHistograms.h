#ifndef QNCORRECTIONS_HISTOGRAMS_H
#define QNCORRECTIONS_HISTOGRAMS_H
/************************************************************************** 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>    * 
 * See cxx source for full Copyright notice                               * 
 *************************************************************************/ 
 
 

#include "QnCorrectionsConstants.h"
#include "QnCorrectionsSteps.h"
#include "QnCorrectionsQnVector.h"
#include "QnCorrectionsAxes.h"
#include <iostream>
#include <Rtypes.h>
#include <THn.h>

class TProfile;
class TList;
//class THnF;
//class THnT<float> THnF;
class TH1F;
class TObject;
class QnCorrectionsAxes;
class QnCorrectionsCuts;
class QnCorrectionsConfiguration;


//_____________________________________________________________________
class QnCorrectionsHistograms : public TObject {


 public:
  QnCorrectionsHistograms();
  ~QnCorrectionsHistograms();
    

  // setters
  void SetGroupEqualizationHistogramM(THnF* eqHistQ, Int_t itype)  {fGroupEqualizationHistogramsM[itype]=eqHistQ;}
  void SetGroupEqualizationHistogramE(THnF* eqHistE, Int_t itype)  {fGroupEqualizationHistogramsE[itype]=eqHistE;}
  void SetEqualizationHistogramM(THnF* eqHistQ, Int_t itype)  {fEqualizationHistogramsM[itype]=eqHistQ;}
  void SetEqualizationHistogramE(THnF* eqHistE, Int_t itype)  {fEqualizationHistogramsE[itype]=eqHistE;}
  void SetCalibrationHistogramQ(Int_t ih, Int_t is, Int_t ic, THnF* eqHistQ)  { fCalibrationHistogramsQ[is][ih-1][ic]=eqHistQ;}
  void SetCalibrationHistogramE(Int_t is, THnF* eqHistE)  {  fCalibrationHistogramsE[is]=eqHistE;}
  void SetU2nHistogram(Int_t ih, Int_t ic, THnF* eqHistQ)  { fU2nHistograms[ih-1][ic]=eqHistQ;}
  void SetU2nHistogramE(THnF* eqHistE)  {  fU2nHistogramsE=eqHistE;}
  void SetRotationHistogram(Int_t cor, THnF* hist)  {  fRotationHistogram[0][cor]=hist;}
  void SetRotationHistogramE(THnF* hist) {  fRotationHistogramE[0]=hist;}


  void CreateCalibrationHistograms(QnCorrectionsConfiguration* QnConf, Int_t managerStep) {CreateMultiplicityHistograms(QnConf);CreateRotationHistograms(QnConf);CreateQvectorHistograms(QnConf);CreateCorrelationHistograms(QnConf,managerStep);}
  void CreateQvectorHistograms(QnCorrectionsConfiguration* QnConf);
  void CreateCorrelationHistograms(QnCorrectionsConfiguration* QnConf, Int_t currentManagerStep);
  void CreateCorrelationHistogramsQA(QnCorrectionsConfiguration* QnConf);
  void CreateMultiplicityHistograms(QnCorrectionsConfiguration* QnConf);
  void CreateGroupMultiplicityHistograms(TList* list, QnCorrectionsConfiguration* QnConf);
  void CreateEventHistograms(QnCorrectionsConfiguration* QnConf);
  void CreateRotationHistograms(QnCorrectionsConfiguration* QnConf);

  //void ConnectCalibrationHistograms(TList* list, QnCorrectionsConfiguration* QnConf) {ConnectMultiplicityHistograms(list,QnConf);ConnectQnCalibrationHistograms(list,QnConf);};
  Bool_t ConnectMultiplicityHistograms(TList* list, QnCorrectionsConfiguration* QnConf);
  Bool_t ConnectMeanQnCalibrationHistograms(TList* list, QnCorrectionsConfiguration* QnConf);
  Bool_t ConnectU2nQnCalibrationHistograms(TList* list, QnCorrectionsConfiguration* QnConf);
  Bool_t ConnectRotationQnCalibrationHistograms(TList* list, QnCorrectionsConfiguration* QnConf);

  void FillEventHistograms(Int_t dim, Double_t* fillValues);

  // getters
  THnF* AddHistogram( const Char_t* name, const Char_t* title, Int_t nDimensions,TAxis* binLimits);
  THnF* CreateHistogram( const Char_t* name, const Char_t* title, QnCorrectionsAxes* axes);
  THnF* DivideTHnF(THnF* t1, THnF* t2);
  TObject* GetHistogram(TList* list, const Char_t* listname, const Char_t* hname);
  THnF* GroupEqualizationHistogramM(Int_t step)  const {return (step < 3 ?  fGroupEqualizationHistogramsM[step]:  0x0);}
  THnF* GroupEqualizationHistogramE(Int_t step)  const {return (step < 3 ?  fGroupEqualizationHistogramsE[step]:  0x0);}
  THnF* EqualizationHistogramM(Int_t step)  const { return (step < 3 ?  fEqualizationHistogramsM[step]:  0x0);}
  THnF* EqualizationHistogramE(Int_t step)  const { return (step < 3 ?  fEqualizationHistogramsE[step]:  0x0);}
  THnF* CalibrationHistogramQ(Int_t step, Int_t ih, Int_t ic)  const {  return fCalibrationHistogramsQ[step][ih-1][ic];}
  THnF* CalibrationHistogramE(Int_t step)  const {return fCalibrationHistogramsE[step];}
  THnF* GetRotationHistogram(Int_t step, Int_t cor)   const {return fRotationHistogram[step][cor];}
  THnF* GetRotationHistogramE(Int_t step)  const {return fRotationHistogramE[step];}
  TH1F* EventHistogram(Int_t var)  const {return fEventHistograms[var];}

  TProfile* CorrelationProf(Int_t stage, Int_t det, Int_t har, Int_t component, Int_t axis)  const {return fCorrelationProfs[stage][det][har-1][component][axis];}
  TProfile* CorrelationEpProf(Int_t stage, Int_t det, Int_t har, Int_t component, Int_t axis)  const {return fCorrelationEpProfs[stage][det][har-1][component][axis];}
  THnF* U2nHistogram(Int_t har, Int_t component)  const {return fU2nHistograms[har-1][component];}
  THnF* U2nHistogramE()  const {return fU2nHistogramsE;}

 private:

  THnF* fGroupEqualizationHistogramsM[3];
  THnF* fGroupEqualizationHistogramsE[3];
  THnF* fEqualizationHistogramsM[3];
  THnF* fEqualizationHistogramsE[3];
  THnF* fRotationHistogram[2][4];
  THnF* fRotationHistogramE[2];
  THnF* fU2nHistogramsQA[QnCorrectionsConstants::nHarmonics][4];
  THnF* fU2nHistogramsEQA;
  THnF* fCalibrationHistogramsQ[QnCorrectionsConstants::nCorrectionSteps][QnCorrectionsConstants::nHarmonics][2];
  THnF* fCalibrationHistogramsE[QnCorrectionsConstants::nCorrectionSteps];
  TProfile* fCorrelationProfs[QnCorrectionsConstants::nCorrectionSteps][3][QnCorrectionsConstants::nHarmonics][4][QnCorrectionsConstants::nHistogramDimensions];
  TProfile* fCorrelationEpProfs[QnCorrectionsConstants::nCorrectionSteps][3][QnCorrectionsConstants::nHarmonics][4][QnCorrectionsConstants::nHistogramDimensions];
  THnF* fU2nHistograms[QnCorrectionsConstants::nHarmonics][2];
  THnF* fU2nHistogramsE;
  TH1F* fEventHistograms[QnCorrectionsConstants::nHistogramDimensions];

  TString fStages[QnCorrectionsConstants::nCorrectionSteps];


  ClassDef(QnCorrectionsHistograms, 1);
};


#endif
