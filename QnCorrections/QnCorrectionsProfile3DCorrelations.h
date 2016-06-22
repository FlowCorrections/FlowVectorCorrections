#ifndef QNCORRECTIONS_PROFILE3DCORRELATIONS_H
#define QNCORRECTIONS_PROFILE3DCORRELATIONS_H

/// \file QnCorrectionsProfile3DCorrelations.h
/// \brief Three detector correlation components based set of profiles with harmonic support for the Q vector correction framework

#include "QnCorrectionsHistogramBase.h"

/// \class QnCorrectionsProfileCorrelationComponentsHarmonics
/// \brief Base class for the correlation components based set of profiles
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

  Bool_t CreateCorrelationComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
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

  virtual void Fill(QnCorrectionsQnVector *QnA, QnCorrectionsQnVector *QnB, QnCorrectionsQnVector *QnC, const Float_t *variableContainer);

private:
  THnF **fXXValues;            //!<! XX component histogram for each requested harmonic
  THnF **fXYValues;            //!<! XY component histogram for each requested harmonic
  THnF **fYXValues;            //!<! YX component histogram for each requested harmonic
  THnF **fYYValues;            //!<! YY component histogram for each requested harmonic
  UInt_t fXXharmonicFillMask;  //!<! keeps track of harmonic XX component filled values
  UInt_t fXYharmonicFillMask;  //!<! keeps track of harmonic XY component filled values
  UInt_t fYXharmonicFillMask;  //!<! keeps track of harmonic YX component filled values
  UInt_t fYYharmonicFillMask;  //!<! keeps track of harmonic YY component filled values
  UInt_t fFullFilled;          //!<! mask for the fully filled condition
  THnI  *fEntries;             //!<! Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfileCorrelationComponentsHarmonics, 1);
  /// \endcond
};

#endif
