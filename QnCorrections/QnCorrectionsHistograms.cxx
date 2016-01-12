#include "QnCorrectionsEventClasses.h"
#include "QnCorrectionsHistograms.h"

/// The suffix for the name of the entries histograms
const char *QnCorrectionsHistogramBase::szEntriesHistoSuffix = "_entries";

/// \cond CLASSIMP
ClassImp(QnCorrectionsHistogramBase);
/// \endcond

/// Default constructor
QnCorrectionsHistogramBase::QnCorrectionsHistogramBase() :
  TNamed(),
  fEventClassVariables() {

}

/// Default destructor
QnCorrectionsHistogramBase::~QnCorrectionsHistogramBase() {

}

/// Normal constructor
///
/// Basically stores the set of variables that identify
/// the different event classes the involved histograms
/// are storing information about
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
///
QnCorrectionsHistogramBase::QnCorrectionsHistogramBase(const char *name, const char *title, QnCorrectionsEventClassVariablesSet &ecvs) :
  TNamed(name, title),
  fEventClassVariables(ecvs) {

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
/// different event classes passing the to its parent
/// and prepares the object for actual histogram
/// creation or attachment
///
/// \param name base for the name of the histograms
/// \param title base for the title of the histograms
/// \param ecvs the event classes variables set
///
QnCorrectionsProfile::QnCorrectionsProfile(const char *name, const char *title, QnCorrectionsEventClassVariablesSet &ecvs):
    QnCorrectionsHistogramBase(name, title, ecvs) {
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
  TString entriesHistoName = GetName() + szEntriesHistoSuffix;
  TString entriesHistoTitle = GetTitle() + szEntriesHistoSuffix;

  Int_t nVariables = fEventClassVariables.GetEntriesFast();

  Double_t *minvals = new Double_t[nVariables];
  Double_t *maxvals = new Double_t[nVariables];
  Int_t *nbins = new Int_t[nVariables];
  TString sVariableLabels = "";

  /* get the multidimensional structure */
  fEventClassVariables.GetMultidimensionalConfiguration(nbins,minvals,maxvals);

  /* create the values and entries multidimensional histograms */
  fValues = new THnF((const char *) histoName, (const char *) histoTitle,nVariables,nbins,minvals,maxvals);
  fEntries = new THnF((const char *) entriesHistoName, (const char *) entriesHistoTitle,nVariables,nbins,minvals,maxvals);

  /* now let's set the proper binning and label on each axis */
  for (Int_t var = 0; var < nVariables; var++) {
    fValues->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fEntries->GetAxis(var)->Set(fEventClassVariables.At(var)->GetNBins(),fEventClassVariables.At(var)->GetBins());
    fValues->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetName());
    fEntries->GetAxis(var)->SetTitle(fEventClassVariables.At(var)->GetName());
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
/// The histograms are located in the passed list and if found and whith th
/// proper dimensions their references are stored in member variables.
///
/// \param histogramList list where the histograms have to be located
/// \return true if properly attached else false
Bool_t QnCorrectionsProfile::AttachProfileHistograms(TList *histogramList) {
  /* let's build the histograms names */
  TString histoName = GetName();
  TString entriesHistoName = GetName() + szEntriesHistoSuffix;

  /* initialize. Remember we don't own the histograms */
  fEntries = fValues = NULL;

  fEntries = histogramList->FindObject((const char*) entriesHistoName);
  if (fEntries != NULL) {
    fValues = histogramList->FindObject((const char *)histoName);
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

