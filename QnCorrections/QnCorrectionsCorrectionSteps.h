#ifndef QNCORRECTIONS_CORRECTIONSTEPS_H
#define QNCORRECTIONS_CORRECTIONSTEPS_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsCorrectionSteps.h
/// \brief Base classes that support the different correction steps within Q vector correction framework
///

#include <TObject.h>
#include <TList.h>

/// \class QnCorrectionsCorrectionStepBase
/// \brief Base class for correction steps
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 05, 2016

class QnCorrectionsCorrectionStepBase : public TNamed {
public:
  QnCorrectionsCorrectionStepBase();
  QnCorrectionsCorrectionStepBase(const char *name);
  virtual ~QnCorrectionsCorrectionStepBase();

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list) = 0;
  /// Processes the correction step
  ///
  /// Pure virtual function
  /// \return kTRUE if everything went OK
  virtual Bool_t Process() = 0;
};

/// \class QnCorrectionsCorrectionOnInputData
/// \brief Base class for correction steps applied to input data
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 05, 2016

class QnCorrectionsCorrectionOnInputData: public QnCorrectionsCorrectionStepBase {
public:
  QnCorrectionsCorrectionOnInputData();
  QnCorrectionsCorrectionOnInputData(const char *name);
  virtual ~QnCorrectionsCorrectionOnInputData();

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list) = 0;
  /// Processes the correction step
  ///
  /// Pure virtual function
  /// \return kTRUE if everything went OK
  virtual Bool_t Process() = 0;
};

/// \class QnCorrectionsCorrectionOnQvector
/// \brief Base class for correction steps applied to a Q vector
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 05, 2016

class QnCorrectionsCorrectionOnQvector: public QnCorrectionsCorrectionStepBase {
public:
  QnCorrectionsCorrectionOnQvector();
  QnCorrectionsCorrectionOnQvector(const char *name);
  virtual ~QnCorrectionsCorrectionOnQvector();

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list) = 0;
  /// Processes the correction step
  ///
  /// Pure virtual function
  /// \return kTRUE if everything went OK
  virtual Bool_t Process() = 0;
};

/// \class QnCorrectionsSetOfCorrectionsOnInputData
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

class QnCorrectionsSetOfCorrectionsOnInputData: public TList {
public:
  QnCorrectionsSetOfCorrectionsOnInputData();
  virtual ~QnCorrectionsSetOfCorrectionsOnInputData();

  /// Access the correction step at the passed position
  /// \param i position in the list (starting at zero)
  /// \return the correction step object a position i
  QnCorrectionsCorrectionOnInputData *At(Int_t i)
    { return (QnCorrectionsCorrectionOnInputData *) TList::At(i);}

};

/// \class QnCorrectionsSetOfCorrectionsOnQvector
/// \brief Encapsulate the set of corrections to apply on Q vectors
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

class QnCorrectionsSetOfCorrectionsOnQvector: public TList {
public:
  QnCorrectionsSetOfCorrectionsOnQvector();
  virtual ~QnCorrectionsSetOfCorrectionsOnQvector();

  /// Access the correction step at the passed position
  /// \param i position in the list (starting at zero)
  /// \return the correction step object a position i
  QnCorrectionsCorrectionOnQvector *At(Int_t i)
    { return (QnCorrectionsCorrectionOnQvector *) TList::At(i);}

};

#endif // QNCORRECTIONS_CORRECTIONSTEPS_H
