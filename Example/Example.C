/// \file Example.C
/// \brief The file where we test things
///
/// At the current level it incorporates the tests for:
/// * event classes variables
/// * event class variables set
/// * profile function support
/// * components profile function support
/// * correlation components profile function support
/// * cuts function support
/// * logging function support (implicitly via the others)
///
/// For the profile functions, some indications are needed because the
/// behavior is matched towards TProfile objects, concretely,
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

/* to exclude */
#include <TProfile2D.h>
#include <TClonesArray.h>
/* end to exclude */

#include "../QnCorrections/QnCorrectionsEventClasses.h"
#include "../QnCorrections/QnCorrectionsCuts.h"
#include "../QnCorrections/QnCorrectionsHistograms.h"
#include "../QnCorrections/QnCorrectionsDataVector.h"
#include "../QnCorrections/QnCorrectionsQnVector.h"
#include "../QnCorrections/QnCorrectionsDetector.h"
#include "../QnCorrections/QnCorrectionsManager.h"
#include "../QnCorrections/QnCorrectionsInputGainEqualization.h"

void Setup(QnCorrectionsManager* QnMan);
void Loop(QnCorrectionsManager* QnMan);
void Finish(QnCorrectionsManager* QnMan);

/// the detectors we will use for our example
enum Detectors{
  kDetector1 = 0,
  kDetector2,
  kDetector3,
  kNDetectors
};

/// few variables to use with above detectors
enum Variables{
  kCentrality = 0,
  kVertexZ,
  kCharge,
  kPx,
  kPy,
  kNVars
};

/// the variable names
TString VarNames[kNVars] = {"Centrality", "VertexZ", "Charge", "px", "py"};
/// the detector names
TString DetectorNames[kNDetectors] = {"Detector one", "Detector two", "Detector three"};

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
void TestComponentsHistograms();
void TestCorrelationComponentsHistograms();
void TestCuts();
void TestDataVectorsAndQnVectors(Int_t nEvents = 20);


/* Characteristics of the channelized detector */
/// Channelized detector number of channels
Int_t nDetectorTwoNoOfChannels = 64;
/// Channelized detector number of sectors
Int_t nDetectorTwoNoOfSectors = 8;
/// Channelized detector lowest channel within the second sub-detector
Int_t nDetectorTwoLowestDetectorTwoCChannel = 32;

/// The actual example code
///
/// We will use it as a kind of sandbox to incrementally test
/// our functionality until its final shape.

#ifdef MAKEEVENTTEXTOUTPUT
void Example(Int_t nevents, TString inputFileName, TString outputFileName, Bool_t bTextEventFile = kFALSE){
#else
void Example(Int_t nevents, TString inputFileName, TString outputFileName){
#endif

  QnCorrectionsManager* QnMan = new QnCorrectionsManager();

#ifdef MAKEEVENTTEXTOUTPUT
  bProduceTextEventFile = bTextEventFile;
#endif


  Setup(QnMan);

  TStopwatch stopwatch;
  stopwatch.Start();

  /* run the tests */
  /* TestEventClasses();
  TestProfileHistograms();
  TestComponentsHistograms();
  TestCorrelationComponentsHistograms();
  TestCuts();
  TestDataVectorsAndQnVectors(2); */

  /* event loop */
  for(Int_t ie=0; ie<nevents; ie++) Loop(QnMan);

  stopwatch.Stop();
  cout<<"Events:         "<<nevents<<endl;
  cout<<"Total time:     "<<stopwatch.RealTime()<<" s"<<endl;
  cout<<"Time per event: "<<stopwatch.RealTime()*1000./nevents<<" ms"<<endl;

  Finish(QnMan);


}

/// The routine to initialize our test framework before the events loop
void Setup(QnCorrectionsManager* QnMan){

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

  /* initialize the corrections framework */
  QnMan->InitializeQnCorrectionsFramework();

  /* our event classes variables: vertexZ and centrality */
  const Int_t nEventClassesDimensions = 2;
  QnCorrectionsEventClassVariablesSet *CorrEventClasses = new QnCorrectionsEventClassVariablesSet(nEventClassesDimensions);
  Double_t VtxZaxes[][2] = { { -10.0, 4} , {-7.0, 1}, {7.0, 8}, {10.0, 1}};
  CorrEventClasses->Add(new QnCorrectionsEventClassVariable(kVertexZ, VarNames[kVertexZ], VtxZaxes));
  CorrEventClasses->Add(new QnCorrectionsEventClassVariable(kCentrality, VarNames[kCentrality], 10, 0.0, 100.0));

  /* the harmonics the analysis will run on */
  Int_t nNoOfHarmonics = 1;
  Int_t harmonicsMap[] = {2};

  /* our cuts to differentiate positive from negative tracks */
  QnCorrectionsCutsSet *myPositiveCuts = new QnCorrectionsCutsSet();
  myPositiveCuts->Add(new QnCorrectionsCutAbove(kCharge, 0.0));
  /* let's own the cuts due to how they are created */
  myPositiveCuts->SetOwner(kTRUE);

  QnCorrectionsCutsSet *myNegativeCuts = new QnCorrectionsCutsSet();
  myNegativeCuts->Add(new QnCorrectionsCutBelow(kCharge, 0.0));
  /* let's own the cuts due to how they are created */
  myNegativeCuts->SetOwner(kTRUE);

  /* let's create our detectors */
  QnCorrectionsDetector *myDetectorOne = new QnCorrectionsDetector(DetectorNames[kDetector1], kDetector1);
  QnCorrectionsDetector *myDetectorTwo = new QnCorrectionsDetector(DetectorNames[kDetector2], kDetector2);

  /* and our detector configurations for the track detector */
  QnCorrectionsTrackDetectorConfiguration *myDetectorOnePositive =
      new QnCorrectionsTrackDetectorConfiguration(
          "Det1pos",
          myDetectorOne,
          CorrEventClasses,
          nNoOfHarmonics,
          harmonicsMap);
  myDetectorOnePositive->SetCuts(myPositiveCuts);
  myDetectorOnePositive->SetQVectorCalibrationMethod(QCALIB_QoverM);

  QnCorrectionsTrackDetectorConfiguration *myDetectorOneNegative =
      new QnCorrectionsTrackDetectorConfiguration(
          "Det1neg",
          myDetectorOne,
          CorrEventClasses,
          nNoOfHarmonics,
          harmonicsMap);
  myDetectorOneNegative->SetCuts(myNegativeCuts);
  myDetectorOneNegative->SetQVectorCalibrationMethod(QCALIB_QoverM);

  /* add the configurations to the detector */
  myDetectorOne->AddDetectorConfiguration(myDetectorOnePositive);
  myDetectorOne->AddDetectorConfiguration(myDetectorOneNegative);

  /* now add the detector to the framework */
  QnMan->AddDetector(myDetectorOne);

  /* and our detector configurations for the channels detector */
  Bool_t *bUsedChannelDetectorTwoC = new Bool_t[nDetectorTwoNoOfChannels];
  Bool_t *bUsedChannelDetectorTwoA = new Bool_t[nDetectorTwoNoOfChannels];
  Int_t *nChannelGroupDetectorTwoC = new Int_t[nDetectorTwoNoOfChannels];
  Int_t *nChannelGroupDetectorTwoA = new Int_t[nDetectorTwoNoOfChannels];

  for(Int_t ixChannel = 0; ixChannel < nDetectorTwoNoOfChannels; ixChannel++) {
    if (ixChannel < nDetectorTwoLowestDetectorTwoCChannel) {
      /* we are in sub-detector A */
      bUsedChannelDetectorTwoC[ixChannel] = kFALSE;
      bUsedChannelDetectorTwoA[ixChannel] = kTRUE;
    }
    else {
      /* we are in sub-detector C */
      bUsedChannelDetectorTwoC[ixChannel] = kTRUE;
      bUsedChannelDetectorTwoA[ixChannel] = kFALSE;
    }
    /* and channel groups */
    nChannelGroupDetectorTwoC[ixChannel] = Int_t(ixChannel / 8);
    nChannelGroupDetectorTwoA[ixChannel] = Int_t(ixChannel / 8);
  }

  QnCorrectionsChannelDetectorConfiguration *myDetectorTwoA =
      new QnCorrectionsChannelDetectorConfiguration(
          "Det2A",
          myDetectorTwo,
          CorrEventClasses,
          nDetectorTwoNoOfChannels,
          nNoOfHarmonics,
          harmonicsMap);
  myDetectorTwoA->SetChannelsScheme(bUsedChannelDetectorTwoA, nChannelGroupDetectorTwoA);

  /* let's configure the Q vector calibration */
  myDetectorTwoA->SetQVectorCalibrationMethod(QCALIB_QoverM);
  /* lets configure the equalization of input data */
  QnCorrectionsInputGainEqualization *eqA = new QnCorrectionsInputGainEqualization();
  eqA->SetEqualizationMethod(QEQUAL_widthEqualization);
  myDetectorTwoA->AddCorrectionOnInputData(eqA);

  QnCorrectionsChannelDetectorConfiguration *myDetectorTwoC =
      new QnCorrectionsChannelDetectorConfiguration(
          "Det2C",
          myDetectorTwo,
          CorrEventClasses,
          nDetectorTwoNoOfChannels,
          nNoOfHarmonics,
          harmonicsMap);
  myDetectorTwoC->SetChannelsScheme(bUsedChannelDetectorTwoC, nChannelGroupDetectorTwoC);

  /* let's configure the Q vector calibration */
  myDetectorTwoC->SetQVectorCalibrationMethod(QCALIB_QoverM);
  /* lets configure the equalization of input data */
  QnCorrectionsInputGainEqualization *eqC = new QnCorrectionsInputGainEqualization();
  eqC->SetEqualizationMethod(QEQUAL_widthEqualization);
  myDetectorTwoC->AddCorrectionOnInputData(eqC);

  /* add the configurations to the detector */
  myDetectorTwo->AddDetectorConfiguration(myDetectorTwoA);
  myDetectorTwo->AddDetectorConfiguration(myDetectorTwoC);

  /* finally add the detector to the framework */
  QnMan->AddDetector(myDetectorTwo);

  /* here we should be able to store produced data vectors */
}

/// the final output and clean up routine
void Finish(QnCorrectionsManager* QnMan){
  /* Make the final output of the correction framework */
  QnMan->FinalizeQnCorrectionsFramework();
}


/// the events loop
void Loop(QnCorrectionsManager* QnMan){

#ifdef MAKEEVENTTEXTOUTPUT
  if (bProduceTextEventFile) {
    textTrackEventFile.open(sTrackEventFileName,std::ofstream::out | std::ofstream::app);
    textChannelsEventFile.open(sChannelsEventFileName,std::ofstream::out | std::ofstream::app);
  }
#endif

  // Set event data
  QnMan->GetDataContainer()[kCentrality] = gRandom->Rndm() * 100;
  QnMan->GetDataContainer()[kVertexZ] = (gRandom->Rndm() - 0.5) * 20;

  // azimuthal angle and weights to fill into data vector objects
  Float_t dphi = 2 * TMath::Pi() / nDetectorTwoNoOfSectors;
  Double_t phiSector[nDetectorTwoNoOfSectors];
  for (Int_t ixSector = 0; ixSector < nDetectorTwoNoOfSectors; ixSector++) {
    phiSector[ixSector] = ixSector * dphi;
  }

  Double_t flowV2 = 0.5;
  Double_t rotation = -0.3;
  Double_t PsiRP = gRandom->Rndm() * 2*TMath::Pi();

  Double_t weight=0.;

  for(Int_t ixChannel = 0; ixChannel < nDetectorTwoNoOfChannels; ixChannel++){
    // weight contains flow and event multiplicity dependent channel signal, and non-uniform acceptance
    weight = gRandom->Rndm()
        * ((200. + ixChannel) / 200.)
        * (100 - QnMan->GetDataContainer()[kCentrality])
        * (1 + flowV2 * TMath::Cos(2 * (phiSector[ixChannel % nDetectorTwoNoOfSectors] - PsiRP)));

    QnMan->AddDataVector(kDetector2, phiSector[ixChannel % nDetectorTwoNoOfSectors] + rotation, weight, ixChannel);

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
  }


  Double_t multiplicity = 2 + gRandom->Rndm() * (100 - QnMan->GetDataContainer()[kCentrality]) * 100;
  Int_t nTracks = 0;

  while(nTracks < multiplicity){
    Double_t trackPhi = gRandom->Rndm() * 2*TMath::Pi();

    if (gRandom->Rndm() > (1 - flowV2 + flowV2 * TMath::Cos(2 * (trackPhi - PsiRP)))) continue;

    if ((trackPhi > 0) && (trackPhi < 0.5))
      if (gRandom->Rndm() < 0.5) continue;

    // Fill relevant track information into data container, if track cuts have to be applied
    if (gRandom->Rndm() < 0.4)
      QnMan->GetDataContainer()[kCharge] = 1;
    else
      QnMan->GetDataContainer()[kCharge] = -1;

    QnMan->AddDataVector(kDetector1, trackPhi);


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
    nTracks++;
  }

#ifdef MAKEEVENTTEXTOUTPUT
  if (bProduceTextEventFile) {
    textTrackEventFile.close();
    textChannelsEventFile.close();
    nEventNo++;
  }
#endif

  QnMan->ProcessEvent();
}

/// Test fot the event classes variables and set
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
  delete CorrEventClasses[0];
  delete CorrEventClasses[1];
}

/// Test for the profile function support
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
  delete myProfile;
  delete myList;
  delete hprof2d;
}

/// Test for the components profile function support
void TestComponentsHistograms() {
  /* let's test now the components profile functions */
  cout << "\n\nCOMPONENTS PROFILE HISTOGRAMS TESTS\n===================================\n";
  Int_t nBins = 40;
  Double_t min = -4.0;
  Double_t max = 4.0;
  Double_t binwidth = (max - min) / nBins;
  Double_t binmiddle[nBins]; for (Int_t bin = 0; bin < nBins; bin++) binmiddle[bin] = min + binwidth / 2 + bin * binwidth;

  /* the ROOT 2D profiles */
  TProfile2D *hprofX2d  = new TProfile2D("hprofX2d","Profile of pz*|cos 2#phi| versus px and py",nBins,min,max,nBins,min,max,0,20, "s");
  TProfile2D *hprofY2d  = new TProfile2D("hprofY2d","Profile of pz*|sin 2#phi| versus px and py",nBins,min,max,nBins,min,max,0,20, "s");

  Float_t px, py, pz;
  /* now let's build our event class variables */
  Int_t nDimensions = 2;
  QnCorrectionsEventClassVariablesSet evtClassSet = QnCorrectionsEventClassVariablesSet(nDimensions);
  evtClassSet[0] = new QnCorrectionsEventClassVariable(kPx,VarNames[kPx],nBins,min,max);
  evtClassSet[1] = new QnCorrectionsEventClassVariable(kPy,VarNames[kPy],nBins,min,max);

  /* the variable container */
  Float_t varContainer[kNVars];

  /* and now our histogram */
  Int_t nNoOfHarmonics = 1;
  Int_t harmonicsMap[] = {2};
  Int_t myHarmonic = 2;
  QnCorrectionsComponentsProfile *myProfile = new QnCorrectionsComponentsProfile("QnCorrectionsComponentsProfile", "myComponentsProfile", evtClassSet);
  /* and the list we need for create it */
  TList *myList = new TList(); myList->SetOwner(kTRUE);
  myProfile->CreateComponentsProfileHistograms(myList,nNoOfHarmonics,harmonicsMap);
  myList->Print();

  /* taken from the TProfile2D description */
  for ( Int_t i=0; i<25000; i++) {
     gRandom->Rannor(px,py);
     pz = px*px + py*py;
     hprofX2d->Fill(px,py,pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px)))),1);
     hprofY2d->Fill(px,py,pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px)))),1);

     /* let's fill the variable container with the new sample */
     varContainer[kPx] = px;
     varContainer[kPy] = py;

     /* and now fill our profile */
     /* test we cannot access the Fill base method -> get error */
     /* comment this out after the test */
     /* myProfile->Fill(varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))))); */
     /* test we get error when accessing not allocated harmonic */
     /* comment this out after the test */
     /* myProfile->FillX(1, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))))); */
     myProfile->FillX(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px)))));
     /* test we get error when filling twice X before filling Y (entries not updated) */
     /* comment this out after the test */
     /* myProfile->FillX(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))))); */
     myProfile->FillY(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px)))));
  }
  /* let's match both profile supports */
  Double_t hprofileXSum = 0.0;
  Double_t hprofileYSum = 0.0;
  Double_t myProfileXSum = 0.0;
  Double_t myProfileYSum = 0.0;
  Double_t hprofileXErrSum = 0.0;
  Double_t hprofileYErrSum = 0.0;
  Double_t myProfileXErrSum = 0.0;
  Double_t myProfileYErrSum = 0.0;
  for (Int_t binpx = 0; binpx < nBins; binpx++) {
    for (Int_t binpy = 0; binpy < nBins; binpy++) {
      /* for the profile is easy */
      Double_t hprofileX2dBinContent = hprofX2d->GetBinContent(hprofX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileX2dBinError =  hprofX2d->GetBinError(hprofX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileX2dBinEntries = hprofX2d->GetBinEntries(hprofX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileY2dBinContent = hprofY2d->GetBinContent(hprofY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileY2dBinError =  hprofY2d->GetBinError(hprofY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileY2dBinEntries = hprofY2d->GetBinEntries(hprofY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));

      /* for us we have to fill the variable container first */
      varContainer[kPx] = binmiddle[binpx];
      varContainer[kPy] = binmiddle[binpy];
      /* test we cannot access the GetBinContent base method -> get error */
      /* comment this out after the test */
      /* Double_t myProfileBinContent = myProfile->GetBinContent(myProfile->GetBin(varContainer)); */
      /* test we get error when accessing not allocated harmonic */
      /* comment this out after the test */
      /* Double_t myProfileBinContent = myProfile->GetXBinContent(1,myProfile->GetBin(varContainer)); */
      Double_t myProfileXBinContent = myProfile->GetXBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXBinError = myProfile->GetXBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYBinContent = myProfile->GetYBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYBinError = myProfile->GetYBinError(myHarmonic,myProfile->GetBin(varContainer));
      /* remove comment if you want a detailed output of each of the bins */
      /* cout << Form("Profile 2D X: %013.9f +/- %013.9f in: %03d entries;  My profile X: %013.9f +/- %013.9f\n",
          hprofileX2dBinContent, hprofileX2dBinError, hprofileX2dBinEntries, myProfileXBinContent, myProfileXBinError);
      cout << Form("Profile 2D Y: %013.9f +/- %013.9f in: %03d entries;  My profile Y: %013.9f +/- %013.9f\n",
          hprofileY2dBinContent, hprofileY2dBinError, hprofileY2dBinEntries, myProfileYBinContent, myProfileYBinError); */
      if (hprofileX2dBinEntries > 1) {
        hprofileXSum += hprofileX2dBinContent;
        myProfileXSum += myProfileXBinContent;
        hprofileXErrSum += hprofileX2dBinError;
        myProfileXErrSum += myProfileXBinError;
      }
      if (hprofileY2dBinEntries > 1) {
        hprofileYSum += hprofileY2dBinContent;
        myProfileYSum += myProfileYBinContent;
        hprofileYErrSum += hprofileY2dBinError;
        myProfileYErrSum += myProfileYBinError;
      }
    }
  }
  cout << Form("Profile 2D X cummulated sum: %20.9f, my profile X cummulated sum: %20.9f\n", hprofileXSum, myProfileXSum);
  cout << Form("Profile 2D X error cummulated sum: %20.9f, my profile X error cummulated sum: %20.9f\n\n", hprofileXErrSum, myProfileXErrSum);
  cout << Form("Profile 2D Y cummulated sum: %20.9f, my profile Y cummulated sum: %20.9f\n", hprofileYSum, myProfileYSum);
  cout << Form("Profile 2D Y error cummulated sum: %20.9f, my profile Y error cummulated sum: %20.9f\n\n", hprofileYErrSum, myProfileYErrSum);

  myList->Print();
  hprofX2d->Print();
  hprofY2d->Print();

  /* Check now we are able to attach to a new constructed object the hsitograms existing in the list */
  delete myProfile;       /* should not delete the histograms owned by the list */

  /* we create it back */
  myProfile = new QnCorrectionsComponentsProfile("QnCorrectionsComponentsProfile", "myComponentsProfile", evtClassSet);
  if(myProfile->AttachHistograms(myList))
    cout << "OK: histograms properly attached to the Components Profile object\n";
  else
    cout << "ERROR: something went wrong\n";

  /* check we keep the proper harmonic order */
  /* test we get error when accessing not allocated harmonic */
  /* comment this out after the test */
  /* Double_t myProfileBinContent = myProfile->GetXBinContent(1,myProfile->GetBin(varContainer)); */
  /* and that we keep everything */
  hprofileXSum = 0.0;
  hprofileYSum = 0.0;
  myProfileXSum = 0.0;
  myProfileYSum = 0.0;
  hprofileXErrSum = 0.0;
  hprofileYErrSum = 0.0;
  myProfileXErrSum = 0.0;
  myProfileYErrSum = 0.0;
  for (Int_t binpx = 0; binpx < nBins; binpx++) {
    for (Int_t binpy = 0; binpy < nBins; binpy++) {
      /* for the profile is easy */
      Double_t hprofileX2dBinContent = hprofX2d->GetBinContent(hprofX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileX2dBinError =  hprofX2d->GetBinError(hprofX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileX2dBinEntries = hprofX2d->GetBinEntries(hprofX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileY2dBinContent = hprofY2d->GetBinContent(hprofY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileY2dBinError =  hprofY2d->GetBinError(hprofY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileY2dBinEntries = hprofY2d->GetBinEntries(hprofY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));

      /* for us we have to fill the variable container first */
      varContainer[kPx] = binmiddle[binpx];
      varContainer[kPy] = binmiddle[binpy];
      /* test we cannot access the GetBinContent base method -> get error */
      /* comment this out after the test */
      /* Double_t myProfileBinContent = myProfile->GetBinContent(myProfile->GetBin(varContainer)); */
      /* test we get error when accessing not allocated harmonic */
      /* comment this out after the test */
      /* Double_t myProfileBinContent = myProfile->GetXBinContent(1,myProfile->GetBin(varContainer)); */
      Double_t myProfileXBinContent = myProfile->GetXBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXBinError = myProfile->GetXBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYBinContent = myProfile->GetYBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYBinError = myProfile->GetYBinError(myHarmonic,myProfile->GetBin(varContainer));
      /* remove comment if you want a detailed output of each of the bins */
      /* cout << Form("Profile 2D X: %013.9f +/- %013.9f in: %03d entries;  My profile X: %013.9f +/- %013.9f\n",
          hprofileX2dBinContent, hprofileX2dBinError, hprofileX2dBinEntries, myProfileXBinContent, myProfileXBinError);
      cout << Form("Profile 2D Y: %013.9f +/- %013.9f in: %03d entries;  My profile Y: %013.9f +/- %013.9f\n",
          hprofileY2dBinContent, hprofileY2dBinError, hprofileY2dBinEntries, myProfileYBinContent, myProfileYBinError); */
      if (hprofileX2dBinEntries > 1) {
        hprofileXSum += hprofileX2dBinContent;
        myProfileXSum += myProfileXBinContent;
        hprofileXErrSum += hprofileX2dBinError;
        myProfileXErrSum += myProfileXBinError;
      }
      if (hprofileY2dBinEntries > 1) {
        hprofileYSum += hprofileY2dBinContent;
        myProfileYSum += myProfileYBinContent;
        hprofileYErrSum += hprofileY2dBinError;
        myProfileYErrSum += myProfileYBinError;
      }
    }
  }
  cout << Form("Profile 2D X cummulated sum: %20.9f, my attached profile X cummulated sum: %20.9f\n", hprofileXSum, myProfileXSum);
  cout << Form("Profile 2D X error cummulated sum: %20.9f, my attached profile X error cummulated sum: %20.9f\n\n", hprofileXErrSum, myProfileXErrSum);
  cout << Form("Profile 2D Y cummulated sum: %20.9f, my attached profile Y cummulated sum: %20.9f\n", hprofileYSum, myProfileYSum);
  cout << Form("Profile 2D Y error cummulated sum: %20.9f, my attached profile Y error cummulated sum: %20.9f\n\n", hprofileYErrSum, myProfileYErrSum);

  delete myProfile;
  delete myList;
  delete hprofX2d;
  delete hprofY2d;
}

/// Test for the correlations components profile function support
void TestCorrelationComponentsHistograms() {
  /* let's test now the correlation components profile functions */
  cout << "\n\nCORRELATION COMPONENTS PROFILE HISTOGRAMS TESTS\n===============================================\n";
  Int_t nBins = 40;
  Double_t min = -4.0;
  Double_t max = 4.0;
  Double_t binwidth = (max - min) / nBins;
  Double_t binmiddle[nBins]; for (Int_t bin = 0; bin < nBins; bin++) binmiddle[bin] = min + binwidth / 2 + bin * binwidth;

  /* the ROOT 2D profiles */
  TProfile2D *hprofXX2d  =
      new TProfile2D("hprofXX2d","Profile of pz*|cos 2#phi * cos 2#phi| versus px and py",nBins,min,max,nBins,min,max,0,20, "s");
  TProfile2D *hprofXY2d  =
      new TProfile2D("hprofXY2d","Profile of pz*|cos 2#phi * sin 2#phi| versus px and py",nBins,min,max,nBins,min,max,0,20, "s");
  TProfile2D *hprofYX2d  =
      new TProfile2D("hprofYX2d","Profile of pz*|sin 2#phi * cos 2#phi| versus px and py",nBins,min,max,nBins,min,max,0,20, "s");
  TProfile2D *hprofYY2d  =
      new TProfile2D("hprofYY2d","Profile of pz*|sin 2#phi * sin 2#phi| versus px and py",nBins,min,max,nBins,min,max,0,20, "s");

  Float_t px, py, pz;
  /* now let's build our event class variables */
  Int_t nDimensions = 2;
  QnCorrectionsEventClassVariablesSet evtClassSet = QnCorrectionsEventClassVariablesSet(nDimensions);
  evtClassSet[0] = new QnCorrectionsEventClassVariable(kPx,VarNames[kPx],nBins,min,max);
  evtClassSet[1] = new QnCorrectionsEventClassVariable(kPy,VarNames[kPy],nBins,min,max);

  /* the variable container */
  Float_t varContainer[kNVars];

  /* and now our histogram */
  Int_t nNoOfHarmonics = 1;
  Int_t harmonicsMap[] = {2};
  Int_t myHarmonic = 2;
  QnCorrectionsCorrelationComponentsProfile *myProfile =
      new QnCorrectionsCorrelationComponentsProfile("QnCorrectionsCorrelationComponentsProfile", "myCorrelationComponentsProfile", evtClassSet);
  /* and the list we need for create it */
  TList *myList = new TList(); myList->SetOwner(kTRUE);
  myProfile->CreateCorrelationComponentsProfileHistograms(myList,nNoOfHarmonics,harmonicsMap);
  myList->Print();

  /* taken from the TProfile2D description */
  for ( Int_t i=0; i<25000; i++) {
     gRandom->Rannor(px,py);
     pz = px*px + py*py;
     hprofXX2d->Fill(px,py,pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))*TMath::Cos(2*TMath::ATan2(py,px)))),1);
     hprofXY2d->Fill(px,py,pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))*TMath::Sin(2*TMath::ATan2(py,px)))),1);
     hprofYX2d->Fill(px,py,pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px))*TMath::Cos(2*TMath::ATan2(py,px)))),1);
     hprofYY2d->Fill(px,py,pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px))*TMath::Sin(2*TMath::ATan2(py,px)))),1);

     /* let's fill the variable container with the new sample */
     varContainer[kPx] = px;
     varContainer[kPy] = py;

     /* and now fill our profile */
     /* test we cannot access the FillY base method -> get error */
     /* comment this out after the test */
     /* myProfile->FillY(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px))))); */
     /* test we get error when accessing not allocated harmonic */
     /* comment this out after the test */
     /* myProfile->FillXX(1, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))*TMath::Cos(2*TMath::ATan2(py,px))))); */
     myProfile->FillXX(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))*TMath::Cos(2*TMath::ATan2(py,px)))));
     myProfile->FillXY(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))*TMath::Sin(2*TMath::ATan2(py,px)))));
     /* test we get error when filling twice X before filling Y (entries not updated) */
     /* comment this out after the test */
     /* myProfile->FillXY(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Cos(2*TMath::ATan2(py,px))*TMath::Sin(2*TMath::ATan2(py,px))))); */
     myProfile->FillYX(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px))*TMath::Cos(2*TMath::ATan2(py,px)))));
     myProfile->FillYY(myHarmonic, varContainer, pz*(TMath::Abs(TMath::Sin(2*TMath::ATan2(py,px))*TMath::Sin(2*TMath::ATan2(py,px)))));
  }
  /* let's match both profile supports */
  Double_t hprofileXXSum = 0.0;
  Double_t hprofileXYSum = 0.0;
  Double_t hprofileYXSum = 0.0;
  Double_t hprofileYYSum = 0.0;
  Double_t myProfileXXSum = 0.0;
  Double_t myProfileXYSum = 0.0;
  Double_t myProfileYXSum = 0.0;
  Double_t myProfileYYSum = 0.0;
  Double_t hprofileXXErrSum = 0.0;
  Double_t hprofileXYErrSum = 0.0;
  Double_t hprofileYXErrSum = 0.0;
  Double_t hprofileYYErrSum = 0.0;
  Double_t myProfileXXErrSum = 0.0;
  Double_t myProfileXYErrSum = 0.0;
  Double_t myProfileYXErrSum = 0.0;
  Double_t myProfileYYErrSum = 0.0;
  for (Int_t binpx = 0; binpx < nBins; binpx++) {
    for (Int_t binpy = 0; binpy < nBins; binpy++) {
      /* for the profile is easy */
      Double_t hprofileXX2dBinContent = hprofXX2d->GetBinContent(hprofXX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileXX2dBinError =  hprofXX2d->GetBinError(hprofXX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileXX2dBinEntries = hprofXX2d->GetBinEntries(hprofXX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileXY2dBinContent = hprofXY2d->GetBinContent(hprofXY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileXY2dBinError =  hprofXY2d->GetBinError(hprofXY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileXY2dBinEntries = hprofXY2d->GetBinEntries(hprofXY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYX2dBinContent = hprofYX2d->GetBinContent(hprofYX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYX2dBinError =  hprofYX2d->GetBinError(hprofYX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileYX2dBinEntries = hprofYX2d->GetBinEntries(hprofYX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYY2dBinContent = hprofYY2d->GetBinContent(hprofYY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYY2dBinError =  hprofYY2d->GetBinError(hprofYY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileYY2dBinEntries = hprofYY2d->GetBinEntries(hprofYY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));

      /* for us we have to fill the variable container first */
      varContainer[kPx] = binmiddle[binpx];
      varContainer[kPy] = binmiddle[binpy];
      /* test we cannot access the GetxBinContent base method -> get error */
      /* comment this out after the test */
      /* Double_t myProfileXBinContent = myProfile->GetXBinContent(myHarmonic,myProfile->GetBin(varContainer)); */
      /* test we get error when accessing not allocated harmonic */
      /* comment this out after the test */
      /* Double_t myProfileBinError = myProfile->GetXYBinError(1,myProfile->GetBin(varContainer)); */
      Double_t myProfileXXBinContent = myProfile->GetXXBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXXBinError = myProfile->GetXXBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXYBinContent = myProfile->GetXYBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXYBinError = myProfile->GetXYBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYXBinContent = myProfile->GetYXBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYXBinError = myProfile->GetYXBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYYBinContent = myProfile->GetYYBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYYBinError = myProfile->GetYYBinError(myHarmonic,myProfile->GetBin(varContainer));
      /* remove comment if you want a detailed output of each of the bins */
      /* cout << Form("Profile 2D XX: %013.9f +/- %013.9f in: %03d entries;  My profile XX: %013.9f +/- %013.9f\n",
          hprofileXX2dBinContent, hprofileXX2dBinError, hprofileXX2dBinEntries, myProfileXXBinContent, myProfileXXBinError);
      cout << Form("Profile 2D XY: %013.9f +/- %013.9f in: %03d entries;  My profile XY: %013.9f +/- %013.9f\n",
          hprofileXY2dBinContent, hprofileXY2dBinError, hprofileXY2dBinEntries, myProfileXYBinContent, myProfileXYBinError);
      cout << Form("Profile 2D YX: %013.9f +/- %013.9f in: %03d entries;  My profile YX: %013.9f +/- %013.9f\n",
          hprofileYX2dBinContent, hprofileYX2dBinError, hprofileYX2dBinEntries, myProfileYXBinContent, myProfileYXBinError);
      cout << Form("Profile 2D YY: %013.9f +/- %013.9f in: %03d entries;  My profile YY: %013.9f +/- %013.9f\n",
          hprofileYY2dBinContent, hprofileYY2dBinError, hprofileYY2dBinEntries, myProfileYYBinContent, myProfileYYBinError); */
      if (hprofileXX2dBinEntries > 1) {
        hprofileXXSum += hprofileXX2dBinContent;
        myProfileXXSum += myProfileXXBinContent;
        hprofileXXErrSum += hprofileXX2dBinError;
        myProfileXXErrSum += myProfileXXBinError;
      }
      if (hprofileXY2dBinEntries > 1) {
        hprofileXYSum += hprofileXY2dBinContent;
        myProfileXYSum += myProfileXYBinContent;
        hprofileXYErrSum += hprofileXY2dBinError;
        myProfileXYErrSum += myProfileXYBinError;
      }
      if (hprofileYX2dBinEntries > 1) {
        hprofileYXSum += hprofileYX2dBinContent;
        myProfileYXSum += myProfileYXBinContent;
        hprofileYXErrSum += hprofileYX2dBinError;
        myProfileYXErrSum += myProfileYXBinError;
      }
      if (hprofileYY2dBinEntries > 1) {
        hprofileYYSum += hprofileYY2dBinContent;
        myProfileYYSum += myProfileYYBinContent;
        hprofileYYErrSum += hprofileYY2dBinError;
        myProfileYYErrSum += myProfileYYBinError;
      }
    }
  }
  cout << Form("Profile 2D XX cummulated sum: %20.9f, my profile XX cummulated sum: %20.9f\n", hprofileXXSum, myProfileXXSum);
  cout << Form("Profile 2D XX error cummulated sum: %20.9f, my profile XX error cummulated sum: %20.9f\n\n", hprofileXXErrSum, myProfileXXErrSum);
  cout << Form("Profile 2D XY cummulated sum: %20.9f, my profile XY cummulated sum: %20.9f\n", hprofileXYSum, myProfileXYSum);
  cout << Form("Profile 2D XY error cummulated sum: %20.9f, my profile XY error cummulated sum: %20.9f\n\n", hprofileXYErrSum, myProfileXYErrSum);
  cout << Form("Profile 2D YX cummulated sum: %20.9f, my profile YX cummulated sum: %20.9f\n", hprofileYXSum, myProfileYXSum);
  cout << Form("Profile 2D YX error cummulated sum: %20.9f, my profile YX error cummulated sum: %20.9f\n\n", hprofileYXErrSum, myProfileYXErrSum);
  cout << Form("Profile 2D YY cummulated sum: %20.9f, my profile YY cummulated sum: %20.9f\n", hprofileYYSum, myProfileYYSum);
  cout << Form("Profile 2D YY error cummulated sum: %20.9f, my profile YY error cummulated sum: %20.9f\n\n", hprofileYYErrSum, myProfileYYErrSum);

  myList->Print();
  hprofXX2d->Print();
  hprofXY2d->Print();
  hprofYX2d->Print();
  hprofYY2d->Print();

  /* Check now we are able to attach to a new constructed object the hsitograms existing in the list */
  delete myProfile;       /* should not delete the histograms owned by the list */

  /* we create it back */
  myProfile = new QnCorrectionsCorrelationComponentsProfile("QnCorrectionsCorrelationComponentsProfile", "myCorrelationComponentsProfile", evtClassSet);
  if(myProfile->AttachHistograms(myList))
    cout << "OK: histograms properly attached to the Components Profile object\n";
  else
    cout << "ERROR: something went wrong\n";

  /* check we keep the proper harmonic order */
  /* test we get error when accessing not allocated harmonic */
  /* comment this out after the test */
  /* Double_t myProfileBinContent = myProfile->GetXXBinContent(1,myProfile->GetBin(varContainer)); */
  /* and that we keep everything */
  hprofileXXSum = 0.0;
  hprofileXYSum = 0.0;
  hprofileYXSum = 0.0;
  hprofileYYSum = 0.0;
  myProfileXXSum = 0.0;
  myProfileXYSum = 0.0;
  myProfileYXSum = 0.0;
  myProfileYYSum = 0.0;
  hprofileXXErrSum = 0.0;
  hprofileXYErrSum = 0.0;
  hprofileYXErrSum = 0.0;
  hprofileYYErrSum = 0.0;
  myProfileXXErrSum = 0.0;
  myProfileXYErrSum = 0.0;
  myProfileYXErrSum = 0.0;
  myProfileYYErrSum = 0.0;
  for (Int_t binpx = 0; binpx < nBins; binpx++) {
    for (Int_t binpy = 0; binpy < nBins; binpy++) {
      /* for the profile is easy */
      Double_t hprofileXX2dBinContent = hprofXX2d->GetBinContent(hprofXX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileXX2dBinError =  hprofXX2d->GetBinError(hprofXX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileXX2dBinEntries = hprofXX2d->GetBinEntries(hprofXX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileXY2dBinContent = hprofXY2d->GetBinContent(hprofXY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileXY2dBinError =  hprofXY2d->GetBinError(hprofXY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileXY2dBinEntries = hprofXY2d->GetBinEntries(hprofXY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYX2dBinContent = hprofYX2d->GetBinContent(hprofYX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYX2dBinError =  hprofYX2d->GetBinError(hprofYX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileYX2dBinEntries = hprofYX2d->GetBinEntries(hprofYX2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYY2dBinContent = hprofYY2d->GetBinContent(hprofYY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Double_t hprofileYY2dBinError =  hprofYY2d->GetBinError(hprofYY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));
      Int_t hprofileYY2dBinEntries = hprofYY2d->GetBinEntries(hprofYY2d->FindBin(binmiddle[binpx], binmiddle[binpy]));

      /* for us we have to fill the variable container first */
      varContainer[kPx] = binmiddle[binpx];
      varContainer[kPy] = binmiddle[binpy];
      /* test we cannot access the GetxBinContent base method -> get error */
      /* comment this out after the test */
      /* Double_t myProfileXBinContent = myProfile->GetXBinContent(myHarmonic,myProfile->GetBin(varContainer)); */
      /* test we get error when accessing not allocated harmonic */
      /* comment this out after the test */
      /* Double_t myProfileXYBinError = myProfile->GetXYBinError(1,myProfile->GetBin(varContainer)); */
      Double_t myProfileXXBinContent = myProfile->GetXXBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXXBinError = myProfile->GetXXBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXYBinContent = myProfile->GetXYBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileXYBinError = myProfile->GetXYBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYXBinContent = myProfile->GetYXBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYXBinError = myProfile->GetYXBinError(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYYBinContent = myProfile->GetYYBinContent(myHarmonic,myProfile->GetBin(varContainer));
      Double_t myProfileYYBinError = myProfile->GetYYBinError(myHarmonic,myProfile->GetBin(varContainer));
      /* remove comment if you want a detailed output of each of the bins */
      /* cout << Form("Profile 2D XX: %013.9f +/- %013.9f in: %03d entries;  My profile XX: %013.9f +/- %013.9f\n",
          hprofileXX2dBinContent, hprofileXX2dBinError, hprofileXX2dBinEntries, myProfileXXBinContent, myProfileXXBinError);
      cout << Form("Profile 2D XY: %013.9f +/- %013.9f in: %03d entries;  My profile XY: %013.9f +/- %013.9f\n",
          hprofileXY2dBinContent, hprofileXY2dBinError, hprofileXY2dBinEntries, myProfileXYBinContent, myProfileXYBinError);
      cout << Form("Profile 2D YX: %013.9f +/- %013.9f in: %03d entries;  My profile YX: %013.9f +/- %013.9f\n",
          hprofileYX2dBinContent, hprofileYX2dBinError, hprofileYX2dBinEntries, myProfileYXBinContent, myProfileYXBinError);
      cout << Form("Profile 2D YY: %013.9f +/- %013.9f in: %03d entries;  My profile YY: %013.9f +/- %013.9f\n",
          hprofileYY2dBinContent, hprofileYY2dBinError, hprofileYY2dBinEntries, myProfileYYBinContent, myProfileYYBinError); */
      if (hprofileXX2dBinEntries > 1) {
        hprofileXXSum += hprofileXX2dBinContent;
        myProfileXXSum += myProfileXXBinContent;
        hprofileXXErrSum += hprofileXX2dBinError;
        myProfileXXErrSum += myProfileXXBinError;
      }
      if (hprofileXY2dBinEntries > 1) {
        hprofileXYSum += hprofileXY2dBinContent;
        myProfileXYSum += myProfileXYBinContent;
        hprofileXYErrSum += hprofileXY2dBinError;
        myProfileXYErrSum += myProfileXYBinError;
      }
      if (hprofileYX2dBinEntries > 1) {
        hprofileYXSum += hprofileYX2dBinContent;
        myProfileYXSum += myProfileYXBinContent;
        hprofileYXErrSum += hprofileYX2dBinError;
        myProfileYXErrSum += myProfileYXBinError;
      }
      if (hprofileYY2dBinEntries > 1) {
        hprofileYYSum += hprofileYY2dBinContent;
        myProfileYYSum += myProfileYYBinContent;
        hprofileYYErrSum += hprofileYY2dBinError;
        myProfileYYErrSum += myProfileYYBinError;
      }
    }
  }
  cout << Form("Profile 2D XX cummulated sum: %20.9f, my profile XX cummulated sum: %20.9f\n", hprofileXXSum, myProfileXXSum);
  cout << Form("Profile 2D XX error cummulated sum: %20.9f, my profile XX error cummulated sum: %20.9f\n\n", hprofileXXErrSum, myProfileXXErrSum);
  cout << Form("Profile 2D XY cummulated sum: %20.9f, my profile XY cummulated sum: %20.9f\n", hprofileXYSum, myProfileXYSum);
  cout << Form("Profile 2D XY error cummulated sum: %20.9f, my profile XY error cummulated sum: %20.9f\n\n", hprofileXYErrSum, myProfileXYErrSum);
  cout << Form("Profile 2D YX cummulated sum: %20.9f, my profile YX cummulated sum: %20.9f\n", hprofileYXSum, myProfileYXSum);
  cout << Form("Profile 2D YX error cummulated sum: %20.9f, my profile YX error cummulated sum: %20.9f\n\n", hprofileYXErrSum, myProfileYXErrSum);
  cout << Form("Profile 2D YY cummulated sum: %20.9f, my profile YY cummulated sum: %20.9f\n", hprofileYYSum, myProfileYYSum);
  cout << Form("Profile 2D YY error cummulated sum: %20.9f, my profile YY error cummulated sum: %20.9f\n\n", hprofileYYErrSum, myProfileYYErrSum);


  delete myProfile;
  delete myList;
  delete hprofXX2d;
  delete hprofXY2d;
  delete hprofYX2d;
  delete hprofYY2d;
}

/// Test for the cuts function support
void TestCuts() {
  /* let's test now the cuts functions */
  cout << "\n\nCUTS TESTS\n==========\n";

  /* define the variables we are going to use */
  enum {
    kValueToFilterBit,
    kVariableAbove,
    kVariableBelow,
    kVariableWithin,
    kVariableOutside,
    kVariableValue,
    kNoOfVariables
  };

  /* let's define the bit numbering */
  enum {
    kBit0,
    kBit1,
    kBit2,
    kBit3,
    kBit4,
    kBit5,
    kBit6,
    kBit7
  };

  /* and let's define our variable bank */
  Float_t varBank[kNoOfVariables];

  /* let's create few cuts */
  /* first the cuts set storage */
  QnCorrectionsCutsSet mySetOfCuts;
  mySetOfCuts.Add(new QnCorrectionsCutBitSet(kValueToFilterBit, kBit3, kTRUE));
  mySetOfCuts.Add(new QnCorrectionsCutBitSet(kValueToFilterBit, kBit7, kFALSE));
  mySetOfCuts.Add(new QnCorrectionsCutAbove(kVariableAbove, 3.5));
  mySetOfCuts.Add(new QnCorrectionsCutBelow(kVariableBelow, 1.7));
  mySetOfCuts.Add(new QnCorrectionsCutWithin(kVariableWithin, -0.8, 0.8));
  mySetOfCuts.Add(new QnCorrectionsCutOutside(kVariableOutside, -0.3, 0.3));
  mySetOfCuts.Add(new QnCorrectionsCutValue(kVariableValue, -1.3));
  /* let's own the cuts due to how they are created */
  mySetOfCuts.SetOwner(kTRUE);

  /* let's put some values */
  varBank[kValueToFilterBit] = 0x0080; /* will fail both */
  varBank[kVariableAbove] = 1.9; /* will fail */
  varBank[kVariableBelow] = 1.75; /* will fail */
  varBank[kVariableWithin] = 0.87; /* will fail */
  varBank[kVariableOutside] = 0; /* will fail */
  varBank[kVariableValue] = -1.3007; /* will fail */

  /* let's check the over all set */
  if (!mySetOfCuts.IsSelected(varBank)) cout << "  correctly the set of cuts rejects the variable bank content\n";
  else cout << "  ERROR: should have rejected the variable content\n";

  /* let's fine tune the rejection */
  if (!mySetOfCuts.At(0)->IsSelected(varBank)) cout << "  correct filter bit set\n";
  else cout << "  ERROR: filter bit set\n";
  if (!mySetOfCuts.At(1)->IsSelected(varBank)) cout << "  correct filter bit reset\n";
  else cout << "  ERROR: filter bit reset\n";
  if (!mySetOfCuts.At(2)->IsSelected(varBank)) cout << "  correct filter above\n";
  else cout << "  ERROR: filter above\n";
  if (!mySetOfCuts.At(3)->IsSelected(varBank)) cout << "  correct filter below\n";
  else cout << "  ERROR: filter below\n";
  if (!mySetOfCuts.At(4)->IsSelected(varBank)) cout << "  correct filter within\n";
  else cout << "  ERROR: filter within\n";
  if (!mySetOfCuts.At(5)->IsSelected(varBank)) cout << "  correct filter outside\n";
  else cout << "  ERROR: filter outside\n";
  if (!mySetOfCuts.At(6)->IsSelected(varBank)) cout << "  correct filter value\n";
  else cout << "  ERROR: filter value\n";

  varBank[kValueToFilterBit] = 0x007F; /* will pass both */
  varBank[kVariableAbove] = 3.7; /* will pass */
  varBank[kVariableBelow] = 1.69; /* will pass */
  varBank[kVariableWithin] = -0.87; /* will fail */
  varBank[kVariableOutside] = -3.01; /* will pass */
  varBank[kVariableValue] = -1.3; /* will pass */

  /* let's check the over all set */
  if (!mySetOfCuts.IsSelected(varBank)) cout << "  correctly the set of cuts rejects the variable bank content\n";
  else cout << "  ERROR: should have rejected the variable content\n";

  /* let's fine tune the rejection */
  if (mySetOfCuts.At(0)->IsSelected(varBank)) cout << "  correct filter bit set\n";
  else cout << "  ERROR: filter bit set\n";
  if (mySetOfCuts.At(1)->IsSelected(varBank)) cout << "  correct filter bit reset\n";
  else cout << "  ERROR: filter bit reset\n";
  if (mySetOfCuts.At(2)->IsSelected(varBank)) cout << "  correct filter above\n";
  else cout << "  ERROR: filter above\n";
  if (mySetOfCuts.At(3)->IsSelected(varBank)) cout << "  correct filter below\n";
  else cout << "  ERROR: filter below\n";
  if (!mySetOfCuts.At(4)->IsSelected(varBank)) cout << "  correct filter within\n";
  else cout << "  ERROR: filter within\n";
  if (mySetOfCuts.At(5)->IsSelected(varBank)) cout << "  correct filter outside\n";
  else cout << "  ERROR: filter outside\n";
  if (mySetOfCuts.At(6)->IsSelected(varBank)) cout << "  correct filter value\n";
  else cout << "  ERROR: filter value\n";

  /* so the last one to pass it */
  varBank[kVariableWithin] = -0.73; /* will pass */

  /* let's check the over all set */
  if (mySetOfCuts.IsSelected(varBank)) cout << "  correctly the set of cuts accepts the variable bank content\n";
  else cout << "  ERROR: should have accepted the variable content\n";
  /* let's fine tune the rejection */
  if (mySetOfCuts.At(4)->IsSelected(varBank)) cout << "  correct filter within\n";
  else cout << "  ERROR: filter within\n";
}

/// Test the data vectors and the Qn vectors
/// \param nEvents number of events to simulate
void TestDataVectorsAndQnVectors(Int_t nEvents) {
  /* let's test now vector functions */
  cout << "\n\nVECTORS TESTS\n=============\n";

  /// the data vectors bank
  TClonesArray *dataVectorsBank = new TClonesArray("QnCorrectionsDataVector", 100000);
  /// the channelized data vectors bank
  TClonesArray *channelizedDataVectorsBank = new TClonesArray("QnCorrectionsChannelizedDataVector", 100000);

  /// the Q vectors we are going to use
  Int_t nHarmonics = 3;
  Int_t harmonicMap[] = {2, 4, 6};
  QnCorrectionsQnVectorBuild myChannelizedDetectorQnVector(nHarmonics,harmonicMap);
  QnCorrectionsQnVectorBuild myDetectorQnVector(nHarmonics,harmonicMap);

  /// loop over events
  for (Int_t event = 0; event < nEvents; event++) {
    /// Set event data
    Float_t centrality = gRandom->Rndm()*100;
    cout << "Centrality: " << centrality << "\n";

    /// Fill azimuthal angle and weights into data vector objects
    Float_t dphi = 2.0*TMath::Pi() / 8.0;
    const Double_t phiSector[8] = { dphi*0.0,dphi*1.0,dphi*2.0,dphi*3.0,dphi*4.0,dphi*5.0,dphi*6.0,dphi*7.0 };

    Double_t flowV2 = 0.5;
    Double_t rotation = -0.3;
    Double_t PsiRP = gRandom->Rndm()*2.0*TMath::Pi();

    Double_t weight = 0.0;

    for(Int_t ich=0; ich<64; ich++){
      // weight contains flow and event multiplicity dependent channel signal, and non-uniform acceptance
      weight = gRandom->Rndm()*((200.+ich)/200.)*(100-centrality)*(1+flowV2*TMath::Cos(2*(phiSector[ich%8]-PsiRP)));

      /// add the data vector to the bank
      QnCorrectionsChannelizedDataVector *channelizedDataVector =
          new (channelizedDataVectorsBank->ConstructedAt(channelizedDataVectorsBank->GetEntriesFast()))
            QnCorrectionsChannelizedDataVector(ich, phiSector[ich%8]+rotation, weight);
    }

    /// let's fill the Q vector
    for (Int_t ixdata = 0; ixdata < channelizedDataVectorsBank->GetEntriesFast(); ixdata++) {
      QnCorrectionsChannelizedDataVector *dataVector =
          dynamic_cast<QnCorrectionsChannelizedDataVector*>(channelizedDataVectorsBank->At(ixdata));
      myChannelizedDetectorQnVector.Add(dataVector->Phi(), dataVector->Weight());
      cout << Form("channel: d, phi: %f, weight: %f\n", dataVector->Phi(), dataVector->Weight());
      cout << Form("X(2) comp: %f; X(4) comp: %f; X(6) comp: %f\n",
          myChannelizedDetectorQnVector.Qx(2), myChannelizedDetectorQnVector.Qx(4), myChannelizedDetectorQnVector.Qx(6));
      cout << Form("Y(2) comp: %f; Y(4) comp: %f; Y(6) comp: %f\n",
          myChannelizedDetectorQnVector.Qy(2), myChannelizedDetectorQnVector.Qy(4), myChannelizedDetectorQnVector.Qy(6));
      cout << Form("fN: %d\n", myChannelizedDetectorQnVector.GetN());
      cout << Form("sumW: %f\n", myChannelizedDetectorQnVector.GetSumOfWeights());
    }

    /// let's output the event plane for the different harmonics
    cout << Form("Introduced #Psi: %f\n", PsiRP);
    for (Int_t h = 0; h < nHarmonics; h++) {
      cout << Form("  EP(h=%d): %f\n", harmonicMap[h], myChannelizedDetectorQnVector.EventPlane(harmonicMap[h]));
    }

    /// let's clean everything before next event loop
    channelizedDataVectorsBank->Clear("C");
    myChannelizedDetectorQnVector.Reset();

    /// now not channelized data vectors
    Int_t Multiplicity = Int_t (2 + gRandom->Rndm() * (100-centrality) * 100);
    cout << "Multiplicity: " << Multiplicity << "\n";

    for (Int_t track = 0; track < Multiplicity; track++){
      Double_t phiTrack = gRandom->Rndm() * 2.0 * TMath::Pi();

      /// add the data vector to the bank
      QnCorrectionsDataVector *dataVector =
          new (dataVectorsBank->ConstructedAt(dataVectorsBank->GetEntriesFast()))
              QnCorrectionsDataVector(phiTrack);
    }

    cout << Form("data vector bank entries: %d\n", dataVectorsBank->GetEntriesFast());

    /// let's fill the Q vector
    for (Int_t ixdata = 0; ixdata < dataVectorsBank->GetEntriesFast(); ixdata++) {
      QnCorrectionsDataVector *dataVector =
          dynamic_cast<QnCorrectionsDataVector*>(dataVectorsBank->At(ixdata));
      myDetectorQnVector.Add(dataVector->Phi(), dataVector->Weight());
    }

    /// let's output the event plane for the different harmonics
    cout << Form("Introduced #Psi: %f\n", PsiRP);
    for (Int_t h = 0; h < nHarmonics; h++) {
      cout << Form("  EP(h=%d): %f\n", harmonicMap[h], myDetectorQnVector.EventPlane(harmonicMap[h]));
    }

    /// let's clean everything before next event loop
    dataVectorsBank->Clear("C");
    myDetectorQnVector.Reset();
  }
}
