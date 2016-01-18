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
/// The encapsulated bin axes values provide an efficient
/// runtime storage for computing bin numbers.
///
/// Provides the interface for the whole set of histogram
/// classes providing error information that helps debugging.
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

  /// Attaches existing histograms as the supporting histograms
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param variableContainer the current variables content addressed by var Id
  /// \return the associated bin to the current variables content
  virtual Bool_t AttachHistograms(TList *histogramList) { return kFALSE; }

  /// Get the bin number for the current variable content
  ///
  /// The bin number identifies the event class the current
  /// variable content points to.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param variableContainer the current variables content addressed by var Id
  /// \return the associated bin to the current variables content
  virtual Int_t GetBin(Float_t *variableContainer) { return -1; }

  /// Get the bin content for the passed bin number
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetBinContent(Int_t bin) { return 0.0; }

  /// Get the X component bin content for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetXBinContent(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the Y component bin content for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetYBinContent(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the bin XX component content for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetXXBinContent(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the bin XY component content for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetXYBinContent(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the bin YX component content for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetYXBinContent(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the bin YY component content for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin number content
  virtual Float_t GetYYBinContent(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the bin content error for the passed bin number
  ///
  /// The bin number identifies a desired event class whose content
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param bin the interested bin number
  /// \return the bin number content error
  virtual Float_t GetBinError(Int_t bin) {return 0.0; }

  /// Get the X component bin content error for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin content error
  virtual Float_t GetXBinError(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the Y component bin content error for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin content error
  virtual Float_t GetYBinError(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the XX component bin content error for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin content error
  virtual Float_t GetXXBinError(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the XY component bin content error for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin content error
  virtual Float_t GetXYBinError(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the YX component bin content error for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin content error
  virtual Float_t GetYXBinError(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Get the YY component bin content error for the passed bin number
  /// for the corresponding harmonic
  ///
  /// The bin number identifies a desired event class whose content is
  /// error is requested
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param bin the interested bin number
  /// \return the bin content error
  virtual Float_t GetYYBinError(Int_t harmonic, Int_t bin) { return 0.0; }

  /// Fills the histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void Fill(Float_t *variableContainer, Float_t weight) {}

  /// Fills the X component for the corresponding harmonic histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param bin the interested bin number
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void FillX(Int_t harmonic, Float_t *variableContainer, Float_t weight) { }

  /// Fills the Y component for the corresponding harmonic histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void FillY(Int_t harmonic, Float_t *variableContainer, Float_t weight) { }

  /// Fills the XX component for the corresponding harmonic histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void FillXX(Int_t harmonic, Float_t *variableContainer, Float_t weight) { }

  /// Fills the XY component for the corresponding harmonic histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void FillXY(Int_t harmonic, Float_t *variableContainer, Float_t weight) { }

  /// Fills the YX component for the corresponding harmonic histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void FillYX(Int_t harmonic, Float_t *variableContainer, Float_t weight) { }

  /// Fills the YY component for the corresponding harmonic histogram
  ///
  /// The involved bin is computed according to the current variables
  /// content. The bin is then increased by the given weight and the
  /// entries also increased properly.
  ///
  /// Interface declaration function.
  /// Default behavior. Base class should not be instantiated.
  ///
  /// \param harmonic the interested external harmonic number
  /// \param variableContainer the current variables content addressed by var Id
  /// \param weight the increment in the bin content
  virtual void FillYY(Int_t harmonic, Float_t *variableContainer, Float_t weight) { }

private:
  void FillBinAxesValues(Float_t *variableContainer);

  QnCorrectionsEventClassVariablesSet fEventClassVariables;  ///< The variables set that determines the event classes
  Double_t *fBinAxesValues; ///< Runtime place holder for computing bin number
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsHistogramBase, 1);
  /// \endcond
  static const char *szEntriesHistoSuffix;              ///< The suffix for the name of the entries histograms
  static const char *szXComponentSuffix;                ///< The suffix for the name of X component histograms
  static const char *szYComponentSuffix;                ///< The suffix for the name of Y component histograms
  static const char *szXXCorrelationComponentSuffix;     ///< The suffix for the name of XX correlation component histograms
  static const char *szXYCorrelationComponentSuffix;     ///< The suffix for the name of XY correlation component histograms
  static const char *szYXCorrelationComponentSuffix;     ///< The suffix for the name of YX correlation component histograms
  static const char *szYYCorrelationComponentSuffix;     ///< The suffix for the name of YY correlation component histograms
  static const Int_t nMaxHarmonicNumberSupported;       ///< The maximum external harmonic number the framework support
  static const UInt_t harmonicNumberMask[];             ///< Mask for each external harmonic number
};

/// Fills the axes values for the current passed variable container
///
/// Core of the GetBin members. Stores the current values of the involved
/// variables in the internal place holder.
///
/// \param variableContainer the current variables content addressed by var Id
inline void QnCorrectionsHistogramBase::FillBinAxesValues(Float_t *variableContainer) {
  for (Int_t var = 0; var < fEventClassVariables.GetEntriesFast(); var++) {
    fBinAxesValues[var] = variableContainer[fEventClassVariables.At(var)->GetVariableId()];
  }
}


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
/// GetBinContent (once the intended bin is obtained by mean
/// of GetBin) returns in the profile way
/// \f[
///    \frac{\Sigma fValues(bin)}{fEntries(bin)}
/// \f]
/// while GetBinError returns the standard deviation of the values
/// in the interested bin
/// \f[
///    \sqrt{\frac{\Sigma fValues^2(bin)}{fEntries(bin)}
///          - \left(\frac{\Sigma fValues(bin)}{fEntries(bin)}\right)^2}
/// \f]
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
  virtual Bool_t AttachHistograms(TList *histogramList);

  virtual Int_t GetBin(Float_t *variableContainer);
  virtual Float_t GetBinContent(Int_t bin);
  virtual Float_t GetBinError(Int_t bin);

  virtual void Fill(Float_t *variableContainer, Float_t weight);
private:
  THnF *fValues;   ///< Cumulates values for each of the event classes
  THnI *fEntries;  ///< Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfile, 1);
  /// \endcond
};

/// \class QnCorrectionsComponentsProfile
/// \brief Base class for the components based set of profiles
///
/// Provides profile histograms for storing component, X, Y, based
/// information for a set of predefined harmonics defined at creation
/// time. The user can select the harmonic addressing procedure so that
/// it will possible to ask for just one harmonic support and assign
/// to it any desired number.
///
/// As any histogram derived from QnCorrectionsHistogramBase the set
/// of variables that identify the different event classes has to
/// be passed to the constructor together with the required number of
/// harmonics and an optional harmonic expected numbering scheme.
/// Of course,  the base name and base title for the different
/// histograms has to be also provided.
///
/// The harmonic map passed should contain an ordered array with
/// as many items as requested harmonics that provides the external
/// number to be used for request the corresponding harmonic.
/// Requesting five harmonics without maps is equivalent to pass
/// {1,2,3,4,5} as map. Requesting just support for the harmonic
/// four will require a map {4}.
///
/// When you fill the histograms care is taken for not increasing
/// the number of entries until all components for the whole set of
/// harmonics have been filled. If you try to fill twice a harmonic
/// component before the whole set is filled you will get an execution
/// error because you are doing something that shall be corrected
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 15, 2016
class QnCorrectionsComponentsProfile : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsComponentsProfile();
  QnCorrectionsComponentsProfile(const char *name, const char *title, QnCorrectionsEventClassVariablesSet &ecvs);
  virtual ~QnCorrectionsComponentsProfile();

  Bool_t CreateComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
  virtual Bool_t AttachHistograms(TList *histogramList);

  virtual Int_t GetBin(Float_t *variableContainer);
  virtual Float_t GetXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYBinError(Int_t harmonic, Int_t bin);

  virtual void FillX(Int_t harmonic, Float_t *variableContainer, Float_t weight);
  virtual void FillY(Int_t harmonic, Float_t *variableContainer, Float_t weight);

private:
  THnF **fXValues;            ///< X component histogram for each requested harmonic
  THnF **fYValues;            ///< Y component histogram for each requested harmonic
  UInt_t fXharmonicFillMask;  ///< keeps track of harmonic X component filled values
  UInt_t fYharmonicFillMask;  ///< keeps track of harmonic Y component filled values
  UInt_t fFullFilled;         ///< mask for the fully filled condition
  THnI  *fEntries;  ///< Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsComponentsProfile, 1);
  /// \endcond
};

/// \class QnCorrectionsCorrelationComponentsProfile
/// \brief Base class for the correlation components based set of profiles
///
/// Provides profile histograms for storing component, XX, XY, YX, YY, based
/// information for a set of predefined harmonics defined at creation
/// time. The user can select the harmonic addressing procedure so that
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
class QnCorrectionsCorrelationComponentsProfile : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsCorrelationComponentsProfile();
  QnCorrectionsCorrelationComponentsProfile(
      const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs);
  virtual ~QnCorrectionsCorrelationComponentsProfile();

  Bool_t CreateCorrelationComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
  virtual Bool_t AttachHistograms(TList *histogramList);

  virtual Int_t GetBin(Float_t *variableContainer);
  virtual Float_t GetXXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetXYBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYYBinError(Int_t harmonic, Int_t bin);

  virtual void FillXX(Int_t harmonic, Float_t *variableContainer, Float_t weight);
  virtual void FillXY(Int_t harmonic, Float_t *variableContainer, Float_t weight);
  virtual void FillYX(Int_t harmonic, Float_t *variableContainer, Float_t weight);
  virtual void FillYY(Int_t harmonic, Float_t *variableContainer, Float_t weight);

private:
  THnF **fXXValues;  ///< XX component histogram for each requested harmonic
  THnF **fXYValues;  ///< XY component histogram for each requested harmonic
  THnF **fYXValues;  ///< YX component histogram for each requested harmonic
  THnF **fYYValues;  ///< YY component histogram for each requested harmonic
  UInt_t fXXharmonicFillMask;  ///< keeps track of harmonic XX component filled values
  UInt_t fXYharmonicFillMask;  ///< keeps track of harmonic XY component filled values
  UInt_t fYXharmonicFillMask;  ///< keeps track of harmonic YX component filled values
  UInt_t fYYharmonicFillMask;  ///< keeps track of harmonic YY component filled values
  UInt_t fFullFilled;         ///< mask for the fully filled condition
  THnI  *fEntries;  ///< Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsCorrelationComponentsProfile, 1);
  /// \endcond
};


#endif
