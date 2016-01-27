#ifndef QNCORRECTIONS_CUTS_H
#define QNCORRECTIONS_CUTS_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsCuts.h
/// \brief Classes that model the cuts support for the Q vector correction framework

#include <TObject.h>
#include <TObjArray.h>

/// \class QnCorrectionsCutBase
/// \brief Base class for the Q vector correction cuts
///
/// Stores the external variable Id the cut should act on
///
/// Provides the interface for the set of different cuts
/// classes.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 22, 2016
class QnCorrectionsCutBase: public TObject {

 public:
  QnCorrectionsCutBase();
  QnCorrectionsCutBase(const QnCorrectionsCutBase &cut);
  QnCorrectionsCutBase(Int_t varId);
  virtual ~QnCorrectionsCutBase();

  /// Gets the variable Id the cut is applied to
  Int_t           GetVariableId() const { return fVarId; }

  /// Check if the actual variable value passes the cut
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param variableContainer the current variables content addressed by var Id
  /// \return kTRUE if the actual value passes the cut else kFALSE
  virtual Bool_t IsSelected(Float_t *variableContainer) = 0;
 protected:
  Int_t         fVarId;   ///< The external Id for the variable in the data bank

  static const Int_t nHighestBitNumberSupported;       ///< The highest bit number the framework support

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutBase, 1);
/// \endcond
};

/// \class QnCorrectionsCutAboveCut
/// \brief Lower limit cut class for Q vector correction
///
/// Provides support for cuts based in a lower limit
///
/// Stores the threshold value and pass the var Id to its parent.
/// Implements IsSelected accordingly.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 22, 2016
class QnCorrectionsCutAboveCut: public QnCorrectionsCutBase {

 public:
  QnCorrectionsCutAboveCut();
  QnCorrectionsCutAboveCut(const QnCorrectionsCutAboveCut &cut);
  QnCorrectionsCutAboveCut(Int_t varId, Float_t threshold);
  virtual ~QnCorrectionsCutAboveCut();

  virtual Bool_t IsSelected(Float_t *variableContainer);
 private:
  Float_t         fThreshold;   ///< The value that must be surpassed

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutAboveCut, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual value is above the threshold else kFALSE
Bool_t QnCorrectionsCutAboveCut::IsSelected(Float_t *variableContainer) {
  if (variableContainer[fVarId] > fThreshold)
    return kTRUE;
  else
    return kFALSE;
}

/// \class QnCorrectionsCutBelowCut
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
class QnCorrectionsCutBelowCut: public QnCorrectionsCutBase {

 public:
  QnCorrectionsCutBelowCut();
  QnCorrectionsCutBelowCut(const QnCorrectionsCutBelowCut &cut);
  QnCorrectionsCutBelowCut(Int_t varId, Float_t threshold);
  virtual ~QnCorrectionsCutBelowCut();

  virtual Bool_t IsSelected(Float_t *variableContainer);
 private:
  Float_t         fThreshold;   ///< The upper, not reached, value

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutBelowCut, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual value is below the threshold else kFALSE
Bool_t QnCorrectionsCutBelowCut::IsSelected(Float_t *variableContainer) {
  if (variableContainer[fVarId] < fThreshold)
    return kTRUE;
  else
    return kFALSE;
}

/// \class QnCorrectionsCutWithinCut
/// \brief Within range cut class for Q vector correction
///
/// Provides support for cuts based in being within a range
/// defined by a minimum value and a maximum value
///
/// Stores the threshold values and pass the var Id to its parent.
/// Implements IsSelected accordingly.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 22, 2016
class QnCorrectionsCutWithinCut: public QnCorrectionsCutBase {

 public:
  QnCorrectionsCutWithinCut();
  QnCorrectionsCutWithinCut(const QnCorrectionsCutWithinCut &cut);
  QnCorrectionsCutWithinCut(Int_t varId, Float_t min, Float_t max);
  virtual ~QnCorrectionsCutWithinCut();

  virtual Bool_t IsSelected(Float_t *variableContainer);
 private:
  Float_t         fMinThreshold;   ///< The lower limit
  Float_t         fMaxThreshold;   ///< The upper limit

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutWithinCut, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual value is below the threshold else kFALSE
Bool_t QnCorrectionsCutWithinCut::IsSelected(Float_t *variableContainer) {
  if ((fMinThreshold < variableContainer[fVarId]) && (variableContainer[fVarId] < fMaxThreshold))
    return kTRUE;
  else
    return kFALSE;
}

/// \class QnCorrectionsCutOutsideCut
/// \brief Outside range cut class for Q vector correction
///
/// Provides support for cuts based in being outside a range
/// defined by a minimum value and a maximum value
///
/// Stores the threshold values and pass the var Id to its parent.
/// Implements IsSelected accordingly.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 22, 2016
class QnCorrectionsCutOutsideCut: public QnCorrectionsCutBase {

 public:
  QnCorrectionsCutOutsideCut();
  QnCorrectionsCutOutsideCut(const QnCorrectionsCutOutsideCut &cut);
  QnCorrectionsCutOutsideCut(Int_t varId, Float_t min, Float_t max);
  virtual ~QnCorrectionsCutOutsideCut();

  virtual Bool_t IsSelected(Float_t *variableContainer);
 private:
  Float_t         fMinThreshold;   ///< The lower limit
  Float_t         fMaxThreshold;   ///< The upper limit

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutOutsideCut, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual value is below the threshold else kFALSE
Bool_t QnCorrectionsCutOutsideCut::IsSelected(Float_t *variableContainer) {
  if ((fMinThreshold < variableContainer[fVarId]) && (variableContainer[fVarId] < fMaxThreshold))
    return kFALSE;
  else
    return kTRUE;
}

/// \class QnCorrectionsCutValueCut
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
class QnCorrectionsCutValueCut: public QnCorrectionsCutBase {

 public:
  QnCorrectionsCutValueCut();
  QnCorrectionsCutValueCut(const QnCorrectionsCutValueCut &cut);
  QnCorrectionsCutValueCut(Int_t varId, Float_t value);
  virtual ~QnCorrectionsCutValueCut();

  virtual Bool_t IsSelected(Float_t *variableContainer);
 private:
  Float_t         fValue;   ///< The desired value

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutValueCut, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual variable content is equal to the stored value else kFALSE
Bool_t QnCorrectionsCutValueCut::IsSelected(Float_t *variableContainer) {
  if (variableContainer[fVarId] != fValue)
    return kFALSE;
  else
    return kTRUE;
}

/// \class QnCorrectionsCutBitSetCut
/// \brief Bit setting cut class for Q vector correction
///
/// Provides support for cuts based in the setting or not
/// of a concrete bit within the actual value of a variable.
/// The selected bit is passed as an integer from 0 up to 31.
///
/// Stores the desired bit and its (un)set condition and pass
/// the var Id to its parent. Implements IsSelected accordingly.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 25, 2016
class QnCorrectionsCutBitSetCut: public QnCorrectionsCutBase {

 public:
  QnCorrectionsCutBitSetCut();
  QnCorrectionsCutBitSetCut(const QnCorrectionsCutBitSetCut &cut);
  QnCorrectionsCutBitSetCut(Int_t varId, Int_t bitNo, Bool_t set);
  virtual ~QnCorrectionsCutBitSetCut();

  virtual Bool_t IsSelected(Float_t *variableContainer);
 private:
  UInt_t          fBitMask;   ///< The mask to apply to the variable value
  UInt_t          fExpectedResult; ///< The expected masked result to pass the cut

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutBitSetCut, 1);
/// \endcond
};

/// Check if the actual variable value passes the cut
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual value is below the threshold else kFALSE
Bool_t QnCorrectionsCutBitSetCut::IsSelected(Float_t *variableContainer) {
  if ((UInt_t(variableContainer[fVarId]) & fBitMask) != fExpectedResult)
    return kFALSE;
  else
    return kTRUE;
}

/// \class QnCorrectionsCutsSet
/// \brief Set of cuts to assign to a detector
///
/// Array of cuts that contains the whole set of cuts to assign to
/// a concrete detector or detector configuration to filter the desired
/// events (in the broad sense) it will have to handle.
///
/// Inherits all the methods of TObjArray specially the
/// subscript [] operator and Add method that allows
/// the array to expand.
///
/// Provides IsSelected that goes through the whole set of cuts
/// to check whether the current variables values pass the them.
///
/// The cuts objects are not own by the array so,
/// they are not destroyed when the the set is destroyed. This allows
/// to create several sets with the same cuts.
/// Pay attention to this when you create your cuts,
/// they should live at least the same time you expect the sets to
/// live.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 4, 2016


class QnCorrectionsCutsSet : public TObjArray {
public:
  /// Normal constructor
  /// \param n number of cuts in the set
  QnCorrectionsCutsSet(Int_t n = TCollection::kInitCapacity) : TObjArray(n) {}
  /// Copy constructor
  /// \param ccs the object instance to be copied
  QnCorrectionsCutsSet(const QnCorrectionsCutsSet &ccs) : TObjArray(ccs) {}
  /// Default destructor
  virtual ~QnCorrectionsCutsSet() {}

  /// Access the event class variable at the passed position
  /// \param i position in the array (starting at zero)
  /// \return the event class variable object a position i
  virtual QnCorrectionsCutBase *At(Int_t i) const { return (QnCorrectionsCutBase *) TObjArray::At(i); }

  Bool_t IsSelected(Float_t *variableContainer);

/// \cond CLASSIMP
  ClassDef(QnCorrectionsCutsSet, 1);
/// \endcond
};

/// Checks that the current content of the variableContainer passes
/// the whole set of cuts by going through all the array components
///
/// \param variableContainer the current variables content addressed by var Id
/// \return kTRUE if the actual values pass the set of cuts else kFALSE
Bool_t QnCorrectionsCutsSet::IsSelected(Float_t *variableContainer) {
  for (Int_t icut = 0; icut < GetEntriesFast(); icut++) {
    if (!At(icut)->IsSelected(variableContainer)) {
      return kFALSE;
    }
  }
  return kTRUE;
}

#endif // QNCORRECTIONS_CUTS_H
