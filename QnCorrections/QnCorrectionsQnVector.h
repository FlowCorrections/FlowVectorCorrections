#ifndef QNCORRECTIONS_QNVECTORS_H
#define QNCORRECTIONS_QNVECTORS_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsQnVector.h
/// \brief Classes that model Q vectors for different harmonics within the Q vector correction framework

#include <TNamed.h>
#include <TMath.h>

/// The maximum external harmonic number the framework currently support for Q vectors
#define MAXHARMONICNUMBERSUPPORTED 15

/// \typedef QnVectorNormalizationMethod
/// \brief The class of the id of the supported Q vector normalization methods
///
/// Actually it is not a class because the C++ level of implementation.
/// But full protection will be reached when were possible declaring it
/// as a class.
///
/// M is the sum of weights.
typedef enum QVECNORM {
  QVNORM_noCalibration, ///< \f$ \mbox{Q'} = \mbox{Q}\f$
  QVNORM_QoverSqrtM,    ///< \f$ \mbox{Q'} = \frac{\mbox{Q}}{\sqrt{\mbox{M}}} \f$
  QVNORM_QoverM,        ///< \f$ \mbox{Q'} = \frac{\mbox{Q}}{\mbox{M}} \f$
  QVNORM_QoverQlength   ///< \f$ \mbox{Q'} = \frac{\mbox{Q}}{|\mbox{Q}|} \f$
} QnVectorNormalizationMethod;

/// \class QnCorrectionsQnVector
/// \brief Class that models and encapsulates a Q vector set
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 27, 2016
class QnCorrectionsQnVector : public TNamed {

public:
  QnCorrectionsQnVector();
  QnCorrectionsQnVector(const char *name, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
  QnCorrectionsQnVector(const QnCorrectionsQnVector &Q);
  virtual ~QnCorrectionsQnVector();

  void ActivateHarmonic(Int_t harmonic);
  Int_t GetNoOfHarmonics() const;
  void GetHarmonicsMap(Int_t *harmonicMap) const;
  Int_t GetFirstHarmonic() const;
  Int_t GetNextHarmonic(Int_t harmonic) const;

  /// Sets the X component for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \param qx the X component for the Q vector
  virtual void SetQx(Int_t harmonic, Float_t qx) { fQnX[harmonic] = qx; }
  /// Sets the Y component for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \param qy the Y component for the Q vector
  virtual void SetQy(Int_t harmonic, Float_t qy) { fQnY[harmonic] = qy; }
  /// Set the good quality flag
  /// \param good kTRUE  if the quality is good
  virtual void SetGood(Bool_t good) { fGoodQuality = good; }


  void Set(QnCorrectionsQnVector* Qn, Bool_t changename);

  void Normalize();
  /// Provides the length of the Q vector for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \return the square root of components square sum
  Float_t Length(Int_t harmonic) const { return  TMath::Sqrt(Qx(harmonic)*Qx(harmonic)+Qy(harmonic)*Qy(harmonic));}
  /// Provides the X component normalized to one of the Q vector for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \return X component of the normalized Q vector
  Float_t QxNorm(Int_t harmonic) const { return  Qx(harmonic)/Length(harmonic);}
  /// Provides the Y component normalized to one of the Q vector for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \return Y component of the normalized Q vector
  Float_t QyNorm(Int_t harmonic) const { return  Qy(harmonic)/Length(harmonic);}

  virtual void Reset();

  /// Gets the Q vector X component for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \return the Q vector X component
  Float_t Qx(Int_t harmonic) const { return fQnX[harmonic]; }
  /// Gets the Q vector Y component for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \return the Q vector Y component
  Float_t Qy(Int_t harmonic) const { return fQnY[harmonic]; }
  /// Get the Qn vector quality flag
  /// \return Qn vector quality flag
  Bool_t IsGoodQuality() const { return fGoodQuality; }

  Double_t EventPlane(Int_t harmonic) const;

  virtual void Print(Option_t *) const;

private:
  /// Assignment operator
  ///
  /// Default implementation to protect against its accidental use.
  /// It will give a compilation error. Don't make it different from
  /// private!
  /// \param Qn the Q vector to assign
  QnCorrectionsQnVector& operator= (const QnCorrectionsQnVector &Qn);

protected:

  static const Float_t  fMinimumSignificantValue;     ///< the minimum value that will be considered as meaningful for processing
  static const UInt_t   harmonicNumberMask[];         ///< Mask for each external harmonic number

  Float_t fQnX[MAXHARMONICNUMBERSUPPORTED+1];   ///< the Q vector X component for each harmonic
  Float_t fQnY[MAXHARMONICNUMBERSUPPORTED+1];   ///< the Q vector Y component for each harmonic
  Int_t   fHighestHarmonic;                    ///< the highest harmonic number handled
  UInt_t  fHarmonicMask;                       ///< the mask for the supported harmonics
  Bool_t  fGoodQuality;                        ///< Qn vector good quality flag

/// \cond CLASSIMP
  ClassDef(QnCorrectionsQnVector, 1);
/// \endcond
};

/// \class QnCorrectionsQnVectorBuild
/// \brief Class that models and encapsulates a Q vector set while building it
///
/// When the Q vector is being built it needs extra support.
/// This class provides such extra support.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 02, 2016
class QnCorrectionsQnVectorBuild : public QnCorrectionsQnVector {

public:
  QnCorrectionsQnVectorBuild();
  QnCorrectionsQnVectorBuild(const char *name, Int_t nNoOfHarmonics, Int_t *harmonicMap = NULL);
  QnCorrectionsQnVectorBuild(const QnCorrectionsQnVector &Qn);
  QnCorrectionsQnVectorBuild(const QnCorrectionsQnVectorBuild &Qn);
  virtual ~QnCorrectionsQnVectorBuild();

  virtual void SetQx(Int_t harmonic, Float_t qx);
  virtual void SetQy(Int_t harmonic, Float_t qy);

  void Set(QnCorrectionsQnVectorBuild* Qn);

  void Add(QnCorrectionsQnVectorBuild* qvec);
  void Add(Double_t phi, Double_t weight = 1.0);

  /// Check the quality of the constructed Qn vector
  /// Current criteria is number of contributors should be at least one.
  /// If so happen, sets the good quality flag.
  void CheckQuality() { fGoodQuality = ((0 < fN) ? kTRUE : kFALSE); }
  void Normalize(QnVectorNormalizationMethod method);

  void NormalizeQoverM();
  void NormalizeQoverSquareRootOfM();

  virtual void Reset();

  /// Gets the sum of weights.
  /// \return sum of weights
  Float_t GetSumOfWeights() const { return fSumW; }
  /// Gets the number of elements
  /// \return number of elements
  Int_t GetN() const { return fN; }

  virtual void Print(Option_t *) const;

private:
  /// Assignment operator
  ///
  /// Default implementation to protect against its accidental use.
  /// It will give a compilation error. Don't make it different from
  /// private!
  /// \param Qn the Q vector to assign
  QnCorrectionsQnVectorBuild& operator= (const QnCorrectionsQnVectorBuild &Qn);

protected:

  Float_t fSumW;   ///< the sum of weights
  Int_t fN;        ///< number of elements

/// \cond CLASSIMP
  ClassDef(QnCorrectionsQnVectorBuild, 1);
/// \endcond
};

/// Get the number of the first harmonic used
/// \return the number of the first harmonic handled by the Q vector, -1 if none
inline Int_t QnCorrectionsQnVector::GetFirstHarmonic() const {
  for(Int_t h = 1; h < fHighestHarmonic + 1; h++){
    if ((fHarmonicMask & harmonicNumberMask[h]) == harmonicNumberMask[h]) {
      return h;
    }
  }
  return -1;
}

/// Get the next harmonic to the one passed as parameter
/// \param harmonic number to find the next one
/// \return the number of the next to the passed harmonic, -1 if none
inline Int_t QnCorrectionsQnVector::GetNextHarmonic(Int_t harmonic) const {
  for(Int_t h = harmonic+1; h < fHighestHarmonic + 1; h++){
    if ((fHarmonicMask & harmonicNumberMask[h]) == harmonicNumberMask[h]) {
      return h;
    }
  }
  return -1;
}


/// Adds a contribution to the build Q vector
/// \param phi azimuthal angle contribution
/// \param weight the weight of the contribution
inline void QnCorrectionsQnVectorBuild::Add(Double_t phi, Double_t weight) {

  if (weight < fMinimumSignificantValue) return;
  for(Int_t h = 1; h < fHighestHarmonic + 1; h++){
    if ((fHarmonicMask & harmonicNumberMask[h]) == harmonicNumberMask[h]) {
      fQnX[h] += (weight * TMath::Cos(h*phi));
      fQnY[h] += (weight * TMath::Sin(h*phi));
    }
  }
  fSumW += weight;
  fN += 1;
}

/// Calibrates the Q vector according to the method passed
/// \param method the method of calibration
inline void QnCorrectionsQnVectorBuild::Normalize(QnVectorNormalizationMethod method) {
  switch (method) {
  case QVNORM_noCalibration:
    break;
  case QVNORM_QoverSqrtM:
    NormalizeQoverSquareRootOfM();
    break;
  case QVNORM_QoverM:
    NormalizeQoverM();
    break;
  case QVNORM_QoverQlength:
    QnCorrectionsQnVector::Normalize();
    break;
  }
}


#endif /* QNCORRECTIONS_QNVECTORS_H */
