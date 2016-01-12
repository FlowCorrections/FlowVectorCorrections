#ifndef QNCORRECTIONS_HISTOGRAMS_H
#define QNCORRECTIONS_HISTOGRAMS_H

/// \file QnCorrectionsHistograms.h
/// \brief Classes that model the multidimensional profile histograms for the Q vector correction framework

#include "THn.h"

class QnCorrectionsEventClassVariablesSet;

/// \class QnCorrectionsHistogramBase
/// \brief Base class for the Q vector correction histograms
///
/// Basically stores the set of variables that identify
/// the different event classes the involved histograms
/// are storing information about. It also stores (in its
/// parent) the base name and base title for the different
/// histograms it will encapsulate.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 11, 2016
class QnCorrectionsHistogramBase : public TNamed {
public:
  QnCorrectionsHistogramBase();
  QnCorrectionsHistogramBase(const char *name, const char *title, QnCorrectionsEventClassVariablesSet &ecvs);
  virtual ~QnCorrectionsHistogramBase();


  virtual Int_t GetBin(Float_t *variableContainer) { return -1; }
  virtual Float_t GetBinContent(Int_t bin) { return 0.0; }
  virtual Float_t GetBinError(Int_t bin) {return 0.0; }
  virtual void Fill(Float_t *varableContainer, Float_t weight, Int_t nEntries = 1) {}
private:
  QnCorrectionsEventClassVariablesSet fEventClassVariables;  ///< The variables set that determines the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsHistogramBase, 1);
  /// \endcond
  static const char *szEntriesHistoSuffix;
};

/// \class QnCorrectionsProfile
/// \brief Single profile class for the Q vector correction histograms
///
/// Encapsulates a multi dimensional profile. Each dimension
/// corresponds to one of the event classes variables so,
/// the number of dimensions matches the number of variables within
/// the set passed in the constructor.
///
/// The involved histograms can be created on the fly when needed,
/// and included in a provided list, or attached to existing ones
/// from a given list. In any case they are not destroyed because
/// the are not own by the class but by the involved list.
///
/// The GetBinContent (once the intended bin is obtained by mean
/// of GetBin) returns in the profile way
///    \f$ \frac{fValues(bin)}{fEntries(bin)} \f$
/// and the same happens for GetBinError
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 11, 2016
class QnCorrectionsProfile : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfile();
  QnCorrectionsProfile(const char *name, const char *title, QnCorrectionsEventClassVariablesSet &ecvs);
  virtual ~QnCorrectionsProfile();

  Bool_t CreateProfileHistograms(TList *histogramList);
  Bool_t AttachProfileHistograms(TList *histogramList);

  Int_t GetBin(Float_t *variableContainer);
  Float_t GetBinContent(Int_t bin);
  Float_t GetBinError(Int_t bin);
  void Fill(Float_t *varableContainer, Float_t weight, Int_t nEntries = 1);
private:
  THnF *fValues;   ///< Cumulates values for each of the event classes
  THnI *fEntries;  ///< Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfile, 1);
  /// \endcond
};

#endif
