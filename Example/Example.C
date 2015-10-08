#include <../QnCorrections/QnCorrectionsAxes.h>
#include <../QnCorrections/QnCorrectionsManager.h>
#include <../QnCorrections/QnCorrectionsConfiguration.h>
#include <../QnCorrections/QnCorrectionsCuts.h>
//#include <TArrayS.h>
//#include <QnCorrectionsManager.h>
#include <TSystem.h>
#include <TMath.h>
#include <TROOT.h>
#include <iostream>
#include <TRandom.h>
#include <TObject.h>
#include <TStopwatch.h>



void Setup(QnCorrectionsManager* QnMan);
void Loop(QnCorrectionsManager* QnMan);
void Finish(QnCorrectionsManager* QnMan);


enum Detectors{
  kDetector1,
  kDetector2,
  kDetector3
};

enum Variables{
  kCentrality,
  kVertexZ,
  kCharge,
  kNVars
};
  
TString VarNames[kNVars] = {"Centrality", "VertexZ", "Charge"};


void Example(Int_t nevents, TString inputFileName, TString outputFileName){

  QnCorrectionsManager* QnMan = new QnCorrectionsManager();

  TFile* inputFile = TFile::Open(inputFileName,"READ");
  TFile* outputFile = TFile::Open(outputFileName,"RECREATE");

  QnMan->SetCalibrationFile(inputFile);
  QnMan->SetCommonOutputFile(outputFile);


  Setup(QnMan);

  TStopwatch stopwatch;
  stopwatch.Start();

  for(Int_t ie=0; ie<nevents; ie++) Loop(QnMan);

  stopwatch.Stop();
  cout<<"Events:         "<<nevents<<endl;
  cout<<"Total time:     "<<stopwatch.RealTime()<<" s"<<endl;
  cout<<"Time per event: "<<stopwatch.RealTime()*1000./nevents<<" ms"<<endl;

  Finish(QnMan);


}

void Setup(QnCorrectionsManager* QnMan){

  QnMan->SetFillHistogramsQnCorrections();
  QnMan->SetFillHistogramsQA();


  //-----------------------------------------------------------
  // Axes for defining event classes for Q-vector calibration
  //
  const Int_t nCordim=2;
  QnCorrectionsAxes * Coraxes = new QnCorrectionsAxes(nCordim);  //  declare axes object with number of calibration dimensions
  TAxis CoraxesAxis[nCordim+1];

  Double_t VtxZaxes[][2] = { { -10.0, 4} , {-7.0, 1}, {7.0, 8}, {10.0, 1}};  // array of pairs, where the 1st element of each pair is the lower edge of a coarse bin, and the 2nd element is the number of fine bins inside the coarse bin. The 2nd element of the first` pair is the number of coarse bins plus one (i.e. total number of pairs).
  CoraxesAxis[0] = Coraxes->MakeAxis(VtxZaxes);

  Double_t Ctaxes[][2] = {{ 0.0, 2}, {100.0, 10 }}; 
  CoraxesAxis[1] = Coraxes->MakeAxis(Ctaxes);

  Int_t VarIdMap[nCordim] = {kVertexZ , kCentrality };

  for(Int_t idim=0; idim<nCordim; idim++) Coraxes->SetAxis(idim, VarIdMap[idim], CoraxesAxis[idim], VarNames[VarIdMap[idim]]);
  

  //-----------------------------------------------------------
  // Binning for channel equalization

  QnCorrectionsAxes * Detector2channelaxes = new QnCorrectionsAxes(nCordim+1); // +1 is for channel axis
  for(Int_t idim=0; idim<nCordim; idim++) Detector2channelaxes->SetAxis(idim, VarIdMap[idim], CoraxesAxis[idim], VarNames[VarIdMap[idim]]);
  Detector2channelaxes->SetNchannels(64);

  ////////// end of axes



  //-----------------------------------------------------------
  // Define track selections

  QnCorrectionsCuts* cutPos = new QnCorrectionsCuts();
  cutPos->AddCut(kCharge,0.5, 1.5);
  QnCorrectionsCuts* cutNeg = new QnCorrectionsCuts();
  cutNeg->AddCut(kCharge,-1.5, -0.5);


  //-----------------------------------------------------------
  // Set Q-vector configuration

  QnCorrectionsConfiguration * Detector1Pos = new QnCorrectionsConfiguration();
  Detector1Pos->SetQnNormalization(1);
  Detector1Pos->SetQnHarmonicsRange(2,2);
  Detector1Pos->SetTwistAndRescalingMethod(0);
  Detector1Pos->SetQnConfigurationName("Det1Pos");
  Detector1Pos->SetQnCorrectionsCommonAxes(Coraxes);
  Detector1Pos->SetDataVectorCuts(cutPos);

  Detector1Pos->SetQnCorrectionRecentering();
  Detector1Pos->SetQnCorrectionTwist();

  QnMan->AddQnConfiguration(Detector1Pos, kDetector1);
  

  
  QnCorrectionsConfiguration * Detector1Neg = new QnCorrectionsConfiguration();
  Detector1Neg->SetQnNormalization(1);
  Detector1Neg->SetQnHarmonicsRange(2,2);
  Detector1Neg->SetTwistAndRescalingMethod(0);
  Detector1Neg->SetQnConfigurationName("Det1Neg");
  Detector1Neg->SetQnCorrectionsCommonAxes(Coraxes);
  Detector1Neg->SetDataVectorCuts(cutNeg);

  Detector1Neg->SetQnCorrectionRecentering();
  Detector1Neg->SetQnCorrectionTwist();

  QnMan->AddQnConfiguration(Detector1Neg, kDetector1);
  

  
  //-----------------------------------------------------------
  // Channel arrays for channels of interest for Q-vector

  Short_t Detector2channels[2][64];
  for(Int_t iv0=0; iv0<2; iv0++) for(Int_t ich=0; ich<64; ich++) Detector2channels[iv0][ich] = 0;

  for(Int_t ich=32; ich<64; ich++) Detector2channels[0][ich] = 1;  // channel list: value 1 if channel should be used
  for(Int_t ich=0; ich<32; ich++) Detector2channels[1][ich] = 1;

  //-----------------------------------------------------------
  // Define channel groups for equalization of gain to average raw multiplicity in the channel group

  TArrayS* channelGroups=new TArrayS(64);
  for(Int_t ich=0; ich<64; ich++) channelGroups->SetAt((Int_t) ich/8, ich);


  //-----------------------------------------------------------
  // Set Q-vector configuration
  
  QnCorrectionsConfiguration * Detector2Aconf = new QnCorrectionsConfiguration();
  Detector2Aconf->SetQnNormalization(1);
  Detector2Aconf->SetQnHarmonicsRange(2,2);
  Detector2Aconf->SetDataVectorEqualizationMethod(1);
  Detector2Aconf->SetHarmonicForAlignment(2);
  Detector2Aconf->SetDataVectorIdList(new TArrayS(64, Detector2channels[1]));
  Detector2Aconf->SetDataVectorEqualizationGroups(channelGroups);
  Detector2Aconf->SetQnConfigurationName("Det2A");
  Detector2Aconf->SetReferenceQnForTwistAndRescaling("Det1Pos","Det2C");
  Detector2Aconf->SetReferenceQnForAlignment("Det1Pos");
  Detector2Aconf->SetQnCorrectionsCommonAxes(Coraxes);
  Detector2Aconf->SetDataVectorEqualizationAxes(Detector2channelaxes);

  Detector2Aconf->SetQnCorrectionDataVectorEqualization();
  Detector2Aconf->SetQnCorrectionRecentering();
  Detector2Aconf->SetQnCorrectionAlignment();


  QnMan->AddQnConfiguration(Detector2Aconf, kDetector2);
  

  

  QnCorrectionsConfiguration * Detector2Cconf = new QnCorrectionsConfiguration();
  Detector2Cconf->SetQnNormalization(1);
  Detector2Cconf->SetQnHarmonicsRange(2,2);
  Detector2Cconf->SetDataVectorEqualizationMethod(1);
  Detector2Cconf->SetHarmonicForAlignment(2);
  Detector2Cconf->SetDataVectorIdList(new TArrayS(64, Detector2channels[0]));
  Detector2Cconf->SetDataVectorEqualizationGroups(channelGroups);
  Detector2Cconf->SetQnConfigurationName("Det2C");
  Detector2Cconf->SetReferenceQnForTwistAndRescaling("Det1Pos","Det2A");
  Detector2Cconf->SetReferenceQnForAlignment("Det1Pos");
  Detector2Cconf->SetQnCorrectionsCommonAxes(Coraxes);
  Detector2Cconf->SetDataVectorEqualizationAxes(Detector2channelaxes);

  Detector2Cconf->SetQnCorrectionDataVectorEqualization();
  Detector2Cconf->SetQnCorrectionRecentering();
  Detector2Cconf->SetQnCorrectionAlignment();


  QnMan->AddQnConfiguration(Detector2Cconf, kDetector2);

  QnMan->SetCalibrationFileDirectoryName("Example");


  
}

void Finish(QnCorrectionsManager* QnMan){

  QnMan->Finalize();

}


void Loop(QnCorrectionsManager* QnMan){

  QnMan->Clear();

  // Set event data
  QnMan->GetDataContainer()[kCentrality]=gRandom->Rndm()*100;
  QnMan->GetDataContainer()[kVertexZ]=(gRandom->Rndm()-0.5)*20;


  // Fill azimuthal angle and weights into datavector objects
  
  Float_t dphi = 2*TMath::Pi()/8;
  const Double_t phiSector[8] = { dphi*0,dphi*1,dphi*2,dphi*3,dphi*4,dphi*5,dphi*6,dphi*7}; 
                           

  Double_t flowV2 = 0.5;
  Double_t rotation = -0.3;
  Double_t PsiRP = gRandom->Rndm()*2*TMath::Pi();

      Double_t weight=0.;


    for(Int_t ich=0; ich<64; ich++){

      weight=gRandom->Rndm()*((200.+ich)/200.)*(100-QnMan->GetDataContainer()[kCentrality])*(1+flowV2*TMath::Cos(2*(phiSector[ich%8]-PsiRP))); // weight contains flow and event multiplicity dependent channel signal, and non-uniform acceptance

      QnMan->AddDataVector(kDetector2, phiSector[ich%8]+rotation , weight, ich);   

    }



    Double_t Mult=2+gRandom->Rndm()*(100-QnMan->GetDataContainer()[kCentrality])*100;
    Int_t nTracks=0;
    while(nTracks<Mult){
      Double_t phiTrack = gRandom->Rndm()*2*TMath::Pi();

      if(gRandom->Rndm()>(1-flowV2+flowV2*TMath::Cos(2*(phiTrack-PsiRP)))) continue;

      if(phiTrack>0&&phiTrack<0.5) if(gRandom->Rndm()<0.5) continue;

      if(gRandom->Rndm()<0.4) QnMan->SetDataContainer(kCharge, 1);   // Fill relevant track information into data container, if track cuts have to be applied
      else QnMan->SetDataContainer(kCharge, -1);

      QnMan->AddDataVector(kDetector1,phiTrack);   
      nTracks++;

   }


  QnMan->Process();


}

