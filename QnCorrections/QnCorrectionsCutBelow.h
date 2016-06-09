#ifndef QNCORRECTIONS_CUT_BELOW_H
#define QNCORRECTIONS_CUT_BELOW_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

#include "QnCorrectionsCutsBase.h"

/// \file QnCorrectionsCutBelow.h
/// \brief Upper limit cut class support for the Q vector correction framework

/// \class QnCorrectionsCutBelow
/// \brief Upper limit cut class for Q vector correction
///
/// Provides support for cuts based in an upper limit
///
/// Stores the threshold value and pass the var Id to its parent.
/// Implements IsSelected accordingly.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 22, 2016
class QnCorrectionsCutBelow: public QnCorrectionsCutsBase {

 public:
  QnCorrectionsCutBelow();
  QnCorrectionsCutBelow(const QnCorrectionsCutBelow &cut);
  QnCorrectionsCutBelow(Int_t varId, Float_t threshold);
  virtual ~QnCorrectionsCutBelow();

  virtual Bool_t IsSelected(const Float_t *variableContainer);
 private:
  Float_t         fThreshold;   ///< The upper, not reached, value

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutBelow, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual value is below the threshold else kFALSE
inline Bool_t QnCorrectionsCutBelow::IsSelected(const Float_t *variableContainer) {
  if (variableContainer[fVarId] < fThreshold)
    return kTRUE;
  else
    return kFALSE;
}

#endif // QNCORRECTIONS_CUT_BELOW_H
