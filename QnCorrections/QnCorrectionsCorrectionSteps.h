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

#include <TNamed.h>
#include <TList.h>

/// \class QnCorrectionsCorrectionStepBase
/// \brief Base class for correction steps
///
/// Each correction has a name and a key. The name identifies it
/// in an open way while the key is used to codify its position
/// in an ordered list of consecutive corrections.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 05, 2016

class QnCorrectionsCorrectionStepBase : public TNamed {
public:
  QnCorrectionsCorrectionStepBase();
  QnCorrectionsCorrectionStepBase(const char *name, const char *key);
  virtual ~QnCorrectionsCorrectionStepBase();

  /// Gets the correction ordering key
  const char *GetKey() const { return (const char *) fKey; }
  Bool_t Before(const QnCorrectionsCorrectionStepBase *correction);

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list) = 0;
  /// Asks for support histograms creation
  ///
  /// Pure virtual function
  /// \param list list where the histograms should be incorporated for its persistence
  /// \return kTRUE if everything went OK
  virtual Bool_t CreateSupportHistograms(TList *list) = 0;
  /// Processes the correction step
  ///
  /// Pure virtual function
  /// \return kTRUE if everything went OK
  virtual Bool_t Process() = 0;
protected:
  TString fKey; ///< the correction key that codifies order information
/// \cond CLASSIMP
  ClassDef(QnCorrectionsCorrectionStepBase, 1);
/// \endcond
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
  QnCorrectionsCorrectionOnInputData(const char *name, const char *key);
  virtual ~QnCorrectionsCorrectionOnInputData();

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list);
  /// Asks for support histograms creation
  ///
  /// Pure virtual function
  /// \param list list where the histograms should be incorporated for its persistence
  /// \return kTRUE if everything went OK
  virtual Bool_t CreateSupportHistograms(TList *list);
  /// Processes the correction step
  ///
  /// Pure virtual function
  /// \return kTRUE if everything went OK
  virtual Bool_t Process() = 0;
/// \cond CLASSIMP
  ClassDef(QnCorrectionsCorrectionOnInputData, 1);
/// \endcond
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
  QnCorrectionsCorrectionOnQvector(const char *name, const char *key);
  virtual ~QnCorrectionsCorrectionOnQvector();

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list);
  /// Asks for support histograms creation
  ///
  /// Pure virtual function
  /// \param list list where the histograms should be incorporated for its persistence
  /// \return kTRUE if everything went OK
  virtual Bool_t CreateSupportHistograms(TList *list);
  /// Processes the correction step
  ///
  /// Pure virtual function
  /// \return kTRUE if everything went OK
  virtual Bool_t Process() = 0;
/// \cond CLASSIMP
  ClassDef(QnCorrectionsCorrectionOnQvector, 1);
/// \endcond
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

  void AddCorrection(QnCorrectionsCorrectionOnInputData *correction);
/// \cond CLASSIMP
  ClassDef(QnCorrectionsSetOfCorrectionsOnInputData, 1);
/// \endcond
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

  void AddCorrection(QnCorrectionsCorrectionOnQvector *correction);
/// \cond CLASSIMP
  ClassDef(QnCorrectionsSetOfCorrectionsOnQvector, 1);
/// \endcond
};

#endif // QNCORRECTIONS_CORRECTIONSTEPS_H
