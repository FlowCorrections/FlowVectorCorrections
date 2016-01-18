#include <TSystem.h>
#include <TMath.h>
#include <TROOT.h>
#include <Riostream.h>
#include <TRandom.h>
#include <TObject.h>
#include <TStopwatch.h>
#include <TFile.h>

#include "../QnCorrections/QnCorrectionsEventClasses.h"
#include "../QnCorrections/QnCorrectionsHistograms.h"

void Setup();
void Loop();
void Finish();


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

#ifdef MAKEEVENTTEXTOUTPUT
const char *sTrackEventFileName = "trackEventFile.txt";
const char *sChannelsEventFileName = "channelEventFile.txt";
Bool_t bProduceTextEventFile;
ofstream textTrackEventFile;
ofstream textChannelsEventFile;
Int_t nEventNo;
#endif


#ifdef MAKEEVENTTEXTOUTPUT
void Example(Int_t nevents, TString inputFileName, TString outputFileName, Bool_t bTextEventFile = kFALSE){
#else
  void Example(Int_t nevents, TString inputFileName, TString outputFileName){
#endif

#ifdef MAKEEVENTTEXTOUTPUT
  bProduceTextEventFile = bTextEventFile;
#endif


  Setup();

  TStopwatch stopwatch;
  stopwatch.Start();

  for(Int_t ie=0; ie<nevents; ie++) Loop();

  stopwatch.Stop();
  cout<<"Events:         "<<nevents<<endl;
  cout<<"Total time:     "<<stopwatch.RealTime()<<" s"<<endl;
  cout<<"Time per event: "<<stopwatch.RealTime()*1000./nevents<<" ms"<<endl;

  Finish();


}

void Setup(){

  //-----------------------------------------------------------
  // Variables for defining event classes for Q-vector calibration
  //
  const Int_t nEventClassesDimensions = 2;

  /*  declare object with the number of event classes dimensions */
  QnCorrectionsEventClassVariablesSet CorrEventClasses = QnCorrectionsEventClassVariablesSet(nEventClassesDimensions);

  /* now each event class variable. Here it makes use of a constructor form ab
  array of pairs, where the 1st element of each pair is the lower edge of a coarse bin,
  and the 2nd element is the number of fine bins inside the coarse bin.
  The 2nd element of the first pair is the number of coarse bins plus one
  (i.e. the total number of pairs). */
  Double_t VtxZaxes[][2] = { { -10.0, 4} , {-7.0, 1}, {7.0, 8}, {10.0, 1}};
  CorrEventClasses[0] = new QnCorrectionsEventClassVariable(kVertexZ, VarNames[kVertexZ], VtxZaxes);

  /* here it makes use of nbins, min and max constructor */
  CorrEventClasses[1] = new QnCorrectionsEventClassVariable(kCentrality, VarNames[kCentrality], 10, 0.0, 100.0);



#ifdef MAKEEVENTTEXTOUTPUT
  if (bProduceTextEventFile) {
    textTrackEventFile.open(sTrackEventFileName);
    textTrackEventFile << "eventno,centrality,vertexz,trackno,phi,charge\n";
    textTrackEventFile.close();
    textChannelsEventFile.open(sChannelsEventFileName);
    textChannelsEventFile << "eventno,centrality,vertexz,channel,phi,weight\n";
    textChannelsEventFile.close();
    nEventNo = 0;
  }
#endif


  /* lets print the variables to check everything is fine */
  /* two methods */
  /* array iterator */
  TObjArrayIter *myiter = new TObjArrayIter(&CorrEventClasses);
  QnCorrectionsEventClassVariable *nextVar = (QnCorrectionsEventClassVariable*) myiter->Next();

  while (nextVar) {
    printf("Variable id: %d\n  name: %s\n  bins: %f", nextVar->GetVariableId(), nextVar->GetVariableLabel(), nextVar->GetBinEdge(0));
    for (Int_t bin = 1; bin < nextVar->GetNBins() + 1; bin++) {
      printf(", %f", nextVar->GetBinEdge(bin));
    }
    printf("\n");
    nextVar = (QnCorrectionsEventClassVariable*) myiter->Next();
  }
  printf("\n");
  /* direct access */
  for (Int_t ixvar = 0; ixvar < CorrEventClasses.GetEntriesFast(); ixvar++) {
    nextVar = (QnCorrectionsEventClassVariable*) CorrEventClasses[ixvar];
    printf("Variable id: %d\n  name: %s\n  bins: %f", nextVar->GetVariableId(), nextVar->GetVariableLabel(), nextVar->GetBinEdge(0));
    for (Int_t bin = 1; bin < nextVar->GetNBins() + 1; bin++) {
      printf(", %f", nextVar->GetBinEdge(bin));
    }
    printf("\n");
  }
}

void Finish(){

}

void Loop(){

#ifdef MAKEEVENTTEXTOUTPUT
  if (bProduceTextEventFile) {
    textTrackEventFile.open(sTrackEventFileName,std::ofstream::out | std::ofstream::app);
    textChannelsEventFile.open(sChannelsEventFileName,std::ofstream::out | std::ofstream::app);
  }
#endif


#ifdef MAKEEVENTTEXTOUTPUT
    if (bProduceTextEventFile) {
      textChannelsEventFile << Form("%d, %.12f, %.12f, %d, %.12f, %.12f\n",
          nEventNo,
          QnMan->GetDataContainer()[kCentrality],
          QnMan->GetDataContainer()[kVertexZ],
          ich,
          phiSector[ich%8]+rotation ,
          weight);
    }
#endif


#ifdef MAKEEVENTTEXTOUTPUT
    if (bProduceTextEventFile) {
      textTrackEventFile << Form("%d, %.12f, %.12f, %d, %.12f, %d\n",
          nEventNo,
          QnMan->GetDataContainer()[kCentrality],
          QnMan->GetDataContainer()[kVertexZ],
          nTracks,
          phiTrack,
          (Int_t) QnMan->GetDataContainer()[kCharge]);
    }
#endif


#ifdef MAKEEVENTTEXTOUTPUT
  if (bProduceTextEventFile) {
    textTrackEventFile.close();
    textChannelsEventFile.close();
    nEventNo++;
  }
#endif

}

