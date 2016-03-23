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
/// The passed at construction option parameter is the option for the
/// the computation of the  errors in the descendant profiles. Possible
/// values for the options are:
///
///     ' '  (Default) the bin errors are the standard error on the mean of the
///          bin values
///
///     's'            the bin are the standard deviation of of the bin values
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
protected:
  /// \typedef QnCorrectionHistogramErrorMode
  /// \brief The type of bin errors supported by the framework histograms.
  ///
  /// Actually it is not a class because the C++ level of implementation.
  /// But full protection will be reached when were possible declaring it
  /// as a class.
  typedef enum {
    kERRORMEAN = 0,                 ///< the bin errors are the standard error on the mean
    kERRORSPREAD                    ///< the bin errors are the standard deviation
  } QnCorrectionHistogramErrorMode;
public:
  QnCorrectionsHistogramBase();
  QnCorrectionsHistogramBase(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Option_t *option="");
  virtual ~QnCorrectionsHistogramBase();

  virtual Bool_t AttachHistograms(TList *histogramList);
  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup);

  virtual Int_t GetBin(const Float_t *variableContainer);
  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel);
  virtual Float_t GetBinContent(Int_t bin);
  virtual Float_t GetXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYYBinContent(Int_t harmonic, Int_t bin);

  virtual Float_t GetBinError(Int_t bin);
  virtual Float_t GetXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetXXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetXYBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYYBinError(Int_t harmonic, Int_t bin);

  virtual void Fill(const Float_t *variableContainer, Float_t weight);
  virtual void Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight);
  virtual void FillX(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillY(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillXX(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillXY(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillYX(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillYY(Int_t harmonic, const Float_t *variableContainer, Float_t weight);

protected:
  void FillBinAxesValues(const Float_t *variableContainer, Int_t chgrpId = -1);
  THnF* DivideTHnF(THnF* values, THnI* entries);
  void CopyTHnF(THnF *hDest, THnF *hSource, Int_t *binsArray);
  void CopyTHnFDimension(THnF *hDest, THnF *hSource, Int_t *binsArray, Int_t dimension);

  QnCorrectionsEventClassVariablesSet fEventClassVariables;  //!<! The variables set that determines the event classes
  Double_t *fBinAxesValues;                                  //!<! Runtime place holder for computing bin number
  QnCorrectionHistogramErrorMode fErrorMode;                 //!<! The error type for the current instance
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsHistogramBase, 1);
  /// \endcond
  static const char *szChannelAxisTitle;                 ///< The title for the channel extra axis
  static const char *szGroupAxisTitle;                   ///< The title for the channel group extra axis
  static const char *szGroupHistoPrefix;                 ///< The prefix for the name of the group histograms
  static const char *szEntriesHistoSuffix;               ///< The suffix for the name of the entries histograms
  static const char *szXComponentSuffix;                 ///< The suffix for the name of X component histograms
  static const char *szYComponentSuffix;                 ///< The suffix for the name of Y component histograms
  static const char *szXXCorrelationComponentSuffix;     ///< The suffix for the name of XX correlation component histograms
  static const char *szXYCorrelationComponentSuffix;     ///< The suffix for the name of XY correlation component histograms
  static const char *szYXCorrelationComponentSuffix;     ///< The suffix for the name of YX correlation component histograms
  static const char *szYYCorrelationComponentSuffix;     ///< The suffix for the name of YY correlation component histograms
  static const Int_t nMaxHarmonicNumberSupported;        ///< The maximum external harmonic number the framework support
  static const UInt_t harmonicNumberMask[];              ///< Mask for each external harmonic number
  static const Int_t nMinNoOfEntriesValidated;           ///< The minimum number of entries for validating a bin content
};

/// Fills the axes values for the current passed variable container
///
/// Core of the GetBin members. Stores the current values of the involved
/// variables in the internal place holder. Space is prepared for potential
/// channel or group id.
///
/// \param variableContainer the current variables content addressed by var Id
/// \param chgrpId additional optional channel or group Id
inline void QnCorrectionsHistogramBase::FillBinAxesValues(const Float_t *variableContainer, Int_t chgrpId) {
  for (Int_t var = 0; var < fEventClassVariables.GetEntriesFast(); var++) {
    fBinAxesValues[var] = variableContainer[fEventClassVariables.At(var)->GetVariableId()];
  }
  fBinAxesValues[fEventClassVariables.GetEntriesFast()] = chgrpId;
}


/// \class QnCorrectionsHistogramChannelized
/// \brief Single histogram class for the Q vector correction histograms
///
/// Encapsulates a multi dimensional histogram. Each dimension
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
/// histograms stored in a file are draw the channels will appear
/// as enumerated form 0 to the number of active channels handled
/// by the detector configuration that is associated to the histogram
/// and as such by the own histogram.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 11, 2016
class QnCorrectionsHistogramChannelized : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsHistogramChannelized();
  QnCorrectionsHistogramChannelized(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs);
  virtual ~QnCorrectionsHistogramChannelized();

  Bool_t CreateChannelizedHistogram(TList *histogramList, const Bool_t *bUsedChannel);

  /// wrong call for this class invoke base class behaviour
  virtual Int_t GetBin(const Float_t *variableContainer)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer); }
  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel);
  virtual Float_t GetBinContent(Int_t bin);
  virtual Float_t GetBinError(Int_t bin);

  /// wrong call for this class invoke base class behavior
  virtual void Fill(const Float_t *variableContainer, Float_t weight)
  { QnCorrectionsHistogramBase::Fill(variableContainer, weight); }
  virtual void Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight);
private:
  THnF *fValues;              //!<! Cumulates values for each of the event classes
  Bool_t *fUsedChannel;       //!<! array, which of the detector channels is used for this configuration
  Int_t fNoOfChannels;        //!<! The number of channels associated to the whole detector
  Int_t fActualNoOfChannels;  //!<! The actual number of channels handled by the histogram
  Int_t *fChannelMap;         //!<! array, the map from histo to detector channel number

  /// \cond CLASSIMP
  ClassDef(QnCorrectionsHistogramChannelized, 1);
  /// \endcond
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
/// GetBinContent (once the intended bin is obtained by mean
/// of GetBin) returns in the profile way
/// \f[
///    \frac{\Sigma \mbox{fValues(bin)}}{\mbox{fEntries(bin)}}
/// \f]
/// while GetBinError returns the standard deviation of the values
/// in the interested bin
/// \f[
///    \sqrt{\frac{\Sigma \mbox{fValues}^2\mbox{(bin)}}{\mbox{fEntries(bin)}}
///          - \left(\frac{\Sigma \mbox{fValues(bin)}}{\mbox{fEntries(bin)}}\right)^2}
/// \f]
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 11, 2016
class QnCorrectionsProfile : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfile();
  QnCorrectionsProfile(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Option_t *option="");
  virtual ~QnCorrectionsProfile();

  Bool_t CreateProfileHistograms(TList *histogramList);
  virtual Bool_t AttachHistograms(TList *histogramList);
  /// wrong call for this class invoke base class behavior
  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup)
  { return QnCorrectionsHistogramBase::AttachHistograms(histogramList, bUsedChannel, nChannelGroup); }

  virtual Int_t GetBin(const Float_t *variableContainer);
  /// wrong call for this class invoke base class behavior
  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer, nChannel); }
  virtual Float_t GetBinContent(Int_t bin);
  virtual Float_t GetBinError(Int_t bin);

  virtual void Fill(const Float_t *variableContainer, Float_t weight);
  /// wrong call for this class invoke base class behavior
  virtual void Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight)
  { QnCorrectionsHistogramBase::Fill(variableContainer, nChannel, weight); }
private:
  THnF *fValues;   //!<! Cumulates values for each of the event classes
  THnI *fEntries;  //!<! Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfile, 1);
  /// \endcond
};

/// \class QnCorrectionsProfileChannelized
/// \brief Channelized profile class for the Q vector correction histograms
///
/// Encapsulates a multidimensional profile. Each dimension
/// corresponds to one of the event classes variables so,
/// the number of dimensions matches the number of variables within
/// the set passed in the constructor. Additionally incorporates an
/// extra dimension to consider the channel number
///
/// The involved histograms can be created on the fly when needed,
/// and included in a provided list. They cannot be attached to existing
/// ones. In any case they are not destroyed because
/// they are not own by the class but by the involved list.
///
/// Storage efficiency reasons dictate that channels were stored in
/// consecutive order although externally to the class everything is
/// handled with the actual external channel number. But if the
/// histograms stored in a file are draw the channels will appear
/// as enumerated form 0 to the number of active channels handled
/// by the detector configuration that is associated to the histogram
/// and as such by the own histogram.
///
/// GetBinContent (once the intended bin is obtained by mean
/// of GetBin) returns in the profile way
/// \f[
///    \frac{\Sigma \mbox{fValues(bin)}}{\mbox{fEntries(bin)}}
/// \f]
/// while depending on the option passed at construction GetBinError returns
/// * "" the standard error on the mean of the values in the interested bin
/// \f[
///    \frac{\sqrt{\frac{\Sigma \mbox{fValues}^2\mbox{(bin)}}{\mbox{fEntries(bin)}}
///          - \left(\frac{\Sigma \mbox{fValues(bin)}}{\mbox{fEntries(bin)}}\right)^2}}
///         {\sqrt{\mbox{fEntries(bin)}}}
/// \f]
/// * "s" the standard deviation of the values
/// in the interested bin
/// \f[
///    \sqrt{\frac{\Sigma \mbox{fValues}^2\mbox{(bin)}}{\mbox{fEntries(bin)}}
///          - \left(\frac{\Sigma \mbox{fValues(bin)}}{\mbox{fEntries(bin)}}\right)^2}
/// \f]
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 11, 2016
class QnCorrectionsProfileChannelized : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfileChannelized();
  QnCorrectionsProfileChannelized(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Int_t nNoOfChannels,
      Option_t *option="");
  virtual ~QnCorrectionsProfileChannelized();

  Bool_t CreateProfileHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup);

  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel);
  /// wrong call for this class invoke base class behavior
  virtual Int_t GetBin(const Float_t *variableContainer)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer); }
  virtual Float_t GetBinContent(Int_t bin);
  virtual Float_t GetBinError(Int_t bin);

  virtual void Fill(const Float_t *variableContainer, Int_t nChannel, Float_t weight);
  /// wrong call for this class invoke base class behavior
  virtual void Fill(const Float_t *variableContainer,Float_t weight)
  { QnCorrectionsHistogramBase::Fill(variableContainer, weight); }
private:
  THnF *fValues;              //!<! Cumulates values for each of the event classes
  THnI *fEntries;             //!<! Cumulates the number on each of the event classes
  Bool_t *fUsedChannel;       //!<! array, which of the detector channels is used for this configuration
  Int_t *fChannelGroup;       //!<! array, the group to which the channel pertains
  Int_t fNoOfChannels;        //!<! The number of channels associated to the whole detector
  Int_t fActualNoOfChannels;  //!<! The actual number of channels handled by the histogram
  Int_t *fChannelMap;         //!<! array, the map from histo to detector channel number


  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfileChannelized, 1);
  /// \endcond
};

/// \class QnCorrectionsProfileChannelizedIngress
/// \brief Ingress channelized profile class for the Q vector correction histograms
///
/// Encapsulates a multidimensional profile. Each dimension
/// corresponds to one of the event classes variables so,
/// the number of dimensions matches the number of variables within
/// the set passed in the constructor. Additionally incorporates an
/// extra dimension to consider the channel number
///
/// The involved histograms can only be attached to existing ones
/// from a given list. The histograms to attach are the ones created from
/// QnCorrectionsProfileChannelized class. The difference with that class
/// is that now, the histograms are the source for applying corrections
/// and as such should be stored in a more efficient way and the
/// implicit group information be recovered.
///
/// Now the class will own histograms that must be deleted when the
/// class gets destroyed. The behavior regarding the channel handling
/// matches the one from QnCorrectionsProfileChannelized class.
///
/// The entries histogram disappears and so, GetBinContent and GetBinError
/// return in the standard way. Additionally, if applicable, the group
/// histogram is created and GetGrpBinContent and GetGrpBinError are
/// functional.
///
/// The profile as such cannot be filled. It should be considered as a
/// read only profile.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 23, 2016
class QnCorrectionsProfileChannelizedIngress : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfileChannelizedIngress();
  QnCorrectionsProfileChannelizedIngress(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Int_t nNoOfChannels,
      Option_t *option="");
  virtual ~QnCorrectionsProfileChannelizedIngress();

  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup);
  /// wrong call for this class invoke base class behavior
  virtual Bool_t AttachHistograms(TList *histogramList)
  { return QnCorrectionsHistogramBase::AttachHistograms(histogramList); }

  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel);
  virtual Int_t GetGrpBin(const Float_t *variableContainer, Int_t nChannel);
  /// wrong call for this class invoke base class behavior
  virtual Int_t GetBin(const Float_t *variableContainer)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer); }
  virtual Float_t GetBinContent(Int_t bin);
  virtual Float_t GetGrpBinContent(Int_t bin);
  virtual Float_t GetBinError(Int_t bin);
  virtual Float_t GetGrpBinError(Int_t bin);

private:
  THnF *fValues;              //!<! the values and errors on each event class and channel
  THnF *fGroupValues;         //!<! the values and errors on each event class and group
  Bool_t *fUsedChannel;       //!<! array, which of the detector channels are used for this configuration
  Int_t *fChannelGroup;       //!<! array, the group to which the channel pertains
  Int_t fNoOfChannels;        //!<! The number of channels associated to the whole detector
  Int_t fActualNoOfChannels;  //!<! The actual number of channels handled by the histogram
  Int_t *fChannelMap;         //!<! array, the map from histo to detector channel number
  Bool_t fUseGroups;          //!<! the groups structures must be used
  Bool_t *fUsedGroup;         //!<! array, which of the detector groups are used for this configuration
  Int_t fNoOfGroups;          //!<! the number of groups associated with the whole detector
  Int_t fActualNoOfGroups;    //!<! The actual number of groups handled by the histogram
  Int_t *fGroupMap;           //!<! array, the map from histo to detector channel group number


  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfileChannelizedIngress, 1);
  /// \endcond
};

/// \class QnCorrectionsProfileComponents
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
/// be passed to the constructor. Of course,  the base name and base
/// title for the different histograms has to be also provided.
/// At creation time the required number of harmonics and an optional
/// expected harmonic numbering scheme has to be passed.
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
class QnCorrectionsProfileComponents : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfileComponents();
  QnCorrectionsProfileComponents(const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Option_t *option="");
  virtual ~QnCorrectionsProfileComponents();

  Bool_t CreateComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
  virtual Bool_t AttachHistograms(TList *histogramList);
  /// wrong call for this class invoke base class behavior
  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup)
  { return QnCorrectionsHistogramBase::AttachHistograms(histogramList, bUsedChannel, nChannelGroup); }

  virtual Int_t GetBin(const Float_t *variableContainer);
  /// wrong call for this class invoke base class behavior
  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer, nChannel); }
  virtual Float_t GetXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYBinError(Int_t harmonic, Int_t bin);

  virtual void FillX(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillY(Int_t harmonic, const Float_t *variableContainer, Float_t weight);

private:
  THnF **fXValues;            //!<! X component histogram for each requested harmonic
  THnF **fYValues;            //!<! Y component histogram for each requested harmonic
  UInt_t fXharmonicFillMask;  //!<! keeps track of harmonic X component filled values
  UInt_t fYharmonicFillMask;  //!<! keeps track of harmonic Y component filled values
  UInt_t fFullFilled;         //!<! mask for the fully filled condition
  THnI  *fEntries;            //!<! Cumulates the number on each of the event classes
  /// \cond CLASSIMP
  ClassDef(QnCorrectionsProfileComponents, 1);
  /// \endcond
};

/// \class QnCorrectionsProfileCorrelationComponents
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
class QnCorrectionsProfileCorrelationComponents : public QnCorrectionsHistogramBase {
public:
  QnCorrectionsProfileCorrelationComponents();
  QnCorrectionsProfileCorrelationComponents(
      const char *name,
      const char *title,
      QnCorrectionsEventClassVariablesSet &ecvs,
      Option_t *option="");
  virtual ~QnCorrectionsProfileCorrelationComponents();

  Bool_t CreateCorrelationComponentsProfileHistograms(TList *histogramList, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
  virtual Bool_t AttachHistograms(TList *histogramList);
  /// wrong call for this class invoke base class behavior
  virtual Bool_t AttachHistograms(TList *histogramList, const Bool_t *bUsedChannel, const Int_t *nChannelGroup)
  { return QnCorrectionsHistogramBase::AttachHistograms(histogramList, bUsedChannel, nChannelGroup); }

  virtual Int_t GetBin(const Float_t *variableContainer);
  /// wrong call for this class invoke base class behavior
  virtual Int_t GetBin(const Float_t *variableContainer, Int_t nChannel)
  { return QnCorrectionsHistogramBase::GetBin(variableContainer, nChannel); }
  virtual Float_t GetXXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYXBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetYYBinContent(Int_t harmonic, Int_t bin);
  virtual Float_t GetXXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetXYBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYXBinError(Int_t harmonic, Int_t bin);
  virtual Float_t GetYYBinError(Int_t harmonic, Int_t bin);

  virtual void FillXX(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillXY(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillYX(Int_t harmonic, const Float_t *variableContainer, Float_t weight);
  virtual void FillYY(Int_t harmonic, const Float_t *variableContainer, Float_t weight);

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
  ClassDef(QnCorrectionsProfileCorrelationComponents, 1);
  /// \endcond
};


#endif
