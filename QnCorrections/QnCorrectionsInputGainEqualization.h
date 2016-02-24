#ifndef QNCORRECTIONS_INPUTGAINEQUALIZATION_H
#define QNCORRECTIONS_INPUTGAINEQUALIZATION_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsInputGainEqualization.h
/// \brief Definition of the class that implements gain equalization of individual channels.
///
/// Gain equalization is applied on raw data from the involved detector.
/// Two procedures are implemented: average gain equalization and width equalization.
///
/// The average gain equalization is applied for the signal \f$ \mbox{M}_{c,i} \f$ of each detector
/// channel \f$ c \f$ measured for event \f$ i \f$ according to:
/// \f[
///        \mbox{M}'_{c,i} = \frac{\mbox{M}_{c,i}}{\langle\mbox{M}_{c}\rangle}
/// \f]
/// where  \f$\langle\mbox{M}_{c}\rangle\f$ is an average over events in a given event class
/// \f[
///        \langle\mbox{M}_{c}\rangle = \frac{1}{\mbox{N}_{ev}} \Sigma_{i}^{\mbox{N}_{ev}} \mbox{M}_{c,i}
/// \f]
///
/// The width equalization is applied for the signal \f$ \mbox{M}_{c,i} \f$ of each detector
/// channel \f$ c \f$ measured for event \f$ i \f$ according to:
/// \f[
///     \mbox{M}'_{c,i} = \mbox{A} + \mbox{B} \frac{\mbox{M}_{c,i} - \langle\mbox{M}_{c}\rangle}
///                               {\sigma_{{M}_{c}}}
/// \f]
/// with A and B are parameters that are the same for all channels and
/// \f$\sigma_{{M}_{c}}\f$ is the standard deviation of the signals of the channel \f$c\f$
/// for the considered event class.
/// \f[
///        \sigma_{{M}_{c}} = \sqrt{
///          \frac{1}{\mbox{N}_{ev}} \Sigma_{i}^{\mbox{N}_{ev}} \mbox{M}^2_{c,i} -
///          \frac{1}{\mbox{N}^2_{ev}} \left(\Sigma_{i}^{\mbox{N}_{ev}} \mbox{M}_{c,i}\right)^2}
/// \f]
///
/// The gain equalization is only applied if the class instance
/// is in the correction status. In order to be in that status the instance
/// should have been able to get the proper calibration histograms that will
/// provide the required averages per event class and channel.
/// If the class instance is not in the correction status then, it is
/// in the calibration one, collecting data for producing, once merged in a
/// further phase, the calibration histograms.


/// \typedef QnGainEqualizationMethod
/// \brief The class of the id of the supported gain equalization methods
///
/// Actually it is not a class because the C++ level of implementation.
/// But full protection will be reached when were possible declaring it
/// as a class.
///
typedef enum {
  QEQUAL_noEqualization,         ///< \f$ \mbox{M'} = \mbox{M}\f$
  QEQUAL_averageEqualization,    ///< \f$ \mbox{M}' = \frac{\mbox{M}}{\langle\mbox{M}\rangle} \f$
  QEQUAL_widthEqualization,      ///< \f$ \mbox{M}' = \mbox{A} + \mbox{B} \frac{\mbox{M} - \langle\mbox{M} \rangle}{\sigma_{{M}}} \f$
} QnGainEqualizationMethod;



class QnCorrectionsInputGainEqualization : public QnCorrectionsCorrectionOnInputData {
public:
  QnCorrectionsInputGainEqualization();
  ~QnCorrectionsInputGainEqualization();

  /// Stores the passed equalization method
  /// \param method the desired equalization method
  void SetEqualizationMethod(QnGainEqualizationMethod method)
  { fEqualizationMethod = method; }

  /// Attaches the needed input information to the correction step
  ///
  /// Pure virtual function
  /// \param list list where the inputs should be found
  /// \return kTRUE if everything went OK
  virtual Bool_t AttachInput(TList *list);
  /// Asks for support histograms creation
  ///
  /// Pure virtual function
  /// \param list list where the histograms should be incorporated for its persistence
  /// \return kTRUE if everything went OK
  virtual Bool_t CreateSupportHistograms(TList *list);

  virtual Bool_t Process(const Float_t *variableContainer);
  /// Clean the correction to accept a new event
  /// Does nothing for the time being
  virtual void ClearCorrectionStep() {}

private:
  static const Float_t  fMinimumSignificantValue;     ///< the minimum value that will be considered as meaningful for processing
  static const char *szCorrectionName;               ///< the name of the correction step
  static const char *szKey;                          ///< the key of the correction step for ordering purpose
  QnCorrectionsProfileChannelizedIngress *fInputHistograms; ///< the histogram with calibration information
  QnCorrectionsProfileChannelized *fCalibrationHistograms; ///< the histogram for building calibration information
  QnGainEqualizationMethod fEqualizationMethod; ///< the selected equalization method

/// \cond CLASSIMP
  ClassDef(QnCorrectionsInputGainEqualization, 1);
/// \endcond
};

#endif // QNCORRECTIONS_INPUTGAINEQUALIZATION_H
