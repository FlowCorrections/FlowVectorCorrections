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

/// \file QnCorrectionsQnVectors.h
/// \brief Classes that model Q vectors for different harmonics within the Q vector correction framework

#include <TObject.h>

/// \class QnCorrectionsQnVector
/// \brief Class that models and encapsulates a Q vector set
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Jan 27, 2016
class QnCorrectionsQnVector : public TObject {

public:
  QnCorrectionsQnVector();
  QnCorrectionsQnVector(Int_t nNoOfHarmonics, UInt_t *harmonicMap = NULL);
  QnCorrectionsQnVector(const QnCorrectionsQnVector &Q);
  virtual ~QnCorrectionsQnVector();

  void ActivateHarmonic(Int_t harmonic);

  /// Sets the X component for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \param qx the X component for the Q vector
  virtual void SetQx(Int_t harmonic, Float_t qx) { fQnX[harmonic] = qx; }
  /// Sets the Y component for the considered harmonic
  /// \param harmonic the intended harmonic
  /// \param qy the Y component for the Q vector
  virtual void SetQy(Int_t harmonic, Float_t qy) { fQnY[harmonic] = qy; }

  virtual void Set(QnCorrectionsQnVector* Qn);

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

  Double_t EventPlane(Int_t harmonic) const;

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
  static const Int_t    nMaxHarmonicNumberSupported;  ///< The maximum external harmonic number the framework supports
  static const UInt_t   harmonicNumberMask[];         ///< Mask for each external harmonic number

  Float_t fQnX[nMaxHarmonicNumberSupported];   ///< the Q vector X component for each harmonic
  Float_t fQnY[nMaxHarmonicNumberSupported];   ///< the Q vector Y component for each harmonic
  Int_t   fHighestHarmonic;                    ///< the highest harmonic number handled
  UInt_t  fHarmonicMask;                       ///< the mask for the supported harmonics

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
  QnCorrectionsQnVectorBuild(Int_t nNoOfHarmonics, UInt_t *harmonicMap = NULL);
  QnCorrectionsQnVectorBuild(const QnCorrectionsQnVectorBuild &Qn);
  virtual ~QnCorrectionsQnVectorBuild();

  virtual void SetQx(Int_t harmonic, Float_t qx);
  virtual void SetQy(Int_t harmonic, Float_t qy);

  virtual void Set(QnCorrectionsQnVectorBuild* Qn);

  void Add(QnCorrectionsQnVectorBuild* qvec);
  void Add(Double_t phi, Double_t weight = 1.0);


  void NormalizeQoverM();
  void NormalizeQoverSquareRootOfM();

  virtual void Reset();

  /// Gets the sum of weights.
  /// \return sum of weights
  Float_t GetSumOfWeights() const { return fSumW; }
  /// Gets the number of elements
  /// \return number of elements
  Int_t GetN() const { return fN; }

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

/// Adds a contribution to the build Q vector
/// \param phi azimuthal angle contribution
/// \param weight the weight of the contribution
inline void QnCorrectionsQnVectorBuild::Add(Double_t phi, Double_t weight = 1.0) {

  for(Int_t h = 1; h < fHighestHarmonic + 1; h++){
    if (fHarmonicMask & harmonicNumberMask[h] == harmonicNumberMask[h]) {
      fQnX[h] += weight * TMath::Cos(h*phi);
      fQnY[h] += weight * TMath::Sin(h*phi);
    }
  }
  fSumW += weight;
  fN += 1;
}

#endif /* QNCORRECTIONS_QNVECTORS_H */
