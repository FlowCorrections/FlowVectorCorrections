#ifndef QNCORRECTIONS_CONSTANTS_H
#define QNCORRECTIONS_CONSTANTS_H
/************************************************************************** 
 * Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>    * 
 * See cxx source for full Copyright notice                               * 
 *************************************************************************/ 
 
 
#include<Rtypes.h>


class QnCorrectionsConstants {

  public:

  enum Constants {
    nHarmonics=6,
    nQnConfigurations=20,
    nDataVectors=10,
    nHistogramDimensions=5,
    nCorrectionSteps=6,//TODO remove duplications with QnCorrectionsSteps
    nCuts=100,
    nDataContainerVariables=2000
  };


  ClassDef(QnCorrectionsConstants, 1);

};





#endif
