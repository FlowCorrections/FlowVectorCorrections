#ifndef QNCORRECTIONS_MANAGER_H
#define QNCORRECTIONS_MANAGER_H
/* Copyright(c) 1998-2015, ALICE Experiment at CERN, All rights reserved. *
* See cxx source for full Copyright notice */

 

#include <TTree.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TList.h>
#include <iostream>
#include <THn.h>
#include <TProfile.h>
#include "QnCorrectionsConstants.h"
#include "QnCorrectionsConfiguration.h"
#include "QnCorrectionsDataVector.h"
#include "QnCorrectionsSteps.h"
//#include "QnCorrectionsHistograms.h"

//const Int_t QnCorrectionsConstants::nDataVectors = 10;

class QnCorrectionsHistograms;

//_________________________________________________________________________
class QnCorrectionsManager : public TObject {

 public:

  QnCorrectionsManager();
  ~QnCorrectionsManager();

  void Process();
  void Initialize();
  void Finalize();
  void SetCommonOutputFile(TFile* file)                  {SetOutputHistogramsQnCorrectionsFile(file);SetHistogramsQAFile(file);SetTreeQnVectorsFile(file);}
  void SetOutputHistogramsQnCorrectionsFile(TFile* file) {fOutputHistogramsQnCorrectionsFile=file;}
  void SetHistogramsQAFile(TFile* file)                  {fHistogramsQAFile=file;}
  void SetTreeQnVectorsFile(TFile* file)                 {fTreeQnVectorsFile=file;}
  //TODO add SetQAFile(), add fQAFile variable

  void SetCalibrationFile(TFile* inputFile)         {if(inputFile) fListInputHistogramsQnCorrections = (TList*) inputFile->Get("CalibrationHistograms");} //TODO make "CalibrationHistograms" name customizable
  void SetCalibrationFileDirectoryName(TString label) {fLabel=label;};
  void SetFillTreeQnVectors(Bool_t b=kTRUE)             {fSetFillTreeQnVectors 	          = b;}
  void SetFillHistogramsQA(Bool_t b=kTRUE)             {fSetFillHistogramsQA             = b;}
  void SetFillHistogramsQnCorrections(Bool_t b=kTRUE)  {fSetFillHistogramsQnCorrections  = b;}

  void AddQnConfiguration(QnCorrectionsConfiguration* QnConf, Int_t detectorId);

  void AddDataVector(Int_t detId, Double_t phi, Double_t weight=1., Int_t id=-1);
  void SetDataContainer(Int_t var, Float_t val) {fDataContainer[var]=val;}; //set event and track/hit variables
  
  Int_t GetNumberOfQnConfigurations()    const  {return fNumberOfQnConfigurations;}
  
  Bool_t ShouldFillTreeQnVectors()              {return fSetFillTreeQnVectors;}   
  Bool_t ShouldFillHistogramsQA()            {return fSetFillHistogramsQA;}   
  Bool_t ShouldFillHistogramsQnCorrections()  {return fSetFillHistogramsQnCorrections ;}
  
  QnCorrectionsConfiguration* GetQnConfiguration(Int_t globalIndex)  {return (QnCorrectionsConfiguration*) fQnCorrectionsConfigurations[fIndex[globalIndex][0]]->At(fIndex[globalIndex][1]);}
  QnCorrectionsConfiguration* GetQnConfiguration(TString name);
  QnCorrectionsConfiguration* GetQnConfiguration(Int_t det, Int_t localIndex) {return (QnCorrectionsConfiguration*) fQnCorrectionsConfigurations[det]->At(localIndex);}
  TClonesArray* GetQnConfigurations(Int_t det)       {return fQnCorrectionsConfigurations[det];}
  TClonesArray* GetDataVectors(Int_t det)                {return (det>-1&&det<fNumberOfDetectors ? fDataVectors[det] : 0x0);}
  TClonesArray* GetConfDataVectors(Int_t det)            {return fConfDataVectors[det];}

  TClonesArray* CorrectedQnVector(Int_t conf, Int_t step=-1)   {return (step==-1 ? fCorrectedQvectors[conf][fLastStep[conf]] : fCorrectedQvectors[conf][step]) ;}

  TTree* GetTreeQnVectors()                 {return fTreeQnVectors;}    
  TList* GetListQnVectors()                 {WriteQnVectorsToList();  return fListQnVectors;}
  TList* GetListOutputHistogramsQnCorrections()     {return fListOutputHistogramsQnCorrections;}
  TList* GetListHistogramsQA()              {return fListHistogramsQA;}

  Float_t* GetDataContainer() {return fDataContainer;}; //event and track/hit variables overwritten for each track/hit and event

  void PrintFrameworkInformation();
  void PrintFrameworkInformationLine(Bool_t HistOrCor,TString correctionname, QnCorrectionsSteps::CorrectionSteps stepflag, Int_t widthEntry, Int_t widthBar);


  //void BuildQnVectors(Bool_t useEqualizedWeights=kFALSE);
  //void CalibrateChannels();
  //void RecenterQvec();
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


 private:

  TList* GetInputListWithLabel(TString label)   {return (fListInputHistogramsQnCorrections ? (TList*) fListInputHistogramsQnCorrections->FindObject(label) : 0x0);}

  void FillHistograms();
  void FillHistogramsMeanQ(QnCorrectionsConfiguration* QnConf, Int_t step);
  void FillHistogramsMeanQ_QA(QnCorrectionsConfiguration* QnConf);
  //TODO define set of QA histograms for other steps besides Recentering
  void FillHistogramsWeights(QnCorrectionsConfiguration* QnConf);
  void FillHistogramsQnCorrelations(QnCorrectionsConfiguration* QnConf);
  void FillHistogramsQnCorrelationsQA(QnCorrectionsConfiguration* QnConf);
  void FillHistogramsU2n(QnCorrectionsConfiguration* QnConf);
  void FillHistogramsQnAlignment(QnCorrectionsConfiguration* QnConf);
  void FillHistogramsQnAlignmentQA(QnCorrectionsConfiguration* QnConf);

  void AddDetectorId(Int_t id)                         { if(fDetectorIdMap[id]==0) fDetectorIdMap[id] = ++fNdetectors;};
  void ApplyQnCorrections();
  void SetCorrectionStep(Int_t step) {fCorrectionStep=step;};
  Int_t GetCorrectionStep()          const  {return fCorrectionStep;}

  void InitializeCalibrationHistograms(); // TODO: call from Process(), need to add bool which is FALSE after the first time it is called

  void DisableFillHistograms(QnCorrectionsSteps::CorrectionSteps step);
  void DisableCorrections(QnCorrectionsSteps::CorrectionSteps step);

  //TODO Move/copy here the code from QnCorrectionsSteps and remove function QnCorrectionsSteps::BuildQnVectors
  Bool_t BuildQnVectors(QnCorrectionsConfiguration* QnConf, Bool_t useEqualizedWeights);
  void FillQnVectors();
  Bool_t CallStepEqualizeDataVectorWeights(QnCorrectionsConfiguration* QnConf);
  Bool_t CallStepRecenterQnVector(QnCorrectionsConfiguration* QnConf);
  void CallStepRescaleQnVector(Int_t corpar);
  void CallStepTwistAndRescaleQnVector(QnCorrectionsConfiguration* QnConf);
  void RotateQvec(QnCorrectionsConfiguration* QnConf);

  void ClearEvent();

  void WriteCalibrationHistogramsToList();
  void WriteQaHistogramsToList();
  void WriteQnVectorsToList();
  void WriteOutputToFile();


  TClonesArray* fDataVectors[QnCorrectionsConstants::nDataVectors];                                                            // arrays containing data 
  TClonesArray* fConfDataVectors[QnCorrectionsConstants::nQnConfigurations];                                                            // arrays containing data 
  TClonesArray* fQnCorrectionsConfigurations[QnCorrectionsConstants::nDataVectors];                                            // arrays containing QnConfigurations

  TClonesArray* fCorrectedQvectors[QnCorrectionsConstants::nQnConfigurations][QnCorrectionsConstants::nCorrectionSteps];      // arrays containing QnVectors at each correction step
  Int_t fLastStep[QnCorrectionsConstants::nQnConfigurations];                                                                     // arrays containing the id of last applied correction step

  QnCorrectionsHistograms* fInputHistograms[QnCorrectionsConstants::nQnConfigurations];                                           // container for input calibration histograms
  QnCorrectionsHistograms* fOutputHistograms[QnCorrectionsConstants::nQnConfigurations];                                          // container for output calibration and QA histograms

  typedef std::map<int, int> Map;
  Map fDetectorIdMap;

  Int_t fNumberOfQnConfigurations;
  Int_t fNumberOfDetectors;
  Int_t fNumberOfQnConfigurationsForDetector[QnCorrectionsConstants::nDataVectors];
  Int_t fNdetectors;
  //Int_t fNMasterDetectors;
  Int_t fIndex[QnCorrectionsConstants::nQnConfigurations][2];
  Int_t fCorrectionStep;
  Int_t fPassesRequired;
  Bool_t fCalibrateByRun;
  TList* fListInputHistogramsQnCorrections;          //! List of input histograms for corrections
  Float_t fDataContainer[QnCorrectionsConstants::nDataContainerVariables];
  TString fLabel; // events can be characterised with a label, corrections are applied with events from this label (for example label=<run number>)
  Bool_t fProcessedFirstEvent;

  Bool_t fSetFillTreeQnVectors;                                                                              // whether to create, fill and write Qn vectors to tree
  Bool_t fSetFillHistogramsQA;                                                                              // whether to create, fill and write QA histograms to list
  Bool_t fSetFillHistogramsQnCorrections;                                                                   // whether to create, fill and write correction histograms to list

  TList* fListQnVectors;                                                                                  // event list of Qn vectors (overwritten at each event)
  TTree* fTreeQnVectors;                                                                                  // event by event tree of corrected Qn vectors
  TList* fListOutputHistogramsQnCorrections;                                                                // output list with Qn correction histograms
  TList* fListHistogramsQA;                                                                               // output list with Qn QA histograms

  TFile* fOutputHistogramsQnCorrectionsFile;
  TFile* fHistogramsQAFile;
  TFile* fTreeQnVectorsFile;

  ClassDef(QnCorrectionsManager, 1);
  
};

#endif
