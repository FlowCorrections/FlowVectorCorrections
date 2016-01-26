#ifndef QNCORRECTIONS_EVENTCLASS_H
#define QNCORRECTIONS_EVENTCLASS_H
/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsEventClasses.h
/// \brief Classes that model the event classes for the Q vector correction framework

/// \class QnCorrectionsEventClassVariable
/// \brief One variable used for defining an event class
///
/// Class defining one variable and its associated binning allowing
/// its use for the definition of event classes within the Q vector
/// correction framework.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 4, 2016
 

#include <TObject.h>
#include <TObjArray.h>


class QnCorrectionsEventClassVariable : public TObject {

 public:
  QnCorrectionsEventClassVariable();
  QnCorrectionsEventClassVariable(const QnCorrectionsEventClassVariable &ecv);
  QnCorrectionsEventClassVariable(Int_t varId, const char *varname, Int_t nbins, Double_t min, Double_t max);
  QnCorrectionsEventClassVariable(Int_t varId, const char *varname, Int_t nbins, Double_t *bins);
  QnCorrectionsEventClassVariable(Int_t varId, const char *varname, Double_t binsArray[][2]);
  ~QnCorrectionsEventClassVariable();

  /// Gets the variable unique Id
  Int_t           GetVariableId() const { return fVarId; }
  /// Gets the variable name / label
  const char *    GetVariableLabel() const { return (const char *) fLabel; }
  /// Gets the number of bins
  Int_t           GetNBins() const { return fNBins; }
  /// Gets the actual bins edges array
  const Double_t *GetBins() const { return fBins; }
  /// Gets the lower edge for the passed bin number
  /// \param bin bin number starting from one
  Double_t        GetBinLowerEdge(Int_t bin) const { return (((bin < 1) || (bin > fNBins)) ? 0.0 : fBins[bin-1]); }
  /// Gets the upper edge for the passed bin number
  /// \param bin bin number starting from one
  Double_t        GetBinUpperEdge(Int_t bin) const { return (((bin < 1) || (bin > fNBins)) ? 0.0 : fBins[bin]); }

  /// Gets the lowest variable value considered
  Double_t        GetLowerEdge() {return fBins[0]; }
  /// Gets the highest variabel value considered
  Double_t        GetUpperEdge() {return fBins[fNBins]; }

 private:
  Int_t         fVarId;   ///< The external Id for the variable in the data bank
  Int_t         fNBins;   ///< The number of bins for the variable when shown in a histogram
  Double_t     *fBins;    ///< Bin edges array for the variable when shown in a histogram
  TString       fLabel;   ///< Label to use in an axis that shows the variable

/// \cond CLASSIMP
  ClassDef(QnCorrectionsEventClassVariable, 1);
/// \endcond
};

/// \class QnCorrectionsEventClassVariablesSet
/// \brief The set of variables that define an event class
///
/// Array of EventClassVariables that fully define the different
/// event classes considered within the Q vector correction framework.
/// The objects of this class are associated to concrete
/// detectors or detector configurations in such a way that
/// all Q vector corrections are performed according to the
/// event class the involved event is allocated.
///
/// Inherits all the methods of TObjArray specially the
/// subscript [] operator and Add method that allows
/// the array to expand.
///
/// The event class variables objects are not own by the array so,
/// they are not destroyed when the the set is destroyed. This allows
/// to create several sets with the same event class variables.
/// Pay attention to this when you create your event class variables,
/// they should live at least the same time you expect the sets to
/// live.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 4, 2016


class QnCorrectionsEventClassVariablesSet : public TObjArray {
public:
  /// Normal constructor
  /// \param n number of variables in the set
  QnCorrectionsEventClassVariablesSet(Int_t n = TCollection::kInitCapacity) : TObjArray(n) {}
  /// Copy constructor
  /// \param cecvs the object instance to be copied
  QnCorrectionsEventClassVariablesSet(const QnCorrectionsEventClassVariablesSet &cecvs) : TObjArray(cecvs) {}
  /// Default destructor
  virtual ~QnCorrectionsEventClassVariablesSet() {}

  /// Access the event class variable at the passed position
  /// \param i position in the array (starting at zero)
  /// \return the event class variable object a position i
  virtual QnCorrectionsEventClassVariable *At(Int_t i) const { return (QnCorrectionsEventClassVariable *) TObjArray::At(i); }

  void GetMultidimensionalConfiguration(Int_t *nbins, Double_t *minvals, Double_t *maxvals);

/// \cond CLASSIMP
  ClassDef(QnCorrectionsEventClassVariablesSet, 1);
/// \endcond
};

#endif
