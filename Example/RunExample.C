#include <TSystem.h>
#include <TROOT.h>
#include <Riostream.h>

using std::cout;
using std::endl;

#ifdef MAKEEVENTTEXTOUTPUT
void RunExample(Bool_t bTextEventFile = kFALSE, Int_t nevents=50, TString inputFileName="exampleOutput0.root", TString outputFileName="exampleOutput1.root"){
#else
void RunExample(Int_t nevents=50, TString inputFileName="exampleOutput0.root", TString outputFileName="exampleOutput1.root"){
#endif

  TString location=gSystem->WorkingDirectory();
  if(location.EndsWith("Example")) location.Remove(location.Length()-7,7);
      cout<<location<<endl;
  //
  TString debugString="+g";

  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsLog.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsEventClassVariable.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsEventClassVariablesSet.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutsBase.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutAbove.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutBelow.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutOutside.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutSetBit.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutsSet.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutValue.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCutWithin.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistogramBase.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistogram.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistogramChannelized.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistogramChannelizedSparse.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistogramSparse.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsProfile.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsProfileChannelized.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsProfileChannelizedIngress.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsProfileComponents.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsProfileCorrelationComponents.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsProfileCorrelationComponentsHarmonics.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDataVector.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDataVectorChannelized.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsQnVector.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsQnVectorBuild.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCorrectionStepBase.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCorrectionsSetOnInputData.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCorrectionsSetOnQvector.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCorrectionOnInputData.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCorrectionOnQvector.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDetectorConfigurationBase.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDetectorConfigurationsSet.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDetectorConfigurationChannels.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDetectorConfigurationTracks.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDetector.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsManager.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsInputGainEqualization.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsQnVectorRecentering.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsQnVectorAlignment.cxx"+debugString);

  gROOT->LoadMacro("Example.C"+debugString);

#ifdef MAKEEVENTTEXTOUTPUT
  Example(nevents,inputFileName,outputFileName,bTextEventFile);
#else
  Example(nevents,inputFileName,outputFileName);
#endif


}



