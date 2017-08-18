#ifndef QNCORRECTIONS_PROFILE3DCORRELATIONS_H
#define QNCORRECTIONS_PROFILE3DCORRELATIONS_H

/// \file QnCorrectionsProfile3DCorrelations.h
/// \brief Three detector correlation components based set of profiles with harmonic support for the Q vector correction framework

#include "QnCorrectionsHistogramBase.h"

class QnCorrectionsQnVector;

/// \class QnCorrectionsProfile3DCorrelations
/// \brief Base class for three detectors correlation components based set of profiles with harmonic support
///
/// Provides profile histograms for storing component, XX, XY, YX, YY, based
/// information for a set of predefined harmonics defined at creation
/// time and for a set of three different subdetectors generically identified
/// as A, B and C. The user can select the harmonic addressing procedure so that
/// it will possible to ask for just one harmonic support and assign
/// to it any desired number.
///
/// As any histogram derived from QnCorrectionsHistogramBase the set
/// of variables that identify the different event classes has to
/// be passed to the constructor together with the required number of
/// harmonics and an optional harmonic expected numbering scheme.
/// Of course,  the base name and base title for the different
/// histograms has also to be provided.
///
/// The harmonic map passed should contain an ordered array with
/// as many items as requested harmonics that provides the external
/// number to be used for request the corresponding harmonic.
/// Requesting five harmonics without maps is equivalent to pass
/// {1,2,3,4,5} as map. Requesting just support for the harmonic
/// four will require a map {4}.
///
/// Externally the harmonic number is addressed as usual. An additional
/// harmonic multiplier field allows to handle mxn harmonics. n is always
/// the external harmonic required internally it si handled as well as
/// n but all the information manipulated is really associated to mxn.
/// Only in the histograms name it appears the proper mxn harmonic to
/// not confuse the external user which browse the histograms.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 19, 2016
class QnCorrectionsProfile3DCorrelations : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfile3DCorrelations();
  QnCorrectionsProfile3DCorrelations(
      const char *name,
      const char *title,
      const char *nameA,
      const char *nameB,
      const char *nameC,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Option_t *option="");
  virtual ~QnCorrectionsProfile3DCorrelations();

  Bool_t CreateCorrelationComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t nHarmonicMultiplier = 1, Int_t *harmonicMap = NULL);
  virtual Bool_t AttachHistograms(TList *histogramList);
  /// wrong call for this class invoke base class behavior
  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup)
  { return QnCorrectionsHistogramBase::AttachHistograms(histogramList, bUsedChannel, nChannelGroup); }

  virtual Long64_t GetBin(const Float_t *variableContainer);
  /// wrong call for this class invoke base class behavior
  virtual Long64_t GetBin(const Float_t *variableContainer, Int_t nChannel)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer, nChannel); }
  virtual Bool_t BinContentValidated(Long64_t bin);
  virtual Float_t GetXXBinContent(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetXYBinContent(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetYXBinContent(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetYYBinContent(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetXXBinError(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetXYBinError(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetYXBinError(const char *comb, Int_t harmonic, Long64_t bin);
  virtual Float_t GetYYBinError(const char *comb, Int_t harmonic, Long64_t bin);

  void Fill(const QnCorrectionsQnVector *QnA,
      const QnCorrectionsQnVector *QnB,
      const QnCorrectionsQnVector *QnC,
      const Float_t *variableContainer);

  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXXBinContent(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXXBinContent(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXYBinContent(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXYBinContent(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYXBinContent(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYXBinContent(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYYBinContent(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYYBinContent(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXXBinError(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXXBinError(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXYBinError(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXYBinError(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYXBinError(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYXBinError(bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYYBinError(Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYYBinError(bin); }

  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXXBinContent(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXXBinContent(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXYBinContent(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXYBinContent(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYXBinContent(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYXBinContent(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYYBinContent(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYYBinContent(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXXBinError(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXXBinError(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetXYBinError(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetXYBinError(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYXBinError(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYXBinError(harmonic, bin); }
  /// wrong call for this class invoke base class behavior
  virtual Float_t GetYYBinError(Int_t harmonic, Long64_t bin)
  { return QnCorrectionsHistogramBase::GetYYBinError(harmonic, bin); }

  /// wrong call for this class invoke base class behavior
  virtual void Fill(const Float_t *variableContainer, Float_t weight)
  { QnCorrectionsHistogramBase::Fill(variableContainer, weight); }
  /// wrong call for this class invoke base class behavior
  virtual void Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight)
  { QnCorrectionsHistogramBase::Fill(variableContainer, nChannel, weight); }


private:
  THnF ***fXXValues;            //!<! XX component histogram for each requested harmonic
  THnF ***fXYValues;            //!<! XY component histogram for each requested harmonic
  THnF ***fYXValues;            //!<! YX component histogram for each requested harmonic
  THnF ***fYYValues;            //!<! YY component histogram for each requested harmonic
  THnI  *fEntries;             //!<! Cumulates the number on each of the event classes
  TString fNameA;               ///< the name of the A detector
  TString fNameB;               ///< the name of the B detector
  TString fNameC;               ///< the name of the C detector
  Int_t fHarmonicMultiplier;    ///< the multiplier for the harmonic number
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfile3DCorrelations, 1);
  /// \endcond
};

#endif
