/// \file Example.C
/// \brief The file where we test things
///
/// At the current level it incorporates the tests for the
/// event class variables, for the event class variables set
/// and for the corrections profile function.
///
/// For the last one, some indications are needed because the
/// behavior is matched towards the TProfile objects, concretely,
/// the TProfile2D. Our histograms store float quantities while
/// TProfile2D stores double quantities, and, as you might perceive
/// in the output, this makes some differences. Our profile function
/// only provides acceptable bin content when the number of bin
/// entries is higher than one, fact that you might perceive as well
/// in the output. Based on the current framework implementation
/// has been decided to keep the GetBinError providing the
/// standard deviation of the bin entries that, as can be seen in the
/// testing code, is not the default for TProfile objects.

#include <TSystem.h>
#include <TMath.h>
#include <TROOT.h>
#include <Riostream.h>
#include <TRandom.h>
#include <TObject.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TProfile2D.h>

#include "../QnCorrections/QnCorrectionsEventClasses.h"
#include "../QnCorrections/QnCorrectionsCuts.h"
#include "../QnCorrections/QnCorrectionsHistograms.h"

void Setup();
void Loop();
void Finish();

/// the detectors we will use for our example
enum Detectors{
  kDetector1,
  kDetector2,
  kDetector3
};

/// few variables to use with above detectors
enum Variables{
  kCentrality,
  kVertexZ,
  kCharge,
  kPx,
  kPy,
  kNVars
};
  
TString VarNames[kNVars] = {"Centrality", "VertexZ", "Charge", "px", "py"};

#ifdef MAKEEVENTTEXTOUTPUT
const char *sTrackEventFileName = "trackEventFile.txt";
const char *sChannelsEventFileName = "channelEventFile.txt";
Bool_t bProduceTextEventFile;
ofstream textTrackEventFile;
ofstream textChannelsEventFile;
Int_t nEventNo;
#endif

/* the prepared tests */
void TestEventClasses();
void TestProfileHistograms();

/// The actual example code
///
/// We will use it as a kind of sandbox to incrementally test
/// our functionality until its final shape.

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

  /* run the tests */
  TestEventClasses();
  TestProfileHistograms();

  /* event loop */
  for(Int_t ie=0; ie<nevents; ie++) Loop();

  stopwatch.Stop();
  cout<<"Events:         "<<nevents<<endl;
  cout<<"Total time:     "<<stopwatch.RealTime()<<" s"<<endl;
  cout<<"Time per event: "<<stopwatch.RealTime()*1000./nevents<<" ms"<<endl;

  Finish();


}

/// The routine to initialize our test framework before the events loop
void Setup(){

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


}

/// the clean up routine
void Finish(){

}


/// the events loop
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

void TestEventClasses() {
  //-----------------------------------------------------------
  // Variables for defining event classes for Q-vector calibration
  //
  cout << "\n\nEVENT CLASSES TESTS\n===================\n";
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

  /* lets print the variables to check everything is fine */
  /* two methods */
  /* array iterator */
  TObjArrayIter *myiter = new TObjArrayIter(&CorrEventClasses);
  QnCorrectionsEventClassVariable *nextVar = (QnCorrectionsEventClassVariable*) myiter->Next();

  while (nextVar) {
    printf("Variable id: %d\n  name: %s\n  bins: %f", nextVar->GetVariableId(), nextVar->GetVariableLabel(), nextVar->GetBinLowerEdge(1));
    for (Int_t bin = 1; bin < nextVar->GetNBins() + 1; bin++) {
      printf(", %f", nextVar->GetBinUpperEdge(bin));
    }
    printf("\n");
    nextVar = (QnCorrectionsEventClassVariable*) myiter->Next();
  }
  printf("\n");
  /* direct access */
  for (Int_t ixvar = 0; ixvar < CorrEventClasses.GetEntriesFast(); ixvar++) {
    printf("Variable id: %d\n  name: %s\n  bins: %f",
        CorrEventClasses.At(ixvar)->GetVariableId(),
        CorrEventClasses.At(ixvar)->GetVariableLabel(),
        CorrEventClasses.At(ixvar)->GetBinLowerEdge(1));
    for (Int_t bin = 1; bin < CorrEventClasses.At(ixvar)->GetNBins() + 1; bin++) {
      printf(", %f", CorrEventClasses.At(ixvar)->GetBinUpperEdge(bin));
    }
    printf("\n");
  }
}

void TestProfileHistograms() {
  /* let's test now the profile functions */
  cout << "\n\nPROFILE HISTOGRAMS TESTS\n========================\n";
  Int_t nBins = 40;
  Double_t min = -4.0;
  Double_t max = 4.0;
  Double_t binwidth = (max - min) / nBins;
  Double_t binmiddle[nBins]; for (Int_t bin = 0; bin < nBins; bin++) binmiddle[bin] = min + binwidth / 2 + bin * binwidth;

  /* the ROOT 2D profile */
  TProfile2D *hprof2d  = new TProfile2D("hprof2d","Profile of pz versus px and py",nBins,min,max,nBins,min,max,0,20, "s");

  Float_t px, py, pz;
  /* now let's build our event class variables */
  Int_t nDimensions = 2;
  QnCorrectionsEventClassVariablesSet evtClassSet = QnCorrectionsEventClassVariablesSet(nDimensions);
  evtClassSet[0] = new QnCorrectionsEventClassVariable(kPx,VarNames[kPx],nBins,min,max);
  evtClassSet[1] = new QnCorrectionsEventClassVariable(kPy,VarNames[kPy],nBins,min,max);

  /* the variable container */
  Float_t varContainer[kNVars];

  /* and now our histogram */
  QnCorrectionsProfile *myProfile = new QnCorrectionsProfile("QnCorrectionsProfile", "myProfile", evtClassSet);
  /* and the list we need for create it */
  TList *myList = new TList(); myList->SetOwner(kTRUE);
  myProfile->CreateProfileHistograms(myList);
  myList->Print();

  /* taken from the TProfile2D description */
  for ( Int_t i=0; i<25000; i++) {
     gRandom->Rannor(px,py);
     pz = px*px + py*py;
     hprof2d->Fill(px,py,pz,1);

     /* let's fill the variable container with the new sample */
     varContainer[kPx] = px;
     varContainer[kPy] = py;

     /* and now fill our profile */
     myProfile->Fill(varContainer, pz);
  }
  /* let's match both profile supports */
  Double_t hprofileSum = 0.0;
  Double_t myProfileSum = 0.0;
  Double_t hprofileErrSum = 0.0;
  Double_t myProfileErrSum = 0.0;
  for (Int_t binpx = 0; binpx < nBins; binpx++) {
    for (Int_t binpy = 0; binpy < nBins; binpy++) {
      /* for the profile is easy */
      Double_t hprofile2dBinContent = hprof2d->GetBinContent(hprof2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofile2dBinError =  hprof2d->GetBinError(hprof2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofile2dBinEntries = hprof2d->GetBinEntries(hprof2d->FindBin(binmiddle[binpx], binmiddle[binpy]));

      /* for us we have to fill the variable container first */
      varContainer[kPx] = binmiddle[binpx];
      varContainer[kPy] = binmiddle[binpy];
      Double_t myProfileBinContent = myProfile->GetBinContent(myProfile->GetBin(varContainer));
      Double_t myProfileBinError = myProfile->GetBinError(myProfile->GetBin(varContainer));
      /* remove comment if you want a detailed output of each of the bins */
      /* cout << Form("Profile 2D: %013.9f +/- %013.9f in: %d entries;  My profile: %013.9f +/- %013.9f\n",
          hprofile2dBinContent, hprofile2dBinError, hprofile2dBinEntries, myProfileBinContent, myProfileBinError); */
      if (hprofile2dBinEntries > 1) {
        hprofileSum += hprofile2dBinContent;
        myProfileSum += myProfileBinContent;
        hprofileErrSum += hprofile2dBinError;
        myProfileErrSum += myProfileBinError;
      }
    }
  }
  cout << Form("Profile 2D cummulated sum: %20.9f, my profile cummulated sum: %20.9f\n", hprofileSum, myProfileSum);
  cout << Form("Profile 2D error cummulated sum: %20.9f, my profile error cummulated sum: %20.9f\n", hprofileErrSum, myProfileErrSum);
  myList->Print();
  hprof2d->Print();
}
