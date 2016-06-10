#ifndef QNCORRECTIONS_CORRECTIONSSETONINPUTDATA_H
#define QNCORRECTIONS_CORRECTIONSSETONINPUTDATA_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsCorrectionsSetOnInputData.h
/// \brief Set of corrections on input data support within Q vector correction framework
///

#include <TNamed.h>
#include <TList.h>

#include "QnCorrectionsCorrectionStepBase.h"
#include "QnCorrectionsCorrectionOnInputData.h"

/// \class QnCorrectionsCorrectionsSetOnInputData
/// \brief Encapsulate the set of corrections over input data
///
/// Order matters so, the list must be built with the order in which
/// corrections should be applied.
///
/// The correction steps are own by the object instance so they will
/// be destroyed with it.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 05, 2016

class QnCorrectionsCorrectionsSetOnInputData: public TList {
public:
  QnCorrectionsCorrectionsSetOnInputData();
  virtual ~QnCorrectionsCorrectionsSetOnInputData();

  /// Access the correction step at the passed position
  /// \param i position in the list (starting at zero)
  /// \return the correction step object a position i
  virtual QnCorrectionsCorrectionOnInputData *At(Int_t i) const
    { return (QnCorrectionsCorrectionOnInputData *) TList::At(i);}

  void AddCorrection(QnCorrectionsCorrectionOnInputData *correction);
  void FillOverallCorrectionsList(TList *correctionlist) const;
/// \cond CLASSIMP
  ClassDef(QnCorrectionsCorrectionsSetOnInputData, 1);
/// \endcond
};

#endif // QNCORRECTIONS_CORRECTIONSSETONINPUTDATA_H
