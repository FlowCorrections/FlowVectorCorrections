/// \file QnCorrectionsHistograms.cxx
/// \brief Implementation of QnCorrectionsHistograms classes

#include "TList.h"

#include "QnCorrectionsEventClasses.h"
#include "QnCorrectionsHistograms.h"
#include "QnCorrectionsLog.h"

/// The suffix for the name of the entries histograms
const char *QnCorrectionsHistogramBase::szChannelAxisTitle = "Channel number";
///< The title for the channel group extra axis
const char *QnCorrectionsHistogramBase::szGroupAxisTitle = "Channels group";
///< The prefix for the name of the group histograms
const char *QnCorrectionsHistogramBase::szGroupHistoPrefix = "Group";
const char *QnCorrectionsHistogramBase::szEntriesHistoSuffix = "_entries";
const char *QnCorrectionsHistogramBase::szXComponentSuffix = "X";
const char *QnCorrectionsHistogramBase::szYComponentSuffix = "Y";
const char *QnCorrectionsHistogramBase::szXXCorrelationComponentSuffix = "XX";
const char *QnCorrectionsHistogramBase::szXYCorrelationComponentSuffix = "XY";
const char *QnCorrectionsHistogramBase::szYXCorrelationComponentSuffix = "YX";
const char *QnCorrectionsHistogramBase::szYYCorrelationComponentSuffix = "YY";
const Int_t QnCorrectionsHistogramBase::nMaxHarmonicNumberSupported = 15;
const UInt_t QnCorrectionsHistogramBase::harmonicNumberMask[] =
  {0x0000,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
   0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
///< The minimum number of entries for validating a bin content
const Int_t QnCorrectionsHistogramBase::nMinNoOfEntriesValidated = 2;

/// \cond CLASSIMP
ClassImp(QnCorrectionsHistogramBase);
/// \endcond

/// Default constructor
QnCorrectionsHistogramBase::QnCorrectionsHistogramBase() :
  TNamed(),
  fEventClassVariables(),
  fBinAxesValues(NULL) {

  fErrorMode = kERRORMEAN;
}

/// Default destructor
///
/// restores the taken memory for the bin axes values bank
QnCorrectionsHistogramBase::~QnCorrectionsHistogramBase() {
  if (fBinAxesValues != NULL)
    delete [] fBinAxesValues;
}

/// Normal constructor
///
/// Basically stores the set of variables that identify
/// the different event classes the involved histograms
/// are storing information about
///
/// This is the base class. For simplicity and consistency
/// we leave open an extra variable storage that will be
/// used by the channelized histograms.
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
/// \param option option for errors computation
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
QnCorrectionsHistogramBase::QnCorrectionsHistogramBase(const char *name,
    const char *title,
    QnCorrectionsEventClassVariablesSet &ecvs,
    Option_t *option) :
  TNamed(name, title),
  fEventClassVariables(ecvs),
  fBinAxesValues(NULL) {

  /* one place more for storing the channel number by inherited classes */
  fBinAxesValues = new Double_t[fEventClassVariables.GetEntries() + 1];

  TString opt = option;
  opt.ToLower();
  fErrorMode = kERRORMEAN;
  if (opt.Contains("s")) fErrorMode = kERRORSPREAD;
}

/// Attaches existing histograms as the supporting histograms
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
///
/// \param histogramList list where the histograms have to be located
/// \return the associated bin to the current variables content
Bool_t QnCorrectionsHistogramBase::AttachHistograms(TList *histogramList) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a channelized profile without passing channels information. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::AttachHistograms()"));
  return kFALSE;
}

/// Attaches existing histograms as the support histograms for the profile function
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
///
/// \param histogramList list where the histograms have to be located
/// \param bUsedChannel array of booleans one per each channel
/// \param nChannelGroup array of group number for each channel
/// \return true if properly attached else false
Bool_t QnCorrectionsHistogramBase::AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a non channelized profile but passing channels information. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::AttachHistograms()"));
  return kFALSE;
}


/// Get the bin number for the current variable content
///
/// The bin number identifies the event class the current
/// variable content points to.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param variableContainer the current variables content addressed by var Id
/// \return the associated bin to the current variables content
Int_t QnCorrectionsHistogramBase::GetBin(const Float_t *variableContainer) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a channelized profile without passing the channel number. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetBin()"));
  return -1;
}

/// Get the bin number for the current variable content and channel number
///
/// The bin number identifies the event class the current
/// variable content points to and the passed channel
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param nChannel the interested external channel number
/// \return the associated bin to the current variables content
Int_t QnCorrectionsHistogramBase::GetBin(const Float_t *variableContainer, Int_t nChannel) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means you have instantiated a base class or\n" \
      "you are using a non channelized profile passing a channel number. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetBin()"));
  return -1;
}

/// Get the bin content for the passed bin number
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetBinContent(Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetXBinContent or GetYBinContent, or GetXXBinContent ... GetYYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetBinContent()"));
  return 0.0;
}

/// Get the X component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetXBinContent(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinContent, or GetXXBinContent ... GetYYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetXBinContent()"));
  return 0.0;
}

/// Get the Y component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetYBinContent(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinContent, or GetXXBinContent ... GetYYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetYBinContent()"));
  return 0.0;
}

/// Get the bin XX component content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetXXBinContent(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinContent, or GetXBinContent or GetYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetXXBinContent()"));
  return 0.0;
}

/// Get the bin XY component content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetXYBinContent(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinContent, or GetXBinContent or GetYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetXYBinContent()"));
  return 0.0;
}

/// Get the bin YX component content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetYXBinContent(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinContent, or GetXBinContent or GetYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetYXBinContent()"));
  return 0.0;
}

/// Get the bin YY component content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsHistogramBase::GetYYBinContent(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinContent, or GetXBinContent or GetYBinContent, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetYYBinContent()"));
  return 0.0;
}

/// Get the bin content error for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param bin the interested bin number
/// \return the bin number content error
Float_t QnCorrectionsHistogramBase::GetBinError(Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetXBinError or GetYBinError, or GetXXBinError ... GetYYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetBinError()"));
  return 0.0;
}

/// Get the X component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsHistogramBase::GetXBinError(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinError, or GetXXBinError ... GetYYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetXBinError()"));
  return 0.0;
}

/// Get the Y component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsHistogramBase::GetYBinError(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinError, or GetXXBinError ... GetYYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetYBinError()"));
  return 0.0;
}

/// Get the XX component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsHistogramBase::GetXXBinError(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinError, or GetXBinError or GetYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetXXBinError()"));
  return 0.0;
}

/// Get the XY component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsHistogramBase::GetXYBinError(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinError, or GetXBinError or GetYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetXYBinError()"));
  return 0.0;
}

/// Get the YX component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsHistogramBase::GetYXBinError(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinError, or GetXBinError or GetYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetYXBinError()"));
  return 0.0;
}

/// Get the YY component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsHistogramBase::GetYYBinError(Int_t harmonic, Int_t bin) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   GetBinError, or GetXBinError or GetYBinError, or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::GetYYBinError()"));
  return 0.0;
}

/// Fills the histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::Fill(const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   FillX or FillY, or FillXX ... FillYY or you have instantiated a base class or you are using\n" \
      "a channelized profile without passing a channel number. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::Fill()"));
}

/// Fills the histogram
///
/// The involved bin is computed according to the current variables
/// content and passed channel number. The bin is then increased by the given
/// weight and the entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param nChannel the interested external channel number
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   FillX or FillY, or FillXX ... FillYY or you have instantiated a base class or you are using\n" \
      "a non channelized profile passing a channel number. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::Fill()"));
}

/// Fills the X component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::FillX(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   Fill or FillXX ... FillYY or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::FillX()"));
}

/// Fills the Y component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::FillY(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   Fill or FillXX ... FillYY or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::FillY()"));
}

/// Fills the XX component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::FillXX(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   Fill, FillX or FillY or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::FillXX()"));
}

/// Fills the XY component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::FillXY(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   Fill, FillX or FillY or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::FillXY()"));
}

/// Fills the YX component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::FillYX(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   Fill, FillX or FillY or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::FillYX()"));
}

/// Fills the YY component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// Interface declaration function.
/// Default behavior. Base class should not be instantiated.
/// Run time error to support debugging.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsHistogramBase::FillYY(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  QnCorrectionsFatal(Form("You have reached base member %s. This means either you should have used\n" \
      "   Fill, FillX or FillY or you have instantiated a base class. FIX IT, PLEASE.",
      "QnCorrectionsHistogramBase::FillYY()"));
}

/// Divide two THn histograms
///
/// Creates a value / error multidimensional histogram from
/// a values and entries multidimensional histograms
/// \param hValues the values multidimensional histogram
/// \param hEntries the entries multidimensional histogram
/// \return the values / error multidimensional histogram
THnF* QnCorrectionsHistogramBase::DivideTHnF(THnF *hValues, THnI *hEntries) {

  THnF *hResult =  (THnF*) THn::CreateHn(hValues->GetName(), hValues->GetTitle(), hValues);

  Double_t value, error2;
  Int_t nEntries;
  Bool_t bErrorMessage = kFALSE;

  for (Int_t bin = 0; bin < hResult->GetNbins(); bin++){
    value = hValues->GetBinContent(bin);
    nEntries = hEntries->GetBinContent(bin);
    error2 = hValues->GetBinError2(bin);

    if (nEntries < nMinNoOfEntriesValidated) {
      /* bin content not validated */
      hResult->SetBinContent(bin, 0.0);
      hResult->SetBinError(bin, 0.0);
      bErrorMessage = kTRUE;
    }
    else {
      Double_t average = value / nEntries;
      Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
      switch (fErrorMode) {
      case kERRORMEAN:
        /* standard error on the mean of the bin values */
        hResult->SetBinContent(bin, average);
        hResult->SetBinError(bin, serror / TMath::Sqrt(nEntries));
        break;
      case kERRORSPREAD:
        /* standard deviation of the bin values */
        hResult->SetBinContent(bin, average);
        hResult->SetBinError(bin, serror);
        break;
      }
    }
    hResult->SetEntries(hValues->GetEntries());
  }
  if (bErrorMessage) {
    QnCorrectionsError(Form("There are bins whose bin content were not validated! histogram: %s.\n" \
        "   Minimum number of entries to validate: %d.",
        hValues->GetName(),
        nMinNoOfEntriesValidated));
  }
  return hResult;
}

/// Starts the copy of two THnF histograms.
///
/// Source should not have channel/group structure (axis)
/// while dest should. The channel/group involved in dest
/// should be stored in binsArray[nVariables].
/// \param hDest the histogram that will receive the copy
/// \param hSource the histogram to copy
/// \param binsArray the array to build the bin numbers on each dimension
void QnCorrectionsHistogramBase::CopyTHnF(THnF *hDest, THnF *hSource, Int_t *binsArray) {

  CopyTHnFDimension(hDest, hSource, binsArray, 0);
}

/// Process a dimension in the copy of two THnF histograms process.
///
/// Source should not have channel/group structure (axis)
/// while dest should. The channel/group involved in dest
/// should be stored in binsArray[nVariables].
/// It is called in a recursive way until the number of
/// event class variables (dimensions in the source) is
/// exhausted then, the corresponding bins are copied.
/// \param hDest the histogram that will receive the copy
/// \param hSource the histogram to copy
/// \param binsArray the array to build the bin numbers on each dimension
/// \param dimension the current dimension being handled
void QnCorrectionsHistogramBase::CopyTHnFDimension(THnF *hDest, THnF *hSource, Int_t *binsArray, Int_t dimension) {

  /* are all variables settled */
  if (dimension < fEventClassVariables.GetEntriesFast()) {
    /* no then, scan this dimension and move to the next one */
    for (Int_t bin = 0; bin < hSource->GetAxis(dimension)->GetNbins(); bin++) {
      binsArray[dimension] = bin + 1;
      CopyTHnFDimension(hDest, hSource, binsArray, dimension +1);
    }
  }
  else {
    /* all variables have a new bin configuration, let's do the actual copy */
    Double_t value = hSource->GetBinContent(binsArray);
    Double_t error = hSource->GetBinError(binsArray);
    hDest->SetBinContent(binsArray, value);
    hDest->SetBinError(binsArray,error);
  }
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsProfile);
/// \endcond

/// Default constructor
QnCorrectionsProfile::QnCorrectionsProfile(): QnCorrectionsHistogramBase() {
  fValues = NULL;
  fEntries = NULL;
}

/// Normal constructor
///
/// Stores the set of variables that identify the
/// different event classes passing them to its parent
/// and prepares the object for actual histogram
/// creation or attachment
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
/// \param option option for errors computation
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
QnCorrectionsProfile::QnCorrectionsProfile(const char *name,
    const char *title,
    QnCorrectionsEventClassVariablesSet &ecvs,
    Option_t *option):
    QnCorrectionsHistogramBase(name, title, ecvs, option) {
  fValues = NULL;
  fEntries = NULL;
}

/// Default destructor
///
/// Does nothing because none of the members are own at destruction time
QnCorrectionsProfile::~QnCorrectionsProfile() {

}

/// Creates the support histograms for the profile function
///
/// Based in the event classes variables set in the parent class
/// the values and entries multidimensional histograms are
/// created.
///
/// Both histograms are added to the passed histogram list
///
/// \param histogramList list where the histograms have to be added
/// \return true if properly created
Bool_t QnCorrectionsProfile::CreateProfileHistograms(TList *histogramList) {
  /* let's build the histograms names and titles */
  TString histoName = GetName();
  TString histoTitle = GetTitle();
  TString entriesHistoName = GetName(); entriesHistoName += szEntriesHistoSuffix;
  TString entriesHistoTitle = GetTitle(); entriesHistoTitle += szEntriesHistoSuffix;

  Int_t nVariables = fEventClassVariables.GetEntriesFast();

  Double_t *minvals = new Double_t[nVariables];
  Double_t *maxvals = new Double_t[nVariables];
  Int_t *nbins = new Int_t[nVariables];

  /* get the multidimensional structure */
  fEventClassVariables.GetMultidimensionalConfiguration(nbins,minvals,maxvals);

  /* create the values and entries multidimensional histograms */
  fValues = new THnF((const char *) histoName, (const char *) histoTitle,nVariables,nbins,minvals,maxvals);
  fEntries = new THnI((const char *) entriesHistoName, (const char *) entriesHistoTitle,nVariables,nbins,minvals,maxvals);

  /* now let's set the proper binning and label on each axis */
  for (Int_t var = 0; var < nVariables; var++) {
    fValues->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fEntries->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fValues->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
    fEntries->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
  }

  fValues->Sumw2();

  histogramList->Add(fValues);
  histogramList->Add(fEntries);

  delete [] minvals;
  delete [] maxvals;
  delete [] nbins;

  return kTRUE;
}

/// Attaches existing histograms as the support histograms for the profile function
///
/// The histograms are located in the passed list and if found and with the
/// proper dimensions their references are stored in member variables.
///
/// \param histogramList list where the histograms have to be located
/// \return true if properly attached else false
Bool_t QnCorrectionsProfile::AttachHistograms(TList *histogramList) {
  /* let's build the histograms names */
  TString histoName = GetName();
  TString entriesHistoName = GetName(); entriesHistoName += szEntriesHistoSuffix;

  /* initialize. Remember we don't own the histograms */
  fEntries = NULL;
  fValues = NULL;

  fEntries = (THnI *) histogramList->FindObject((const char*) entriesHistoName);
  if (fEntries != NULL) {
    fValues = (THnF *) histogramList->FindObject((const char *)histoName);
    if (fValues == NULL)
      return kFALSE;
  }
  else
    return kFALSE;

/* TODO: pending to decide whether we divide the histograms and modify the
 * get content and get errors functions accordingly or we split the class
 * in created histograms and attached histograms. So far we leave it as
 * it is and we'll see the penalty for not having them divided.
 */
  return kTRUE;
}

/// Get the bin number for the current variable content
///
/// The bin number identifies the event class the current
/// variable content points to.
///
/// \param variableContainer the current variables content addressed by var Id
/// \return the associated bin to the current variables content
Int_t QnCorrectionsProfile::GetBin(const Float_t *variableContainer) {
  FillBinAxesValues(variableContainer);
  return fEntries->GetBin(fBinAxesValues);
}

/// Get the bin content for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfile::GetBinContent(Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fValues->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the bin content error for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param bin the interested bin number
/// \return the bin number content error
Float_t QnCorrectionsProfile::GetBinError(Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);
  Float_t values = fValues->GetBinContent(bin);
  Float_t error2 = fValues->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Fills the histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight and the
/// entries also increased properly.
///
/// \param variableContainer the current variables conten addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfile::Fill(const Float_t *variableContainer, Float_t weight) {
  /* keep the total entries in fValues updated */
  Double_t nEntries = fValues->GetEntries();

  FillBinAxesValues(variableContainer);
  fValues->Fill(fBinAxesValues, weight);
  fValues->SetEntries(nEntries + 1);
  fEntries->Fill(fBinAxesValues, 1.0);
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsProfileChannelized);
/// \endcond

/// Default constructor
QnCorrectionsProfileChannelized::QnCorrectionsProfileChannelized() :
    QnCorrectionsHistogramBase() {

  fValues = NULL;
  fEntries = NULL;
  fUsedChannel = NULL;
  fChannelGroup = NULL;
  fNoOfChannels = 0;
  fActualNoOfChannels = 0;
  fChannelMap = NULL;
}

/// Normal constructor
///
/// Stores the set of variables that identify the
/// different event classes passing them to its parent
/// and prepares the object for actual histogram
/// creation or attachment
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
/// \param nNoOfChannels the number of channels associated
/// \param option option for errors computation
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
QnCorrectionsProfileChannelized::QnCorrectionsProfileChannelized(const char *name,
    const char *title,
    QnCorrectionsEventClassVariablesSet &ecvs,
    Int_t nNoOfChannels,
    Option_t *option) : QnCorrectionsHistogramBase(name, title, ecvs, option) {

  fValues = NULL;
  fEntries = NULL;
  fUsedChannel = NULL;
  fChannelGroup = NULL;
  fNoOfChannels = nNoOfChannels;
  fActualNoOfChannels = 0;
  fChannelMap = NULL;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsProfileChannelized::~QnCorrectionsProfileChannelized() {

  if (fUsedChannel != NULL) delete fUsedChannel;
  if (fChannelGroup != NULL) delete fChannelGroup;
  if (fChannelMap != NULL) delete fChannelMap;
}



/// Creates the support histograms for the profile function
///
/// Based in the event classes variables set in the parent class
/// and the channel information passed as parameters
/// the values and entries multidimensional histograms are
/// created.
///
/// Both histograms are added to the passed histogram list
///
/// The actual number of channels is stored and a mask from
/// external channel number to histogram channel number. If
/// bUsedChannel is NULL all channels
/// within fNoOfChannels are assigned to this profile.
/// If nChannelGroup is NULL all channels assigned to this
/// profile are allocated to the same group.
/// \param histogramList list where the histograms have to be added
/// \param bUsedChannel array of booleans one per each channel
/// \param nChannelGroup array of group number for each channel
/// \return true if properly created
Bool_t QnCorrectionsProfileChannelized::CreateProfileHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup) {
  /* let's build the histograms names and titles */
  TString histoName = GetName();
  TString histoTitle = GetTitle();
  TString entriesHistoName = GetName(); entriesHistoName += szEntriesHistoSuffix;
  TString entriesHistoTitle = GetTitle(); entriesHistoTitle += szEntriesHistoSuffix;

  /* we open space for channel variable as well */
  Int_t nVariables = fEventClassVariables.GetEntriesFast();
  Double_t *minvals = new Double_t[nVariables+1];
  Double_t *maxvals = new Double_t[nVariables+1];
  Int_t *nbins = new Int_t[nVariables+1];

  /* get the multidimensional structure */
  fEventClassVariables.GetMultidimensionalConfiguration(nbins,minvals,maxvals);

  /* lets consider now the channel information */
  fUsedChannel = new Bool_t[fNoOfChannels];
  fChannelGroup = new Int_t[fNoOfChannels];
  fChannelMap = new Int_t[fNoOfChannels];

  fActualNoOfChannels = 0;
  for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
    if (bUsedChannel != NULL) {
      fUsedChannel[ixChannel] = bUsedChannel[ixChannel];
    }
    else {
      fUsedChannel[ixChannel] = kTRUE;
    }
    if (nChannelGroup != NULL) {
      fChannelGroup[ixChannel] = nChannelGroup[ixChannel];
    }
    else {
      fChannelGroup[ixChannel] = 0;
    }

    if (fUsedChannel[ixChannel]) {
      fChannelMap[ixChannel] = fActualNoOfChannels;
      fActualNoOfChannels++;
    }
  }

  /* TODO: there will be a wrong external view of the channel number especially */
  /* manifested when there are holes in the channel assignment */
  /* so, lets complete the dimension information */
  minvals[nVariables] = -0.5;
  maxvals[nVariables] = -0.5 + fActualNoOfChannels;
  nbins[nVariables] = fActualNoOfChannels;

  /* create the values and entries multidimensional histograms */
  fValues = new THnF((const char *) histoName, (const char *) histoTitle,nVariables+1,nbins,minvals,maxvals);
  fEntries = new THnI((const char *) entriesHistoName, (const char *) entriesHistoTitle,nVariables+1,nbins,minvals,maxvals);

  /* now let's set the proper binning and label on each axis */
  for (Int_t var = 0; var < nVariables; var++) {
    fValues->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fEntries->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fValues->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
    fEntries->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
  }

  /* and now the channel axis */
  fValues->GetAxis(nVariables)->SetTitle(szChannelAxisTitle);
  fEntries->GetAxis(nVariables)->SetTitle(szChannelAxisTitle);

  /* and now set the proper channel labels if needed */
  if (fActualNoOfChannels != fNoOfChannels) {
    for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
      if (fUsedChannel[ixChannel]) {
        fValues->GetAxis(nVariables)->SetBinLabel(fChannelMap[ixChannel]+1, Form("%d", ixChannel));
        fEntries->GetAxis(nVariables)->SetBinLabel(fChannelMap[ixChannel]+1, Form("%d", ixChannel));
      }
    }
  }

  fValues->Sumw2();

  histogramList->Add(fValues);
  histogramList->Add(fEntries);

  delete [] minvals;
  delete [] maxvals;
  delete [] nbins;

  return kTRUE;
}


/// Get the bin number for the current variable content and passed channel
///
/// The bin number identifies the event class the current
/// variable content points to under the passed channel.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param nChannel the interested external channel number
/// \return the associated bin to the current variables content
Int_t QnCorrectionsProfileChannelized::GetBin(const Float_t *variableContainer, Int_t nChannel) {

  FillBinAxesValues(variableContainer, fChannelMap[nChannel]);
  /* store the channel number */
  return fEntries->GetBin(fBinAxesValues);
}

/// Get the bin content for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// is requested.If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileChannelized::GetBinContent(Int_t bin) {

  Int_t nEntries = fEntries->GetBinContent(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fValues->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the bin content error for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param bin the interested bin number
/// \return the bin number content error
Float_t QnCorrectionsProfileChannelized::GetBinError(Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);
  Float_t values = fValues->GetBinContent(bin);
  Float_t error2 = fValues->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Fills the histogram
///
/// The involved bin is computed according to the current variables
/// content and the passed external channel number. The bin is then
/// increased by the given weight and the entries also increased properly.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param nChannel the interested external channel number
/// \param weight the increment in the bin content
void QnCorrectionsProfileChannelized::Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight) {
  /* keep the total entries in fValues updated */
  Double_t nEntries = fValues->GetEntries();

  FillBinAxesValues(variableContainer, fChannelMap[nChannel]);
  /* and now update the bin */
  fValues->Fill(fBinAxesValues, weight);
  fValues->SetEntries(nEntries + 1);
  fEntries->Fill(fBinAxesValues, 1.0);
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsProfileChannelizedIngress);
/// \endcond

/// Default constructor
QnCorrectionsProfileChannelizedIngress::QnCorrectionsProfileChannelizedIngress() :
    QnCorrectionsHistogramBase() {

  fValues = NULL;
  fGroupValues = NULL;
  fUsedChannel = NULL;
  fChannelGroup = NULL;
  fNoOfChannels = 0;
  fActualNoOfChannels = 0;
  fChannelMap = NULL;
  fUseGroups = kFALSE;
  fUsedGroup = NULL;
  fActualNoOfGroups = 0;
  fNoOfGroups = 0;
  fGroupMap = NULL;
}

/// Normal constructor
///
/// Stores the set of variables that identify the
/// different event classes passing them to its parent
/// and prepares the object for actual histogram
/// attachment
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
/// \param nNoOfChannels the number of channels associated
/// \param option option for errors computation
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
QnCorrectionsProfileChannelizedIngress::QnCorrectionsProfileChannelizedIngress(const char *name,
    const char *title,
    QnCorrectionsEventClassVariablesSet &ecvs,
    Int_t nNoOfChannels,
    Option_t *option) : QnCorrectionsHistogramBase(name, title, ecvs, option) {

  fValues = NULL;
  fGroupValues = NULL;
  fUsedChannel = NULL;
  fChannelGroup = NULL;
  fNoOfChannels = nNoOfChannels;
  fActualNoOfChannels = 0;
  fChannelMap = NULL;
  fUseGroups = kFALSE;
  fUsedGroup = NULL;
  fActualNoOfGroups = 0;
  fNoOfGroups = 0;
  fGroupMap = NULL;
}

/// Default destructor
/// Releases the memory taken
QnCorrectionsProfileChannelizedIngress::~QnCorrectionsProfileChannelizedIngress() {

  if (fUsedChannel != NULL) delete fUsedChannel;
  if (fChannelGroup != NULL) delete fChannelGroup;
  if (fChannelMap != NULL) delete fChannelMap;
  if (fValues != NULL) delete fValues;
  if (fGroupValues != NULL) delete fGroupValues;
  if (fUsedGroup != NULL) delete fUsedChannel;
  if (fGroupMap != NULL) delete fChannelMap;
}



/// Attaches existing histograms as the support histograms for the profile function
///
/// The histograms are located in the passed list and if found and with the
/// proper dimensions their references are stored in member variables.
///
/// Channel information is used to build internal structures such as
/// the channel map and the actual number of channels and the channels groups
/// and the actual number of groups. The information
/// is matched with the found histogram to validate it. If
/// bUsedChannel is NULL all channels
/// within fNoOfChannels are assigned to this profile.
/// If nChannelGroup is NULL all channels assigned to this
/// profile are allocated to the same group.
///
/// Once the histograms are found and validated, a unique value / error channel histogram
/// is created for efficient access and a potential unique value / error channels group
/// histogram is created.
/// \param histogramList list where the histograms have to be located
/// \param bUsedChannel array of booleans one per each channel
/// \param nChannelGroup array of group number for each channel
/// \return true if properly attached else false
Bool_t QnCorrectionsProfileChannelizedIngress::AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup) {
  /* let's build the histograms names */
  TString histoName = GetName();
  TString entriesHistoName = GetName(); entriesHistoName += szEntriesHistoSuffix;

  /* initialize. Remember we own the histograms */
  if (fValues != NULL) delete fValues;
  if (fGroupValues != NULL) delete fGroupValues;
  fValues = NULL;
  fGroupValues = NULL;

  /* lets consider now the channel information */
  fUsedChannel = new Bool_t[fNoOfChannels];
  fChannelGroup = new Int_t[fNoOfChannels];
  fChannelMap = new Int_t[fNoOfChannels];
  for (Int_t i = 0; i < fNoOfChannels; i++) {
    fUsedChannel[i] = kFALSE;
    fChannelGroup[i] = 0;
    fChannelMap[i] = -1;
  }

  Int_t nMinGroup = 0xFFFF;
  Int_t nMaxGroup = 0x0000;
  fActualNoOfChannels = 0;
  for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
    if (bUsedChannel != NULL) {
      fUsedChannel[ixChannel] = bUsedChannel[ixChannel];
    }
    else {
      fUsedChannel[ixChannel] = kTRUE;
    }
    if (nChannelGroup != NULL) {
      fChannelGroup[ixChannel] = nChannelGroup[ixChannel];
      /* update min max group number */
      if (nChannelGroup[ixChannel] < nMinGroup)
        nMinGroup = nChannelGroup[ixChannel];
      if (nMaxGroup < nChannelGroup[ixChannel])
        nMaxGroup = nChannelGroup[ixChannel];
    }
    else {
      fChannelGroup[ixChannel] = 0;
      nMinGroup = 0;
      nMaxGroup = 0;
    }
    if (fUsedChannel[ixChannel]) {
      fChannelMap[ixChannel] = fActualNoOfChannels;
      fActualNoOfChannels++;
    }
  }
  fUseGroups = (nChannelGroup != NULL) && (nMinGroup != nMaxGroup);

  if (fUseGroups) {
    /* let's build the groups support structures */
    fNoOfGroups = nMaxGroup + 1; /* just in case group number starts from zero */
    fUsedGroup = new Bool_t[fNoOfGroups];
    fGroupMap = new Int_t[fNoOfGroups];
    for (Int_t i = 0; i < fNoOfGroups; i++) {
      fUsedGroup[i] = kFALSE;
      fGroupMap[i] = -1;
    }
    fActualNoOfGroups = 0;
    for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
      if (fUsedChannel[ixChannel]) {
        if (fUsedGroup[fChannelGroup[ixChannel]]) {
          /* group already considered */
          continue;
        }
        else {
          /* new group number */
          fUsedGroup[fChannelGroup[ixChannel]] = kTRUE;
          fGroupMap[fChannelGroup[ixChannel]] = fActualNoOfGroups;
          fActualNoOfGroups++;
        }
      }
    }
  }

  /* let's first try the Values / Entries structure */
  THnI *origEntries = (THnI *) histogramList->FindObject((const char*) entriesHistoName);
  if (origEntries != NULL) {
    /* so we get it! */
    /* let's check the channel axis */
    if (fActualNoOfChannels != origEntries->GetAxis(fEventClassVariables.GetEntriesFast())->GetNbins())
      return kFALSE;
    THnF *origValues = (THnF *) histogramList->FindObject((const char *)histoName);
    if (origValues == NULL)
      return kFALSE;
    /* let's check the channel axis */
    if (fActualNoOfChannels != origValues->GetAxis(fEventClassVariables.GetEntriesFast())->GetNbins())
      return kFALSE;

    /* so we got the original histograms */
    /* now we should build the definitive histogram value / error */
    /* and the group value / error histogram if applicable */
    fValues = DivideTHnF(origValues, origEntries);

    if (fUseGroups) {
      /* let's then build the groups histogram */
      TString histoGroupName = szGroupHistoPrefix;
      histoGroupName += GetName();
      TString histoGroupTitle = szGroupHistoPrefix;
      histoGroupTitle += GetTitle();

      /* we open space for channel variable as well */
      Int_t nVariables = fEventClassVariables.GetEntriesFast();
      Double_t *minvals = new Double_t[nVariables+1];
      Double_t *maxvals = new Double_t[nVariables+1];
      Int_t *nbins = new Int_t[nVariables+1];

      /* get the multidimensional structure */
      fEventClassVariables.GetMultidimensionalConfiguration(nbins,minvals,maxvals);

      /* TODO: there will be a wrong external view of the channel number especially */
      /* manifested when there are holes in the channel assignment */
      /* so, lets complete the dimension information */
      minvals[nVariables] = -0.5;
      maxvals[nVariables] = -0.5 + fActualNoOfGroups;
      nbins[nVariables] = fActualNoOfGroups;

      /* create the values and entries multidimensional histograms */
      fGroupValues = new THnF((const char *) histoGroupName, (const char *) histoGroupTitle,nVariables+1,nbins,minvals,maxvals);

      /* now let's set the proper binning and label on each axis */
      for (Int_t var = 0; var < nVariables; var++) {
        fGroupValues->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
        fGroupValues->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      }

      /* and now the channel axis */
      fGroupValues->GetAxis(nVariables)->SetTitle(szGroupAxisTitle);

      fGroupValues->Sumw2();

      /* now let's build its content */
      /* the procedure is as follow: we will project and add together the values histogram */
      /* of the channels corresponding to a group number and then store the result in the */
      /* corresponding group values */
      Int_t *dimToProject = new Int_t[nVariables];
      for (Int_t var = 0; var < nVariables; var++)
        dimToProject[var] = var;
      Int_t *binsArray = new Int_t[nVariables+1];

      for (Int_t ixGroup = 0; ixGroup < fNoOfGroups; ixGroup++) {
        if (fUsedGroup[ixGroup]) {
          /* new group, let's add its contributor channels */
          THnF *hCumProjected = NULL;
          for (Int_t ixChannel = 0; ixChannel < fNoOfChannels; ixChannel++) {
            if (fUsedChannel[ixChannel]) {
              if (fChannelGroup[ixChannel] == ixGroup) {
                /* channel within the group found let's add its content */
                /* first filter the projection */
                fValues->GetAxis(nVariables)->SetRange(fChannelMap[ixChannel]+1, fChannelMap[ixChannel]+1);
                if (hCumProjected != NULL) {
                  /* let's accumulate the new channel */
                  THnF *hProjected = (THnF *) fValues->Projection(nVariables,dimToProject, "E");
                  hCumProjected->Add(hProjected);
                  delete hProjected;
                }
                else {
                  /* first channel in the group */
                  hCumProjected = (THnF *) fValues->Projection(nVariables,dimToProject, "E");
                }
              }
            }
          }
          /* let's store the channels contribution to the group values for the group */
          /* we have to go through the multidimensional structure */
          /* the group we are storing values */
          for (Int_t var = 0; var < nVariables; var++)
            binsArray[var] = 0;
          binsArray[nVariables] = fGroupMap[ixGroup] + 1;
          CopyTHnF(fGroupValues, hCumProjected, binsArray);
          delete hCumProjected;
        }
      }
      /* reset the range */
      fValues->GetAxis(nVariables)->SetRange(0, 0);
      delete dimToProject;
      delete binsArray;
    }
  }
  else
    return kFALSE;

/* TODO: implement a new structure when we so decide */
  return kTRUE;
}

/// Get the bin number for the current variable content and passed channel
///
/// The bin number identifies the event class the current
/// variable content points to under the passed channel.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param nChannel the interested external channel number
/// \return the associated bin to the current variables content
Int_t QnCorrectionsProfileChannelizedIngress::GetBin(const Float_t *variableContainer, Int_t nChannel) {

  /* store also the channel number */
  FillBinAxesValues(variableContainer, fChannelMap[nChannel]);
  return fValues->GetBin(fBinAxesValues);
}

/// Get the bin content for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// is requested. The bin average stored as bin content is returned
///
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileChannelizedIngress::GetBinContent(Int_t bin) {

  return fValues->GetBinContent(bin);
}

/// Get the bin content error for the passed bin number
///
/// The bin number identifies a desired event class whose content
/// error is requested. The bin error stored as bin error is returned.
///
/// \param bin the interested bin number
/// \return the bin number content error
Float_t QnCorrectionsProfileChannelizedIngress::GetBinError(Int_t bin) {

  return fValues->GetBinError(bin);
}

/// Get the bin number for the current variable content and passed channel group number
///
/// The bin number identifies the event class the current
/// variable content points to under the passed channel group number.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param nChannel the interested external channel number which group number is asked
/// \return the associated bin to the current variables content
Int_t QnCorrectionsProfileChannelizedIngress::GetGrpBin(const Float_t *variableContainer, Int_t nChannel) {

  /* check the groups structures are in place */
  if (fUseGroups) {
    /* store also the group number */
    FillBinAxesValues(variableContainer, fGroupMap[fChannelGroup[nChannel]]);
    return fGroupValues->GetBin(fBinAxesValues);
  }
  return -1;
}

/// Get the group bin content for the passed bin number
///
/// The bin number identifies a desired event class whose group content
/// is requested.
///
/// \param bin the interested group bin number
/// \return the group bin number content
Float_t QnCorrectionsProfileChannelizedIngress::GetGrpBinContent(Int_t bin) {

  /* check the groups structures are in place */
  if (fUseGroups) {
    return fGroupValues->GetBinContent(bin);
  }
  return 1.0;
}

/// Get the group bin content error for the passed bin number
///
/// The group bin number identifies a desired event class whose content
/// error is requested.
///
/// \param bin the interested group bin number
/// \return the bin number content error
Float_t QnCorrectionsProfileChannelizedIngress::GetGrpBinError(Int_t bin) {

  /* check the groups structures are in place */
  if (fUseGroups) {
    return fGroupValues->GetBinError(bin);
  }
  return 1.0;
}


/// \cond CLASSIMP
ClassImp(QnCorrectionsProfileComponents);
/// \endcond

/// Default constructor
QnCorrectionsProfileComponents::QnCorrectionsProfileComponents() :
    QnCorrectionsHistogramBase() {

  fXValues = NULL;
  fYValues = NULL;
  fXharmonicFillMask = 0x0000;
  fYharmonicFillMask = 0x0000;
  fFullFilled = 0x0000;
  fEntries = NULL;
}

/// Normal constructor
///
/// Stores the set of variables that identify the
/// different event classes passing them to its parent
/// and prepares the object for actual histogram
/// creation or attachment
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
/// \param option option for errors computation
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
QnCorrectionsProfileComponents::QnCorrectionsProfileComponents(const char *name,
    const char *title,
    QnCorrectionsEventClassVariablesSet &ecvs,
    Option_t *option) :
    QnCorrectionsHistogramBase(name, title, ecvs, option) {

  fXValues = NULL;
  fYValues = NULL;
  fXharmonicFillMask = 0x0000;
  fYharmonicFillMask = 0x0000;
  fFullFilled = 0x0000;
  fEntries = NULL;
}

/// Default destructor
///
/// Returns the only taken memory, the harmonic histograms storage,
/// the own histograms and other members are not own at destruction time
QnCorrectionsProfileComponents::~QnCorrectionsProfileComponents() {

  if (fXValues != NULL)
    delete [] fXValues;
  if (fYValues != NULL)
    delete [] fYValues;
}

/// Creates the X, Y components support histograms for the profile function
///
/// Based in the event classes variables set in the parent class
/// the values and entries multidimensional histograms are
/// created.
///
/// For each harmonic number two values histograms are created, X and Y.
/// The histograms are organized to support external harmonic number.
/// By default the external harmonic number is always considered to
/// start by one. If no map is passed as parameter the external harmonic
/// numbers are considered as: 1, 2, ..., nNoOfHarmonic.
/// If the user wants a different assignment he has to provide an
/// ordered map, for instance: four harmonics with external harmonic numbers
/// 2, 4, 6 and 8 will require nNoOfHarmonics = 4 and harmonicMap = [2, 4, 6, 8].
/// The fully filled condition is computed and stored
///
/// The whole set of histograms are added to the passed histogram list
///
/// \param histogramList list where the histograms have to be added
/// \param nNoOfHarmonics the desired number of harmonics
/// \param harmonicMap ordered array with the external number of the harmonics
/// \return true if properly created
Bool_t QnCorrectionsProfileComponents::CreateComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap) {
  /* let's build the histograms names and titles */
  TString histoXName = GetName(); histoXName += szXComponentSuffix;
  TString histoYName = GetName(); histoYName += szYComponentSuffix;
  TString histoXTitle = GetTitle(); histoXTitle += szXComponentSuffix;
  TString histoYTitle = GetTitle(); histoYTitle += szYComponentSuffix;
  TString entriesHistoName = GetName();
  entriesHistoName += szXComponentSuffix;
  entriesHistoName += szYComponentSuffix;
  entriesHistoName += szEntriesHistoSuffix;
  TString entriesHistoTitle = GetTitle();
  entriesHistoTitle += szXComponentSuffix;
  entriesHistoTitle += szYComponentSuffix;
  entriesHistoTitle += szEntriesHistoSuffix;

  /* check whether within the supported harmonic range */
  Int_t nHigherHarmonic = nNoOfHarmonics;
  if (harmonicMap != NULL) {
    nHigherHarmonic = harmonicMap[nNoOfHarmonics - 1];
  }
  if (nMaxHarmonicNumberSupported < nHigherHarmonic) {
    QnCorrectionsFatal(Form("You requested support for harmonic %d but the highest harmonic supported by the framework is currently %d",
        nHigherHarmonic, nMaxHarmonicNumberSupported));
  }

  /* let's support the external harmonic number map */
  /* external harmonic number will always start from one */
  Int_t nNumberOfSlots = 1;
  if (harmonicMap != NULL) {
    /* the highest harmonic number within the map if any */
    nNumberOfSlots += harmonicMap[nNoOfHarmonics - 1];
  }
  else {
    nNumberOfSlots += nNoOfHarmonics;
  }

  /* now allocate the slots for the values histograms */
  fXValues = new THnF *[nNumberOfSlots];
  fYValues = new THnF *[nNumberOfSlots];
  /* and initiallize them */
  for (Int_t i = 0; i < nNumberOfSlots; i++) {
    fXValues[i] = NULL;
    fYValues[i] = NULL;
  }

  /* now prepare the construction of the histograms */
  Int_t nVariables = fEventClassVariables.GetEntriesFast();

  Double_t *minvals = new Double_t[nVariables];
  Double_t *maxvals = new Double_t[nVariables];
  Int_t *nbins = new Int_t[nVariables];
  TString sVariableLabels = "";

  /* get the multidimensional structure */
  fEventClassVariables.GetMultidimensionalConfiguration(nbins,minvals,maxvals);

  /* create the values multidimensional histograms for each harmonic */
  Int_t currentHarmonic = 0;
  for (Int_t i = 0; i < nNoOfHarmonics; i++) {
    if (harmonicMap != NULL) {
      currentHarmonic = harmonicMap[i];
    }
    else {
      currentHarmonic++;
    }
    fXValues[currentHarmonic] = new THnF(Form("%s_h%d", (const char *) histoXName, currentHarmonic),
        Form("%s h%d", (const char *) histoXTitle, currentHarmonic),
        nVariables,nbins,minvals,maxvals);
    fYValues[currentHarmonic] = new THnF(Form("%s_h%d", (const char *) histoYName, currentHarmonic),
        Form("%s h%d", (const char *) histoYTitle, currentHarmonic),
        nVariables,nbins,minvals,maxvals);

    /* now let's set the proper binning and label on each axis */
    for (Int_t var = 0; var < nVariables; var++) {
      fXValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fXValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fXValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fXValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fYValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fYValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fYValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fYValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
    }

    /* ask for square sum accumulation */
    fXValues[currentHarmonic]->Sumw2();
    fYValues[currentHarmonic]->Sumw2();

    /* and finally add the histograms to the list */
    histogramList->Add(fXValues[currentHarmonic]);
    histogramList->Add(fYValues[currentHarmonic]);

    /* and update the fully filled condition */
    fFullFilled |= harmonicNumberMask[currentHarmonic];
  }

  /* create the entries multidimensional histogram */
  fEntries = new THnI((const char *) entriesHistoName, (const char *) entriesHistoTitle,nVariables,nbins,minvals,maxvals);

  /* now let's set the proper binning and label on each entries histogram axis */
  for (Int_t var = 0; var < nVariables; var++) {
    fEntries->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fEntries->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
  }

  /* and finally add the entries histogram to the list */
  histogramList->Add(fEntries);

  delete [] minvals;
  delete [] maxvals;
  delete [] nbins;

  return kTRUE;
}

/// Attaches existing histograms as the support histograms for X, Y, component
/// of the profile function for different harmonics
///
/// The histograms are located in the passed list and if found and with the
/// proper dimensions their references are stored in member variables.
///
/// The harmonic map is inferred from the found histograms within the list
/// that match the naming scheme.
///
/// \param histogramList list where the histograms have to be located
/// \return true if properly attached else false
Bool_t QnCorrectionsProfileComponents::AttachHistograms(TList *histogramList) {
  /* let's build the histograms names */
  TString histoXName = GetName(); histoXName += szXComponentSuffix;
  TString histoYName = GetName(); histoYName += szYComponentSuffix;
  TString entriesHistoName = GetName();
  entriesHistoName += szXComponentSuffix;
  entriesHistoName += szYComponentSuffix;
  entriesHistoName += szEntriesHistoSuffix;

  /* initialize. Remember we don't own the histograms */
  fEntries = NULL;
  if (fXValues != NULL) {
    delete [] fXValues;
    fXValues = NULL;
  }
  if (fYValues != NULL) {
    delete [] fYValues;
    fYValues = NULL;
  }
  fXharmonicFillMask = 0x0000;
  fYharmonicFillMask = 0x0000;
  fFullFilled = 0x0000;

  fEntries = (THnI *) histogramList->FindObject((const char*) entriesHistoName);
  if (fEntries != NULL) {
    /* allocate enough space for the supported harmonic numbers */
    fXValues = new THnF *[nMaxHarmonicNumberSupported + 1];
    fYValues = new THnF *[nMaxHarmonicNumberSupported + 1];

    /* search the multidimensional histograms for each harmonic */
    Int_t currentHarmonic = 0;
    for (Int_t i = 0; i < nMaxHarmonicNumberSupported; i++) {
      currentHarmonic++;

      fXValues[currentHarmonic] = (THnF *) histogramList->FindObject(Form("%s_h%d", (const char *) histoXName, currentHarmonic));
      fYValues[currentHarmonic] = (THnF *) histogramList->FindObject(Form("%s_h%d", (const char *) histoYName, currentHarmonic));

      /* and update the fully filled condition whether applicable */
      if ((fXValues[currentHarmonic]  != NULL) && (fYValues[currentHarmonic] != NULL))
      fFullFilled |= harmonicNumberMask[currentHarmonic];
    }
  }
  else
    return kFALSE;

/* TODO: pending to decide whether we divide the histograms and modify the
 * get content and get errors functions accordingly or we split the class
 * in created histograms and attached histograms. So far we leave it as
 * it is and we'll see the penalty for not having them divided.
 */
  /* check that we actually got something */
  if (fFullFilled != 0x0000)
    return kTRUE;
  else
    return kFALSE;
}

/// Get the bin number for the current variable content
///
/// The bin number identifies the event class the current
/// variable content points to.
///
/// \param variableContainer the current variables content addressed by var Id
/// \return the associated bin to the current variables content
Int_t QnCorrectionsProfileComponents::GetBin(const Float_t *variableContainer) {
  FillBinAxesValues(variableContainer);
  return fEntries->GetBin(fBinAxesValues);
}

/// Get the X component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileComponents::GetXBinContent(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fXValues[harmonic]->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the Y component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileComponents::GetYBinContent(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fYValues[harmonic]->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the X component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsProfileComponents::GetXBinError(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  Float_t values = fXValues[harmonic]->GetBinContent(bin);
  Float_t error2 = fXValues[harmonic]->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Get the Y component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsProfileComponents::GetYBinError(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  Float_t values = fYValues[harmonic]->GetBinContent(bin);
  Float_t error2 = fYValues[harmonic]->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Fills the X component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight.
/// The entries is only updated if the whole set for both components
/// has been already filled. A check is done for detecting consecutive
/// fills for certain harmonic without a previous entries update.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfileComponents::FillX(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  /* first the sanity checks */
  if (fXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
  }

  if (fXharmonicFillMask & harmonicNumberMask[harmonic]) {
    QnCorrectionsFatal(Form("Filling twice the harmonic %d before entries update in histogram %s.\n" \
        "   This means you probably have not updated the other components for this harmonic. FIX IT, PLEASE.", harmonic, GetName()));
  }

  /* now it's safe to continue */

  /* keep total entries in fValues updated */
  Double_t nEntries = fXValues[harmonic]->GetEntries();

  FillBinAxesValues(variableContainer);
  fXValues[harmonic]->Fill(fBinAxesValues, weight);
  fXValues[harmonic]->SetEntries(nEntries + 1);

  /* update harmonic fill mask */
  fXharmonicFillMask |= harmonicNumberMask[harmonic];

  /* now check if time for updating entries histogram */
  if (fXharmonicFillMask != fFullFilled) return;
  if (fYharmonicFillMask != fFullFilled) return;
  /* update entries and reset the masks */
  fEntries->Fill(fBinAxesValues, 1.0);
  fXharmonicFillMask = 0x0000;
  fYharmonicFillMask = 0x0000;
}

/// Fills the Y component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight.
/// The entries is only updated if the whole set for both components
/// has been already filled. A check is done for detecting consecutive
/// fills for certain harmonic without a previous entries update.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfileComponents::FillY(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  /* first the sanity checks */
  if (fYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
  }

  if (fYharmonicFillMask & harmonicNumberMask[harmonic]) {
    QnCorrectionsFatal(Form("Filling twice the harmonic %d before entries update in histogram %s.\n" \
        "   This means you probably have not updated the other components for this harmonic. FIX IT, PLEASE.", harmonic, GetName()));
  }

  /* now it's safe to continue */

  /* keep total entries in fValues updated */
  Double_t nEntries = fYValues[harmonic]->GetEntries();

  FillBinAxesValues(variableContainer);
  fYValues[harmonic]->Fill(fBinAxesValues, weight);
  fYValues[harmonic]->SetEntries(nEntries + 1);

  /* update harmonic fill mask */
  fYharmonicFillMask |= harmonicNumberMask[harmonic];

  /* now check if time for updating entries histogram */
  if (fYharmonicFillMask != fFullFilled) return;
  if (fXharmonicFillMask != fFullFilled) return;
  /* update entries and reset the masks */
  fEntries->Fill(fBinAxesValues, 1.0);
  fXharmonicFillMask = 0x0000;
  fYharmonicFillMask = 0x0000;
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsProfileCorrelationComponents);
/// \endcond

/// Default constructor
QnCorrectionsProfileCorrelationComponents::QnCorrectionsProfileCorrelationComponents() :
    QnCorrectionsHistogramBase() {

  fXXValues = NULL;
  fXYValues = NULL;
  fYXValues = NULL;
  fYYValues = NULL;
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
  fFullFilled = 0x0000;
  fEntries = NULL;
}

/// Normal constructor
///
/// Stores the set of variables that identify the
/// different event classes passing them to its parent
/// and prepares the object for actual histogram
/// creation or attachment
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
/// \param option option for errors computation
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
QnCorrectionsProfileCorrelationComponents::QnCorrectionsProfileCorrelationComponents(const char *name,
    const char *title,
    QnCorrectionsEventClassVariablesSet &ecvs,
    Option_t *option) :
        QnCorrectionsHistogramBase(name, title, ecvs, option) {

  fXXValues = NULL;
  fXYValues = NULL;
  fYXValues = NULL;
  fYYValues = NULL;
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
  fFullFilled = 0x0000;
  fEntries = NULL;
}

/// Default destructor
///
/// Returns the only taken memory, the harmonic histograms storage,
/// the own histograms and other members are not own at destruction time
QnCorrectionsProfileCorrelationComponents::~QnCorrectionsProfileCorrelationComponents() {

  if (fXXValues != NULL)
    delete [] fXXValues;
  if (fXYValues != NULL)
    delete [] fXYValues;
  if (fYXValues != NULL)
    delete [] fYXValues;
  if (fYYValues != NULL)
    delete [] fYYValues;
}

/// Creates the XX, XY, YX, YY correlation components support histograms
/// for the profile function
///
/// Based in the event classes variables set in the parent class
/// the values and entries multidimensional histograms are
/// created.
///
/// For each harmonic number fout values histograms are created, XX,
/// XY, YX and YY. The histograms are organized to support external harmonic
/// number. By default the external harmonic number is always considered to
/// start by one. If no map is passed as parameter the external harmonic
/// numbers are considered as: 1, 2, ..., nNoOfHarmonic.
/// If the user wants a different assignment he has to provide an
/// ordered map, for instance: four harmonics with external harmonic numbers
/// 2, 4, 6 and 8 will require nNoOfHarmonics = 4 and harmonicMap = [2, 4, 6, 8].
/// The fully filled condition is computed and stored
///
/// The whole set of histograms are added to the passed histogram list
///
/// \param histogramList list where the histograms have to be added
/// \param nNoOfHarmonics the desired number of harmonics
/// \param harmonicMap ordered array with the external number of the harmonics
/// \return true if properly created
Bool_t QnCorrectionsProfileCorrelationComponents::CreateCorrelationComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap) {
  /* let's build the histograms names and titles */
  TString histoXXName = GetName(); histoXXName += szXXCorrelationComponentSuffix;
  TString histoXYName = GetName(); histoXYName += szXYCorrelationComponentSuffix;
  TString histoYXName = GetName(); histoYXName += szYXCorrelationComponentSuffix;
  TString histoYYName = GetName(); histoYYName += szYYCorrelationComponentSuffix;
  TString histoXXTitle = GetTitle(); histoXXTitle += szXXCorrelationComponentSuffix;
  TString histoXYTitle = GetTitle(); histoXYTitle += szXYCorrelationComponentSuffix;
  TString histoYXTitle = GetTitle(); histoYXTitle += szYXCorrelationComponentSuffix;
  TString histoYYTitle = GetTitle(); histoYYTitle += szYYCorrelationComponentSuffix;
  TString entriesHistoName = GetName();
  entriesHistoName += szXXCorrelationComponentSuffix;
  entriesHistoName += szXYCorrelationComponentSuffix;
  entriesHistoName += szYXCorrelationComponentSuffix;
  entriesHistoName += szYYCorrelationComponentSuffix;
  entriesHistoName += szEntriesHistoSuffix;
  TString entriesHistoTitle = GetTitle();
  entriesHistoTitle += szXXCorrelationComponentSuffix;
  entriesHistoTitle += szXYCorrelationComponentSuffix;
  entriesHistoTitle += szYXCorrelationComponentSuffix;
  entriesHistoTitle += szYYCorrelationComponentSuffix;
  entriesHistoTitle += szEntriesHistoSuffix;

  /* check whether within the supported harmonic range */
  Int_t nHigherHarmonic = nNoOfHarmonics;
  if (harmonicMap != NULL) {
    nHigherHarmonic = harmonicMap[nNoOfHarmonics - 1];
  }
  if (nMaxHarmonicNumberSupported < nHigherHarmonic) {
    QnCorrectionsFatal(Form("You requested support for harmonic %d but the highest harmonic supported by the framework is currently %d",
        nHigherHarmonic, nMaxHarmonicNumberSupported));
  }

  /* let's support the external harmonic number map */
  /* external harmonic number will always start from one */
  Int_t nNumberOfSlots = 1;
  if (harmonicMap != NULL) {
    /* the highest harmonic number within the map if any */
    nNumberOfSlots += harmonicMap[nNoOfHarmonics - 1];
  }
  else {
    nNumberOfSlots += nNoOfHarmonics;
  }

  /* now allocate the slots for the values histograms */
  fXXValues = new THnF *[nNumberOfSlots];
  fXYValues = new THnF *[nNumberOfSlots];
  fYXValues = new THnF *[nNumberOfSlots];
  fYYValues = new THnF *[nNumberOfSlots];
  /* and initiallize them */
  for (Int_t i = 0; i < nNumberOfSlots; i++) {
    fXXValues[i] = NULL;
    fXYValues[i] = NULL;
    fYXValues[i] = NULL;
    fYYValues[i] = NULL;
  }

  /* now prepare the construction of the histograms */
  Int_t nVariables = fEventClassVariables.GetEntriesFast();

  Double_t *minvals = new Double_t[nVariables];
  Double_t *maxvals = new Double_t[nVariables];
  Int_t *nbins = new Int_t[nVariables];
  TString sVariableLabels = "";

  /* get the multidimensional structure */
  fEventClassVariables.GetMultidimensionalConfiguration(nbins,minvals,maxvals);

  /* create the values multidimensional histograms for each harmonic */
  Int_t currentHarmonic = 0;
  for (Int_t i = 0; i < nNoOfHarmonics; i++) {
    if (harmonicMap != NULL) {
      currentHarmonic = harmonicMap[i];
    }
    else {
      currentHarmonic++;
    }
    fXXValues[currentHarmonic] = new THnF(Form("%s_h%d", (const char *) histoXXName, currentHarmonic),
        Form("%s h%d", (const char *) histoXXTitle, currentHarmonic),
        nVariables,nbins,minvals,maxvals);
    fXYValues[currentHarmonic] = new THnF(Form("%s_h%d", (const char *) histoXYName, currentHarmonic),
        Form("%s h%d", (const char *) histoXYTitle, currentHarmonic),
        nVariables,nbins,minvals,maxvals);
    fYXValues[currentHarmonic] = new THnF(Form("%s_h%d", (const char *) histoYXName, currentHarmonic),
        Form("%s h%d", (const char *) histoYXTitle, currentHarmonic),
        nVariables,nbins,minvals,maxvals);
    fYYValues[currentHarmonic] = new THnF(Form("%s_h%d", (const char *) histoYYName, currentHarmonic),
        Form("%s h%d", (const char *) histoYYTitle, currentHarmonic),
        nVariables,nbins,minvals,maxvals);

    /* now let's set the proper binning and label on each axis */
    for (Int_t var = 0; var < nVariables; var++) {
      fXXValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fXXValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fXXValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fXXValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fXYValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fXYValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fXYValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fXYValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fYXValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fYXValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fYXValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fYXValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fYYValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fYYValues[currentHarmonic]->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
      fYYValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
      fYYValues[currentHarmonic]->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
    }

    /* ask for square sum accumulation */
    fXXValues[currentHarmonic]->Sumw2();
    fXYValues[currentHarmonic]->Sumw2();
    fYXValues[currentHarmonic]->Sumw2();
    fYYValues[currentHarmonic]->Sumw2();

    /* and finally add the histograms to the list */
    histogramList->Add(fXXValues[currentHarmonic]);
    histogramList->Add(fXYValues[currentHarmonic]);
    histogramList->Add(fYXValues[currentHarmonic]);
    histogramList->Add(fYYValues[currentHarmonic]);

    /* and update the fully filled condition */
    fFullFilled |= harmonicNumberMask[currentHarmonic];
  }

  /* create the entries multidimensional histogram */
  fEntries = new THnI((const char *) entriesHistoName, (const char *) entriesHistoTitle,nVariables,nbins,minvals,maxvals);

  /* now let's set the proper binning and label on each entries histogram axis */
  for (Int_t var = 0; var < nVariables; var++) {
    fEntries->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fEntries->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetVariableLabel());
  }

  /* and finally add the entries histogram to the list */
  histogramList->Add(fEntries);

  delete [] minvals;
  delete [] maxvals;
  delete [] nbins;

  return kTRUE;
}

/// Attaches existing histograms as the support histograms for XX, XY, YX, YY
/// correlation component of the profile function for different harmonics
///
/// The histograms are located in the passed list and if found and with the
/// proper dimensions their references are stored in member variables.
///
/// The harmonic map is inferred from the found histograms within the list
/// that match the naming scheme.
///
/// \param histogramList list where the histograms have to be located
/// \return true if properly attached else false
Bool_t QnCorrectionsProfileCorrelationComponents::AttachHistograms(TList *histogramList) {
  /* let's build the histograms names */
  TString histoXXName = GetName(); histoXXName += szXXCorrelationComponentSuffix;
  TString histoXYName = GetName(); histoXYName += szXYCorrelationComponentSuffix;
  TString histoYXName = GetName(); histoYXName += szYXCorrelationComponentSuffix;
  TString histoYYName = GetName(); histoYYName += szYYCorrelationComponentSuffix;
  TString entriesHistoName = GetName();
  entriesHistoName += szXXCorrelationComponentSuffix;
  entriesHistoName += szXYCorrelationComponentSuffix;
  entriesHistoName += szYXCorrelationComponentSuffix;
  entriesHistoName += szYYCorrelationComponentSuffix;
  entriesHistoName += szEntriesHistoSuffix;

  /* initialize. Remember we don't own the histograms */
  fEntries = NULL;
  if (fXXValues != NULL) {
    delete [] fXXValues;
    fXXValues = NULL;
  }
  if (fXYValues != NULL) {
    delete [] fXYValues;
    fXYValues = NULL;
  }
  if (fYXValues != NULL) {
    delete [] fYXValues;
    fYXValues = NULL;
  }
  if (fYYValues != NULL) {
    delete [] fYYValues;
    fYYValues = NULL;
  }
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
  fFullFilled = 0x0000;

  fEntries = (THnI *) histogramList->FindObject((const char*) entriesHistoName);
  if (fEntries != NULL) {
    /* allocate enough space for the supported harmonic numbers */
    fXXValues = new THnF *[nMaxHarmonicNumberSupported + 1];
    fXYValues = new THnF *[nMaxHarmonicNumberSupported + 1];
    fYXValues = new THnF *[nMaxHarmonicNumberSupported + 1];
    fYYValues = new THnF *[nMaxHarmonicNumberSupported + 1];

    /* search the multidimensional histograms for each harmonic */
    Int_t currentHarmonic = 0;
    for (Int_t i = 0; i < nMaxHarmonicNumberSupported; i++) {
      currentHarmonic++;

      fXXValues[currentHarmonic] = (THnF *) histogramList->FindObject(Form("%s_h%d", (const char *) histoXXName, currentHarmonic));
      fXYValues[currentHarmonic] = (THnF *) histogramList->FindObject(Form("%s_h%d", (const char *) histoXYName, currentHarmonic));
      fYXValues[currentHarmonic] = (THnF *) histogramList->FindObject(Form("%s_h%d", (const char *) histoYXName, currentHarmonic));
      fYYValues[currentHarmonic] = (THnF *) histogramList->FindObject(Form("%s_h%d", (const char *) histoYYName, currentHarmonic));

      /* and update the fully filled condition whether applicable */
      if ((fXXValues[currentHarmonic]  != NULL) && (fXYValues[currentHarmonic] != NULL)
          && (fYXValues[currentHarmonic] != NULL) && (fYYValues[currentHarmonic] != NULL))
      fFullFilled |= harmonicNumberMask[currentHarmonic];
    }
  }
  else
    return kFALSE;

/* TODO: pending to decide whether we divide the histograms and modify the
 * get content and get errors functions accordingly or we split the class
 * in created histograms and attached histograms. So far we leave it as
 * it is and we'll see the penalty for not having them divided.
 */
  /* check that we actually got something */
  if (fFullFilled != 0x0000)
    return kTRUE;
  else
    return kFALSE;
}

/// Get the bin number for the current variable content
///
/// The bin number identifies the event class the current
/// variable content points to.
///
/// \param variableContainer the current variables content addressed by var Id
/// \return the associated bin to the current variables content
Int_t QnCorrectionsProfileCorrelationComponents::GetBin(const Float_t *variableContainer) {
  FillBinAxesValues(variableContainer);
  return fEntries->GetBin(fBinAxesValues);
}

/// Get the XX correlation component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileCorrelationComponents::GetXXBinContent(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fXXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fXXValues[harmonic]->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the XY correlation component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileCorrelationComponents::GetXYBinContent(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fXYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fXYValues[harmonic]->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the YX correlation component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileCorrelationComponents::GetYXBinContent(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fYXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fYXValues[harmonic]->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the YY correlation component bin content for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin number content
Float_t QnCorrectionsProfileCorrelationComponents::GetYYBinContent(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fYYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    return fYYValues[harmonic]->GetBinContent(bin) / Float_t(nEntries);
  }
}

/// Get the XX correlation component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsProfileCorrelationComponents::GetXXBinError(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fXXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  Float_t values = fXXValues[harmonic]->GetBinContent(bin);
  Float_t error2 = fXXValues[harmonic]->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Get the XY correlation component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsProfileCorrelationComponents::GetXYBinError(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fXYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  Float_t values = fXYValues[harmonic]->GetBinContent(bin);
  Float_t error2 = fXYValues[harmonic]->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Get the YX correlation component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsProfileCorrelationComponents::GetYXBinError(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fYXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  Float_t values = fYXValues[harmonic]->GetBinContent(bin);
  Float_t error2 = fYXValues[harmonic]->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Get the YY correlation component bin content error for the passed bin number
/// for the corresponding harmonic
///
/// The bin number identifies a desired event class whose content is
/// error is requested. If the number of entries is lower
/// than the minimum number of entries to validate it
/// the bin is not considered valid and zero is returned.
///
/// \param harmonic the interested external harmonic number
/// \param bin the interested bin number
/// \return the bin content error
Float_t QnCorrectionsProfileCorrelationComponents::GetYYBinError(Int_t harmonic, Int_t bin) {
  Int_t nEntries = fEntries->GetBinContent(bin);

  /* sanity check */
  if (fYYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
    return 0.0;
  }

  Float_t values = fYYValues[harmonic]->GetBinContent(bin);
  Float_t error2 = fYYValues[harmonic]->GetBinError2(bin);

  if (nEntries < nMinNoOfEntriesValidated) {
    return 0.0;
  }
  else {
    Double_t average = values / nEntries;
    Double_t serror = TMath::Sqrt(TMath::Abs(error2 / nEntries - average * average));
    switch (fErrorMode) {
    case kERRORMEAN:
      /* standard error on the mean of the bin values */
      return serror / TMath::Sqrt(nEntries);
      break;
    case kERRORSPREAD:
      /* standard deviation of the bin values */
      return serror;
      break;
    }
  }
}

/// Fills the XX correlation component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight.
/// The entries count is only updated if the whole set for the four components
/// has been already filled. A check is done for detecting consecutive
/// fills for certain harmonic without a previous entries update.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfileCorrelationComponents::FillXX(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  /* first the sanity checks */
  if (fXXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
  }

  if (fXXharmonicFillMask & harmonicNumberMask[harmonic]) {
    QnCorrectionsFatal(Form("Filling twice the harmonic %d before entries update in histogram %s.\n" \
        "   This means you probably have not updated the other components for this harmonic. FIX IT, PLEASE.", harmonic, GetName()));
  }

  /* now it's safe to continue */

  /* keep total entries in fValues updated */
  Double_t nEntries = fXXValues[harmonic]->GetEntries();

  FillBinAxesValues(variableContainer);
  fXXValues[harmonic]->Fill(fBinAxesValues, weight);
  fXXValues[harmonic]->SetEntries(nEntries + 1);

  /* update harmonic fill mask */
  fXXharmonicFillMask |= harmonicNumberMask[harmonic];

  /* now check if time for updating entries histogram */
  if (fXXharmonicFillMask != fFullFilled) return;
  if (fXYharmonicFillMask != fFullFilled) return;
  if (fYXharmonicFillMask != fFullFilled) return;
  if (fYYharmonicFillMask != fFullFilled) return;
  /* update entries and reset the masks */
  fEntries->Fill(fBinAxesValues, 1.0);
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
}

/// Fills the XY correlation component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight.
/// The entries count is only updated if the whole set for the four components
/// has been already filled. A check is done for detecting consecutive
/// fills for certain harmonic without a previous entries update.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfileCorrelationComponents::FillXY(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  /* first the sanity checks */
  if (fXYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
  }

  if (fXYharmonicFillMask & harmonicNumberMask[harmonic]) {
    QnCorrectionsFatal(Form("Filling twice the harmonic %d before entries update in histogram %s.\n" \
        "   This means you probably have not updated the other components for this harmonic. FIX IT, PLEASE.", harmonic, GetName()));
  }

  /* now it's safe to continue */

  /* keep total entries in fValues updated */
  Double_t nEntries = fXYValues[harmonic]->GetEntries();

  FillBinAxesValues(variableContainer);
  fXYValues[harmonic]->Fill(fBinAxesValues, weight);
  fXYValues[harmonic]->SetEntries(nEntries + 1);

  /* update harmonic fill mask */
  fXYharmonicFillMask |= harmonicNumberMask[harmonic];

  /* now check if time for updating entries histogram */
  if (fXXharmonicFillMask != fFullFilled) return;
  if (fXYharmonicFillMask != fFullFilled) return;
  if (fYXharmonicFillMask != fFullFilled) return;
  if (fYYharmonicFillMask != fFullFilled) return;
  /* update entries and reset the masks */
  fEntries->Fill(fBinAxesValues, 1.0);
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
}

/// Fills the YX correlation component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight.
/// The entries count is only updated if the whole set for the four components
/// has been already filled. A check is done for detecting consecutive
/// fills for certain harmonic without a previous entries update.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfileCorrelationComponents::FillYX(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  /* first the sanity checks */
  if (fYXValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
  }

  if (fYXharmonicFillMask & harmonicNumberMask[harmonic]) {
    QnCorrectionsFatal(Form("Filling twice the harmonic %d before entries update in histogram %s.\n" \
        "   This means you probably have not updated the other components for this harmonic. FIX IT, PLEASE.", harmonic, GetName()));
  }

  /* now it's safe to continue */

  /* keep total entries in fValues updated */
  Double_t nEntries = fYXValues[harmonic]->GetEntries();

  FillBinAxesValues(variableContainer);
  fYXValues[harmonic]->Fill(fBinAxesValues, weight);
  fYXValues[harmonic]->SetEntries(nEntries + 1);

  /* update harmonic fill mask */
  fYXharmonicFillMask |= harmonicNumberMask[harmonic];

  /* now check if time for updating entries histogram */
  if (fXXharmonicFillMask != fFullFilled) return;
  if (fXYharmonicFillMask != fFullFilled) return;
  if (fYXharmonicFillMask != fFullFilled) return;
  if (fYYharmonicFillMask != fFullFilled) return;
  /* update entries and reset the masks */
  fEntries->Fill(fBinAxesValues, 1.0);
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
}

/// Fills the YY correlation component for the corresponding harmonic histogram
///
/// The involved bin is computed according to the current variables
/// content. The bin is then increased by the given weight.
/// The entries count is only updated if the whole set for the four components
/// has been already filled. A check is done for detecting consecutive
/// fills for certain harmonic without a previous entries update.
///
/// \param harmonic the interested external harmonic number
/// \param variableContainer the current variables content addressed by var Id
/// \param weight the increment in the bin content
void QnCorrectionsProfileCorrelationComponents::FillYY(Int_t harmonic, const Float_t *variableContainer, Float_t weight) {
  /* first the sanity checks */
  if (fYYValues[harmonic] == NULL) {
    QnCorrectionsFatal(Form("Accessing non allocated harmonic %d in correlation component histogram %s. FIX IT, PLEASE.", harmonic, GetName()));
  }

  if (fYYharmonicFillMask & harmonicNumberMask[harmonic]) {
    QnCorrectionsFatal(Form("Filling twice the harmonic %d before entries update in histogram %s.\n" \
        "   This means you probably have not updated the other components for this harmonic. FIX IT, PLEASE.", harmonic, GetName()));
  }

  /* now it's safe to continue */

  /* keep total entries in fValues updated */
  Double_t nEntries = fYYValues[harmonic]->GetEntries();

  FillBinAxesValues(variableContainer);
  fYYValues[harmonic]->Fill(fBinAxesValues, weight);
  fYYValues[harmonic]->SetEntries(nEntries + 1);

  /* update harmonic fill mask */
  fYYharmonicFillMask |= harmonicNumberMask[harmonic];

  /* now check if time for updating entries histogram */
  if (fXXharmonicFillMask != fFullFilled) return;
  if (fXYharmonicFillMask != fFullFilled) return;
  if (fYXharmonicFillMask != fFullFilled) return;
  if (fYYharmonicFillMask != fFullFilled) return;
  /* update entries and reset the masks */
  fEntries->Fill(fBinAxesValues, 1.0);
  fXXharmonicFillMask = 0x0000;
  fXYharmonicFillMask = 0x0000;
  fYXharmonicFillMask = 0x0000;
  fYYharmonicFillMask = 0x0000;
}

