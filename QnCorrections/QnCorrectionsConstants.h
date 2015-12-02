#ifndef QNCORRECTIONS_CONSTANTS_H
#define QNCORRECTIONS_CONSTANTS_H
/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2015                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/
 
#include<Rtypes.h>

class QnCorrectionsConstants {

  public:

  QnCorrectionsConstants() {};
  virtual ~QnCorrectionsConstants() {};

  enum Constants {
    nHarmonics=6,
    nQnConfigurations=20,
    nDataVectors=10,
    nHistogramDimensions=5,
    nCorrectionSteps=6,//TODO remove duplications with QnCorrectionsSteps
    nCuts=100,
    nDataContainerVariables=2000
  };


  enum CorrectionSteps {
    kNothing=-1,
    kPass0,
    kDataVectorEqualization,
    kRecentering,
    kAlignment,
    kTwist,
    kRescaling,
    kNcorrectionSteps,
    kUndefined,
    kCopy
  };


  ClassDef(QnCorrectionsConstants, 1);

};


#endif
