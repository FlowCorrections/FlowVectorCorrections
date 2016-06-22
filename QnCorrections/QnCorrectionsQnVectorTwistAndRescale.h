#ifndef QNCORRECTIONS_QNVECTORTWISTANDRESCALE_H
#define QNCORRECTIONS_QNVECTORTWISTANDRESCALE_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsQnVectorTwistAndRescale.h
/// \brief Definition of the class that implements twist and rescale on Q vectors.
///
/// Twist and rescale are applied on ongoing Q vector from the involved detector
/// configuration. It is supposed that such a Q vector has already been built after input data
/// equalization and is with the proper normalization for the twist and rescaling corrections being applied.
/// Both twist and rescale application are user configurable.
///
/// Twist correction is applied according to:
/// \f[
///        Q_{n,(x,y)}' = \frac{Q_{n,(x,y)} - \Lambda^{s(+,-)}_{2n} Q_{n,(x,y)}}{1 - \Lambda^{s-}_{2n}\Lambda^{s+}_{2n}}
/// \f]
///
/// The rescaling correction is applied according to:
/// \f[
///     Q''_{n(x,y)} = \frac{Q'_{n(x,y)}}{A_{2n}^{(+,-)}}
/// \f]
/// Parameters \f$ A_{2n}^{(+,-)} \f$ and \f$ \Lambda^{s(+,-)}_{2n} \f$ are extracted from data according to the
/// method selected for the twist and rescale correction. Currently two methods are supported: the double harmonic
/// method and the correlations method.
///
/// For the double harmonic method
/// \f[
///     A^{\pm}_{2n} = 1 \pm \langle X_{2n} \rangle
/// \f]
/// and
/// \f[
///     \Lambda ^{\pm}_{2n} = \frac{\langle Y_{2n} \rangle}{A^{\pm}_{2n}}
/// \f]
/// For the correlations method two additional subdetectors need to be configured. Let these two subdetectors be
/// denominated \f$ A \f$ and \f$ C \f$ being \f$ B \f$ the current subdetector on which the twist and rescaling
/// correction will be applied. Then
///
/// \f[
/// A^{B+}_{2n} = \frac{\sqrt{2 \langle X^{A}_{2n} X^{B}_{2n} \rangle}\langle X^{A}_{2n} X^{C}_{2n} \rangle}
/// {\langle X^{A}_{2n} X^{C}_{2n} \rangle \langle X^{B}_{2n} X^{C}_{2n} \rangle + \langle X^{A}_{2n} Y^{C}_{2n} \rangle \langle X^{B}_{2n} Y^{C}_{2n} \rangle},
/// \f]
///
/// \f[
/// A^{B-}_{2n} = \frac{\sqrt{2 \langle X^{A}_{2n} X^{B}_{2n} \rangle}\langle Y^{A}_{2n} Y^{C}_{2n} \rangle}
/// {\langle X^{A}_{2n} X^{C}_{2n} \rangle \langle X^{B}_{2n} X^{C}_{2n} \rangle + \langle X^{A}_{2n} Y^{C}_{2n} \rangle \langle X^{B}_{2n} Y^{C}_{2n} \rangle},
/// \f]
///
/// \f[
///  \Lambda^{B,+}_{2n} =  \frac{\langle X^{A}_{2n} Y^{B}_{2n} \rangle}{\langle X^{A}_{2n} X^{B}_{2n} \rangle}
/// \f]
/// and
/// \f[
///  \Lambda^{B,-}_{2n} =  \frac{\langle Y^{A}_{2n} X^{B}_{2n} \rangle}{\langle Y^{A}_{2n} Y^{B}_{2n} \rangle}
/// \f]
///
/// Twist and rescale are only applied if the class instance
/// is in the correction status. In order to be in that status the instance
/// should have been able to get the proper correction histograms that will
/// provide the required averages per event class.
/// If the class instance is not in the correction status then, it is
/// in the calibration one, collecting data for producing, once merged in a
/// further phase, the correction histograms.
///
/// Data collection for twist and rescale correction parameters building is
/// performed on plain Qn vector and by using Q2n vector structure, i.e, the
/// construction of the correction parameters is done by using the two multiple of
/// the harmonics defined within the involved detector configuration
///
/// Correction is performed for all harmonics
/// defined within the involved detector configuration

#include "QnCorrectionsCorrectionOnQvector.h"

/// \class QnCorrectionsQnVectorTwistAndRescale
/// \brief Encapsulates twist and rescale on Q vector
///
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jun 22, 2016
///
/// Twist and rescale are applied on ongoing Q vector from the involved detector
/// configuration.
///
/// The recentering is applied according to:
/// \f[
///        Q' = Q - {\langle Q \rangle}
/// \f]
/// where  \f$\langle Q \rangle\f$ is an average over events in a given event class
/// \f[
///        \langle Q \rangle = \frac{1}{\mbox{N}_{ev}} \sum_{i}^{\mbox{N}_{ev}} Q_i
/// \f]
///
/// Twist correction is applied according to:
/// \f[
///        Q_{n,(x,y)}' = \frac{Q_{n,(x,y)} - \Lambda^{s(+,-)}_{2n} Q_{n,(x,y)}}{1 - \Lambda^{s-}_{2n}\Lambda^{s+}_{2n}}
/// \f]
///
/// The rescaling correction is applied according to:
/// \f[
///     Q''_{n(x,y)} = \frac{Q'_{n(x,y)}}{A_{2n}^{(+,-)}}
/// \f]
/// Parameters \f$ A_{2n}^{(+,-)} \f$ and \f$ \Lambda^{s(+,-)}_{2n} \f$ are extracted from data according to the
/// method selected for the twist and rescale correction. Currently two methods are supported: the double harmonic
/// method and the correlations method.
///
/// Recentering (and width equalization) is only applied if the class instance
/// is in the correction status. In order to be in that status the instance
/// should have been able to get the proper correction histograms that will
/// provide the required averages per event class.
/// If the class instance is not in the correction status then, it is
/// in the calibration one, collecting data for producing, once merged in a
/// further phase, the correction histograms.
///
/// Data collecting during calibration is performed for all harmonics
/// defined within the involved detector configuration

class QnCorrectionsHistogramSparse;

class QnCorrectionsQnVectorTwistAndRescale : public QnCorrectionsCorrectionOnQvector {
public:
   /// \enum QnTwistAndRescaleMethod
   /// \brief The class of the id of the supported twist and rescale methods
   ///
   /// Actually it is not a class because the C++ level of implementation.
   /// But full protection will be reached when were possible declaring it
   /// as a class.
   ///
   enum QnTwistAndRescaleMethod {
     TWRESCALE_doubleHarmonic,      ///< \f$ A^{\pm}_{2n} = 1 \pm \langle X_{2n} \rangle, \qquad \Lambda ^{\pm}_{2n} = \frac{\langle Y_{2n} \rangle}{A^{\pm}_{2n}} \f$
     TWRESCALE_correlations,    ///< \f$ A^{B+}_{2n} = \frac{\sqrt{2 \langle X^{A}_{2n} X^{B}_{2n} \rangle}\langle X^{A}_{2n} X^{C}_{2n} \rangle}
     /// {\langle X^{A}_{2n} X^{C}_{2n} \rangle \langle X^{B}_{2n} X^{C}_{2n} \rangle + \langle X^{A}_{2n} Y^{C}_{2n} \rangle \langle X^{B}_{2n} Y^{C}_{2n} \rangle}, \\
     /// A^{B-}_{2n} = \frac{\sqrt{2 \langle X^{A}_{2n} X^{B}_{2n} \rangle}\langle Y^{A}_{2n} Y^{C}_{2n} \rangle}
     /// {\langle X^{A}_{2n} X^{C}_{2n} \rangle \langle X^{B}_{2n} X^{C}_{2n} \rangle + \langle X^{A}_{2n} Y^{C}_{2n} \rangle \langle X^{B}_{2n} Y^{C}_{2n} \rangle}, \\
     ///  \Lambda^{B,+}_{2n} =  \frac{\langle X^{A}_{2n} Y^{B}_{2n} \rangle}{\langle X^{A}_{2n} X^{B}_{2n} \rangle}, \quad
     ///  \Lambda^{B,-}_{2n} =  \frac{\langle Y^{A}_{2n} X^{B}_{2n} \rangle}{\langle Y^{A}_{2n} Y^{B}_{2n} \rangle} \f$
   };

   QnCorrectionsQnVectorTwistAndRescale();
  ~QnCorrectionsQnVectorTwistAndRescale();

  /// Controls if twist step shall be applied
  /// \param apply kTRUE for applying the twist step
  void SetApplyTwist(Bool_t apply)
  { fApplyTwist = apply; }
  /// Controls if rescale step shall be applied
  /// \param apply kTRUE for applying the rescale step
  void SetApplyRescale(Bool_t apply)
  { fApplyRescale = apply; }
  void SetReferenceConfigurationsForTwistAndRescale(const char *nameA, const char *nameC);
  /// Set the minimum number of entries for calibration histogram bin content validation
  /// \param nNoOfEntries the number of entries threshold
  void SetNoOfEntriesThreshold(Int_t nNoOfEntries) { fMinNoOfEntriesToValidate = nNoOfEntries; }

  /// Informs when the detector configuration has been attached to the framework manager
  /// Basically this allows interaction between the different framework sections at configuration time
  /// No action for Qn vector recentering
  virtual void AttachedToFrameworkManager() {}
  virtual Bool_t AttachInput(TList *list);
  virtual void CreateSupportDataStructures();
  virtual Bool_t CreateSupportHistograms(TList *list);
  virtual Bool_t CreateQAHistograms(TList *list);
  virtual Bool_t CreateNveQAHistograms(TList *list);

  virtual Bool_t ProcessCorrections(const Float_t *variableContainer);
  virtual Bool_t ProcessDataCollection(const Float_t *variableContainer);
  virtual void ClearCorrectionStep();
  virtual Bool_t ReportUsage(TList *calibrationList, TList *applyList);

private:
  static const Int_t fDefaultMinNoOfEntries;         ///< the minimum number of entries for bin content validation
  static const char *szTwistCorrectionName;          ///< the name of the twist correction step
  static const char *szRescaleCorrectionName;        ///< the name of the rescale correction step
  static const char *szKey;                          ///< the key of the correction step for ordering purpose
  static const char *szDoubleHarmonicSupportHistogramName;         ///< the name and title for double harmonic method support histograms
  static const char *szCorrelationsSupportHistogramName;         ///< the name and title for correlations method support histograms
  static const char *szTwistCorrectedQnVectorName;        ///< the name of the Qn vector after applying the twist correction
  static const char *szRescaleCorrectedQnVectorName;        ///< the name of the Qn vector after applying the rescale correction
  static const char *szQANotValidatedHistogramName;  ///< the name and title for bin not validated QA histograms
  QnCorrectionsProfileComponents *fDoubleHarmonicInputHistograms; //!<! the histogram with calibration information for the double harmonic method
  QnCorrectionsProfileComponents *fDoubleHarmonicCalibrationHistograms; //!<! the histogram for building calibration information for the doubel harmonic method
  QnCorrectionsProfileCorrelationComponentsHarmonics *fCorrelationsABInputHistograms; //!<! the histogram with calibration information for the correlations method (AB)
  QnCorrectionsProfileCorrelationComponentsHarmonics *fCorrelationsBCInputHistograms; //!<! the histogram with calibration information for the correlations method (BC)
  QnCorrectionsProfileCorrelationComponentsHarmonics *fCorrelationsCAInputHistograms; //!<! the histogram with calibration information for the correlations method (CA)
  QnCorrectionsProfileCorrelationComponentsHarmonics *fCorrelationABCalibrationHistograms; //!<! the histogram for building calibration information for the correlations method (AB)
  QnCorrectionsProfileCorrelationComponentsHarmonics *fCorrelationBCCalibrationHistograms; //!<! the histogram for building calibration information for the correlations method (BC)
  QnCorrectionsProfileCorrelationComponentsHarmonics *fCorrelationCACalibrationHistograms; //!<! the histogram for building calibration information for the correlations method (CA)
  QnCorrectionsHistogramSparse *fQANotValidatedBin;    //!<! the histogram with non validated bin information

  Bool_t fApplyTwist;              ///< apply the twist step
  Bool_t fApplyRescale;            ///< apply the rescale step
  Int_t fMinNoOfEntriesToValidate;              ///< number of entries for bin content validation threshold

/// \cond CLASSIMP
  ClassDef(QnCorrectionsQnVectorTwistAndRescale, 1);
/// \endcond
};

#endif // QNCORRECTIONS_QNVECTORTWISTANDRESCALE_H
