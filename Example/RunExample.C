#include <TSystem.h>
#include <TROOT.h>
#include <Riostream.h>

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
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsEventClasses.cxx"+debugString);
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistograms.cxx"+debugString);

  gROOT->LoadMacro("Example.C"+debugString);

#ifdef MAKEEVENTTEXTOUTPUT
  Example(nevents,inputFileName,outputFileName,bTextEventFile);
#else
  Example(nevents,inputFileName,outputFileName);
#endif


}



