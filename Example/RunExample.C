//#include <../QnCorrections/QnCorrectionsAxes.h>
//#include <../QnCorrections/QnCorrectionsManager.h>
//#include <../QnCorrections/QnCorrectionsConfiguration.h>
//#include <../QnCorrectionsInterface/QnCorrectionsVarManager.h>
//#include <TArrayS.h>
#include <TSystem.h>
#include <TMath.h>
#include <TROOT.h>
#include <iostream>
#include <TRandom>
#include <TObject.h>


void RunExample(Int_t nevents=5000, TString inputFileName="exampleOutput0.root", TString outputFileName="exampleOutput1.root"){


  //gSystem->CompileMacro("Load.C");


  ////TString location="/mnt/hera/alicesoftware/aliphysics/master/src/PWGPP/EVCHAR/";
  TString location=gSystem->WorkingDirectory();
  if(location.EndsWith("Example")) location.Remove(location.Length()-7,7);
      cout<<location<<endl;
  ////TString location="/hera/alice/jonderw/alicesoftware/aliphysics/master/src/PWGPP/EVCHAR/";
  //
  //
  ////if(gROOT->ProcessLine(".x Load.C")!=0) return;

  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsAxes_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsQnVector_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsDataVector_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsCuts_cxx.so");
  ////gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsHelper_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsConfiguration_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsSteps_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsHistograms_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrections/QnCorrectionsManager_cxx.so");

  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrectionsInterface/QnCorrectionsVarManager_cxx.so");
  //gSystem->Load(location+"/FlowVectorCorrections/QnCorrectionsInterface/QnCorrectionsHistos_cxx.so");

  //
  TString debugString="+g";

  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsQnVector.cxx"+debugString           );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsDataVector.cxx"+debugString         );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsAxes.cxx"+debugString               );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsCuts.cxx"+debugString               );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsConfiguration.cxx"+debugString      );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsHistograms.cxx"+debugString         );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsSteps.cxx"+debugString              );//,debugString);//, "QnCorrections");
  gROOT->LoadMacro(location+"QnCorrections/QnCorrectionsManager.cxx"+debugString            );//,debugString);//, "QnCorrections");


  gROOT->LoadMacro(location+"QnCorrectionsInterface/QnCorrectionsHistos.cxx"+debugString    );//,debugString);//, "QnCorrectionsInterface");
  gROOT->LoadMacro(location+"QnCorrectionsInterface/QnCorrectionsVarManager.cxx"+debugString);//,debugString);//, "QnCorrectionsInterface");

  gROOT->LoadMacro("Example.C"+debugString);

  Example(nevents,inputFileName,outputFileName);



}



