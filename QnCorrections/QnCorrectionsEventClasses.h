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
/// \class QnCorrectionEventClassVariable
/// \brief One variable used for defining an event class
///
/// Class defining one variable and its associated binning allowing to
/// using it for the definition of event classes within the Q vector
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

  // setters
  void SetVariable(Int_t varId, const char *varname) { fVarId = varId; fLabel = varname; }
  void SetBins(Int_t nbins, Double_t min, Double_t max);
  void SetBins(Int_t nbins, Double_t *bins);
  void SetLabel(const char *label) { fLabel = label; }

  // getters
  Int_t           GetVariableId() const { return fVarId; }
  const char *    GetVariableLabel() const { return (const char *) fLabel; }
  Int_t           GetNBins() const { return fNBins; }
  const Double_t *GetBins() const { return fBins; }
  Double_t        GetBinEdge(Int_t bin) const { return fBins[bin]; }

  Double_t        GetLowerEdge() {return fBins[0]; }
  Double_t        GetUpperEdge() {return fBins[fNBins]; }

 private:
  Int_t         fVarId;   ///< The external Id for the variable in the data bank
  Int_t         fNBins;   ///< The number of bins for the variable when shown in a histogram
  Double_t     *fBins;    ///< Bin edges array for the variable when shown in a histogram
  TString       fLabel;   ///< Label to use in an axis that shows the variable

  ClassDef(QnCorrectionsEventClassVariable, 1);
};

/// \class QnCorrectionEventClassVariablesSet
/// \brief The set of variables that define an event class
///
/// Array of EventClassVariables that fully define the different
/// event classes considered within the Q vector correction framework.
/// The objects of this class are associated to concrete
/// detectors or detector configurations in such a way that
/// all Q vector corrections are performed according to the
/// event class the involved event is allocated.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 4, 2016


class QnCorrectionsEventClassVariablesSet : public TObjArray {
public:
  QnCorrectionsEventClassVariablesSet(const QnCorrectionsEventClassVariablesSet& a) : TObjArray(a) {}
  QnCorrectionsEventClassVariablesSet(Int_t s = TCollection::kInitCapacity, Int_t lowerBound = 0) : TObjArray(s, lowerBound) {}
  virtual ~QnCorrectionsEventClassVariablesSet() {}

  virtual QnCorrectionsEventClassVariable *At(Int_t i) const { return (QnCorrectionsEventClassVariable *) ((TObjArray *) this)->At(i); }

  ClassDef(QnCorrectionsEventClassVariablesSet, 1);
};

#endif
