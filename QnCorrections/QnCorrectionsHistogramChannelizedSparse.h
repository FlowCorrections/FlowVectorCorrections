#ifndef QNCORRECTIONS_HISTOGRAMSCHANNELSPARSE_H
#define QNCORRECTIONS_HISTOGRAMSCHANNELSPARSE_H

/// \file QnCorrectionsHistogramChannelizedSparse.h
/// \brief Single multidimensional sparse histograms with channel support

#include "QnCorrectionsHistogramBase.h"
#include <THnSparse.h>

/// \class QnCorrectionsHistogramChannelizedSparse
/// \brief Single histogram class for the Q vector correction histograms
///
/// Encapsulates a multi dimensional sparse histogram. Each dimension
/// corresponds to one of the event classes variables so,
/// the number of dimensions matches the number of variables within
/// the set passed in the constructor. Additionally incorporates an
/// extra dimension to consider the channel number
///
/// The involved histograms can be created on the fly when needed,
/// and included in a provided list. They are not destroyed because
/// the are not own by the class but by the involved list.
///
/// Storage efficiency reasons dictate that channels were stored in
/// consecutive order although externally to the class everything is
/// handled with the actual external channel number. But if the
/// histograms stored in a file are drawn the channels will appear
/// as enumerated form 0 to the number of active channels handled
/// by the detector configuration that is associated to the histogram
/// and as such by the own histogram (this is a ROOT bug).
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jun 18, 2016
class QnCorrectionsHistogramChannelizedSparse : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsHistogramChannelizedSparse();
  QnCorrectionsHistogramChannelizedSparse(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Int_t nNoOfChannels);
  virtual ~QnCorrectionsHistogramChannelizedSparse();

  Bool_t CreateChannelizedHistogram(TList *histogramList, const Bool_t *bUsedChannel);

  /// wrong call for this class invoke base class behaviour
  virtual Long64_t GetBin(const Float_t *variableContainer)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer); }
  virtual Long64_t GetBin(const Float_t *variableContainer, Int_t nChannel);
  virtual Bool_t BinContentValidated(Long64_t bin);
  virtual Float_t GetBinContent(Long64_t bin);
  virtual Float_t GetBinError(Long64_t bin);

  /// wrong call for this class invoke base class behavior
  virtual void Fill(const Float_t *variableContainer, Float_t weight)
  { QnCorrectionsHistogramBase::Fill(variableContainer, weight); }
  virtual void Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight);
private:
  THnSparseF *fValues;              //!<! Cumulates values for each of the event classes
  Bool_t *fUsedChannel;       //!<! array, which of the detector channels is used for this configuration
  Int_t fNoOfChannels;        //!<! The number of channels associated to the whole detector
  Int_t fActualNoOfChannels;  //!<! The actual number of channels handled by the histogram
  Int_t *fChannelMap;         //!<! array, the map from histo to detector channel number

  /// \cond CLASSIMP
  ClassDef(QnCorrectionsHistogramChannelizedSparse, 1);
  /// \endcond
};

#endif
