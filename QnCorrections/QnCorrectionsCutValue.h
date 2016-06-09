#ifndef QNCORRECTIONS_CUT_VALUE_H
#define QNCORRECTIONS_CUT_VALUE_H

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

/// \file QnCorrectionsCutValue.h
/// \brief Value cut class support for the Q vector correction framework

/// \class QnCorrectionsCutValue
/// \brief Value cut class for Q vector correction
///
/// Provides support for cuts based in the interest
/// variable having a concrete value
///
/// Stores the desired value and pass the var Id to its parent.
/// Implements IsSelected accordingly.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 25, 2016
class QnCorrectionsCutValue: public QnCorrectionsCutsBase {

 public:
  QnCorrectionsCutValue();
  QnCorrectionsCutValue(const QnCorrectionsCutValue &cut);
  QnCorrectionsCutValue(Int_t varId, Float_t value);
  virtual ~QnCorrectionsCutValue();

  virtual Bool_t IsSelected(const Float_t *variableContainer);
 private:
  Float_t         fValue;   ///< The desired value

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutValue, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual variable content is equal to the stored value else kFALSE
inline Bool_t QnCorrectionsCutValue::IsSelected(const Float_t *variableContainer) {
  if (variableContainer[fVarId] != fValue)
    return kFALSE;
  else
    return kTRUE;
}

#endif // QNCORRECTIONS_CUT_VALUE_H
