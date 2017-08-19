#ifndef QNCORRECTIONS_PROFILECORRCOMP_H
#define QNCORRECTIONS_PROFILECORRCOMP_H

/// \file QnCorrectionsProfileCorrelationComponents.h
/// \brief Correlation components based set of profiles for the Q vector correction framework

#include "QnCorrectionsHistogramBase.h"

/// \class QnCorrectionsProfileCorrelationComponents
/// \brief Base class for the correlation components based set of profiles
///
/// Provides profile histograms for storing component, XX, XY, YX, YY, based
/// information.
///
/// As any histogram derived from QnCorrectionsHistogramBase the set
/// of variables that identify the different event classes has to
/// be passed to the constructor.
/// Of course,  the base name and base title for the different
/// histograms has also to be provided.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date May 08, 2016
class QnCorrectionsProfileCorrelationComponents : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfileCorrelationComponents();
  QnCorrectionsProfileCorrelationComponents(
      const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Option_t *option="");
  virtual ~QnCorrectionsProfileCorrelationComponents();

  Bool_t CreateCorrelationComponentsProfileHistograms(TList *histogramList);
  virtual Bool_t AttachHistograms(TList *histogramList);
  /// wrong call for this class invoke base class behavior
  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup)
  { return QnCorrectionsHistogramBase::AttachHistograms(histogramList, bUsedChannel, nChannelGroup); }

  virtual Long64_t GetBin(const Float_t *variableContainer);
  /// wrong call for this class invoke base class behavior
  virtual Long64_t GetBin(const Float_t *variableContainer, Int_t nChannel)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer, nChannel); }
  virtual Bool_t BinContentValidated(Long64_t bin);
  virtual Float_t GetXXBinContent(Long64_t bin);
  virtual Float_t GetXYBinContent(Long64_t bin);
  virtual Float_t GetYXBinContent(Long64_t bin);
  virtual Float_t GetYYBinContent(Long64_t bin);
  virtual Float_t GetXXBinError(Long64_t bin);
  virtual Float_t GetXYBinError(Long64_t bin);
  virtual Float_t GetYXBinError(Long64_t bin);
  virtual Float_t GetYYBinError(Long64_t bin);

  virtual void FillXX(const Float_t *variableContainer, Float_t weight);
  virtual void FillXY(const Float_t *variableContainer, Float_t weight);
  virtual void FillYX(const Float_t *variableContainer, Float_t weight);
  virtual void FillYY(const Float_t *variableContainer, Float_t weight);

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
  virtual void FillXX(Int_t harmonic, const Float_t *variableContainer, Float_t weight)
  { return QnCorrectionsHistogramBase::FillXX(harmonic, variableContainer, weight); }
  /// wrong call for this class invoke base class behavior
  virtual void FillXY(Int_t harmonic, const Float_t *variableContainer, Float_t weight)
  { return QnCorrectionsHistogramBase::FillXY(harmonic, variableContainer, weight); }
  /// wrong call for this class invoke base class behavior
  virtual void FillYX(Int_t harmonic, const Float_t *variableContainer, Float_t weight)
  { return QnCorrectionsHistogramBase::FillYX(harmonic, variableContainer, weight); }
  /// wrong call for this class invoke base class behavior
  virtual void FillYY(Int_t harmonic, const Float_t *variableContainer, Float_t weight)
  { return QnCorrectionsHistogramBase::FillYY(harmonic, variableContainer, weight); }

private:
  THnF *fXXValues;            //!<! XX component histogram
  THnF *fXYValues;            //!<! XY component histogram
  THnF *fYXValues;            //!<! YX component histogram
  THnF *fYYValues;            //!<! YY component histogram
  UInt_t fXXXYYXYYFillMask;   //!<! keeps track of component filled values
  UInt_t fFullFilled;          //!<! mask for the fully filled condition
  THnI  *fEntries;             //!<! Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfileCorrelationComponents, 1);
  /// \endcond
};

#endif
