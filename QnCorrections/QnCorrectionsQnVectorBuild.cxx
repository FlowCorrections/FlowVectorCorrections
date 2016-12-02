/**************************************************************************************************
 *                                                                                                *
 * Package:       FlowVectorCorrections                                                           *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch                              *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com                             *
 *                Víctor González, UCM, victor.gonzalez@cern.ch                                   *
 *                Contributors are mentioned in the code where appropriate.                       *
 * Development:   2012-2016                                                                       *
 *                                                                                                *
 * This file is part of FlowVectorCorrections, a software package that corrects Q-vector          *
 * measurements for effects of nonuniform detector acceptance. The corrections in this package    *
 * are based on publication:                                                                      *
 *                                                                                                *
 *  [1] "Effects of non-uniform acceptance in anisotropic flow measurements"                      *
 *  Ilya Selyuzhenkov and Sergei Voloshin                                                         *
 *  Phys. Rev. C 77, 034904 (2008)                                                                *
 *                                                                                                *
 * The procedure proposed in [1] is extended with the following steps:                            *
 * (*) alignment correction between subevents                                                     *
 * (*) possibility to extract the twist and rescaling corrections                                 *
 *      for the case of three detector subevents                                                  *
 *      (currently limited to the case of two “hit-only” and one “tracking” detectors)            *
 * (*) (optional) channel equalization                                                            *
 * (*) flow vector width equalization                                                             *
 *                                                                                                *
 * FlowVectorCorrections is distributed under the terms of the GNU General Public License (GPL)   *
 * (https://en.wikipedia.org/wiki/GNU_General_Public_License)                                     *
 * either version 3 of the License, or (at your option) any later version.                        *
 *                                                                                                *
 **************************************************************************************************/

/// \file QnCorrectionsQnVectorBuild.cxx
/// \brief Implementation of Q vector while building class

#include <Riostream.h>

#include "QnCorrectionsQnVectorBuild.h"
#include "QnCorrectionsLog.h"

using std::cout;
using std::endl;

/// \cond CLASSIMP
ClassImp(QnCorrectionsQnVectorBuild);
/// \endcond

/// Default constructor
QnCorrectionsQnVectorBuild::QnCorrectionsQnVectorBuild() : QnCorrectionsQnVector() {

}

/// Normal constructor
///
/// Relays on its parent for almost everything
///
/// \param name the name of the Qn vector. Identifies its origin
/// \param nNoOfHarmonics the desired number of harmonics
/// \param harmonicMap ordered array with the external number of the harmonics
QnCorrectionsQnVectorBuild::QnCorrectionsQnVectorBuild(const char *name, Int_t nNoOfHarmonics, Int_t *harmonicMap) :
    QnCorrectionsQnVector(name, nNoOfHarmonics, harmonicMap) {

}

/// Copy constructor from a Q vector
/// \param Qn the Q vector build object to copy after construction
QnCorrectionsQnVectorBuild::QnCorrectionsQnVectorBuild(const QnCorrectionsQnVector &Qn) :
    QnCorrectionsQnVector(Qn) {

}

/// Copy constructor
/// \param Qn the Q vector build object to copy after construction
QnCorrectionsQnVectorBuild::QnCorrectionsQnVectorBuild(const QnCorrectionsQnVectorBuild &Qn) :
    QnCorrectionsQnVector(Qn) {

}

/// Default destructor
QnCorrectionsQnVectorBuild::~QnCorrectionsQnVectorBuild() {

}

/// Sets the X component for the considered harmonic
///
/// It should not be used. Runtime error indication.
void QnCorrectionsQnVectorBuild::SetQx(Int_t, Float_t) {

  QnCorrectionsFatal("You are using a forbidden function for a build Q vector");
}

/// Sets the Y component for the considered harmonic
///
/// It should not be used. Runtime error indication.
void QnCorrectionsQnVectorBuild::SetQy(Int_t, Float_t) {

  QnCorrectionsFatal("You are using a forbidden function for a build Q vector");
}

/// Copy member function
///
/// The passed Q vector is copied within the current object
/// \param Qn pointer to the Q vector to be copied
void QnCorrectionsQnVectorBuild::Set(QnCorrectionsQnVectorBuild* Qn) {

  /* the name is not copied from building Qn vectors */
  QnCorrectionsQnVector::Set(Qn,kFALSE);
}

/// Adds a build Q vector
///
/// The possibility of a different set of harmonics for both
/// build Q vectors is considered. A run time error is
/// raised if they do not match.
/// \param Qn the build Q vector to add
void QnCorrectionsQnVectorBuild::Add(QnCorrectionsQnVectorBuild* Qn) {

  if ((fHighestHarmonic != Qn->fHighestHarmonic) ||
      (fHarmonicMask != Qn->fHarmonicMask) ||
      (fHarmonicMultiplier != Qn->fHarmonicMultiplier)) {
    QnCorrectionsFatal("You requested to add to a Q vector the values from other Q " \
        "vector but the harmonic structures do not match");
    return;
  }

  for(Int_t h = 1; h < fHighestHarmonic + 1; h++){
    if ((fHarmonicMask & harmonicNumberMask[h]) == harmonicNumberMask[h]) {
      fQnX[h] += Qn->Qx(h);
      fQnY[h] += Qn->Qy(h);
    }
  }
  fSumW += Qn->GetSumOfWeights();
  fN += Qn->GetN();
}

/// Normalizes the build Q vector for the whole harmonics set
///
/// Normalizes the build Q vector as \f$ Qn = \frac{Qn}{M} \f$.
/// A check for significant value is made. Not passing it
/// does set the Q vector quality as bad
void QnCorrectionsQnVectorBuild::NormalizeQoverM() {

  if(fSumW < fMinimumSignificantValue) {
    SetGood(kFALSE);
  }
  else {
    for(Int_t h = 1; h < fHighestHarmonic + 1; h++){
      if ((fHarmonicMask & harmonicNumberMask[h]) == harmonicNumberMask[h]) {
        fQnX[h] = (fQnX[h] / fSumW);
        fQnY[h] = (fQnY[h] / fSumW);
      }
    }
  }
}

/// Normalizes the build Q vector for the whole harmonics set
///
/// Normalizes the build Q vector as \f$ Qn = \frac{Qn}{\sqrt{M}} \f$.
/// A check for significant value is made. Not passing it
/// does set the Q vector quality as bad
void QnCorrectionsQnVectorBuild::NormalizeQoverSquareRootOfM() {

  if(fSumW < fMinimumSignificantValue) {
    SetGood(kFALSE);
  }
  else {
    for(Int_t h = 1; h < fHighestHarmonic + 1; h++){
      if ((fHarmonicMask & harmonicNumberMask[h]) == harmonicNumberMask[h]) {
        fQnX[h] = fQnX[h] / TMath::Sqrt(fSumW);
        fQnY[h] = fQnY[h] / TMath::Sqrt(fSumW);
      }
    }
  }
}

/// Resets the Q vector values without touching the structure
void QnCorrectionsQnVectorBuild::Reset() {

  QnCorrectionsQnVector::Reset();
}

/// Print the Qn vector in a readable shape
///
void QnCorrectionsQnVectorBuild::Print(Option_t *) const {
  cout <<"OBJ: building Qn vector" << "\t" << "N: " << fN << "\t" << "Sum w: " << fSumW << "\t"
      << "quality: " << ((fGoodQuality) ? "good" : "bad") << endl;
  Int_t harmonic = GetFirstHarmonic();
  while (harmonic != -1) {
    cout << "\t" << "\t" << "harmonic " << harmonic*fHarmonicMultiplier << "\t" << "QX: " << Qx(harmonic) << "\t" << "QY: " << Qy(harmonic) << endl;
    harmonic = GetNextHarmonic(harmonic);
  }
}


