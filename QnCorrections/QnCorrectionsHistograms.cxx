#include "QnCorrectionsEventClasses.h"
#include "QnCorrectionsHistograms.h"

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
QnCorrectionsProfile::~QnCorrectionsProfile() {

}

Bool_t QnCorrectionsProfile::CreateProfileHistograms(TList *histogramList) {
  return kTRUE;
}
Bool_t QnCorrectionsProfile::AttachProfileHistograms(TList *histogramList) {
  return kTRUE;
}

