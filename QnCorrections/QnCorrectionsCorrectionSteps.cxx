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

/// \file QnCorrectionsCorrectionSteps.cxx
/// \brief Correction steps base classes implementation

#include "QnCorrectionsCorrectionSteps.h"

/// \cond CLASSIMP
ClassImp(QnCorrectionsCorrectionStepBase);
/// \endcond

/// Default constructor
QnCorrectionsCorrectionStepBase::QnCorrectionsCorrectionStepBase() : TNamed() {

  fState = QCORRSTEP_calibration;
  fDetectorConfiguration = NULL;
  fKey = "";
}

/// Normal constructor
/// \param name the name of the correction step
/// \param key the associated ordering key
QnCorrectionsCorrectionStepBase::QnCorrectionsCorrectionStepBase(const char *name, const char *key) :
    TNamed(name,name) {

  fState = QCORRSTEP_calibration;
  fDetectorConfiguration = NULL;
  fKey = key;
}

/// Default destructor
QnCorrectionsCorrectionStepBase::~QnCorrectionsCorrectionStepBase() {

}

/// Checks if should be applied before the one passed as parameter
/// \param correction correction to check whether to be applied after
/// \return kTRUE if to apply before the one passed as argument
Bool_t QnCorrectionsCorrectionStepBase::Before(const QnCorrectionsCorrectionStepBase *correction) {

  if (fKey.CompareTo(correction->GetKey()) < 0) return kTRUE;
  return kFALSE;
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCorrectionOnInputData);
/// \endcond

/// Default constructor
QnCorrectionsCorrectionOnInputData::QnCorrectionsCorrectionOnInputData() :
    QnCorrectionsCorrectionStepBase() {

}

/// Normal constructor
/// \param name of the correction step
/// \param key the associated ordering key
QnCorrectionsCorrectionOnInputData::QnCorrectionsCorrectionOnInputData(const char *name, const char *key) :
    QnCorrectionsCorrectionStepBase(name, key) {

}

/// Default destructor
QnCorrectionsCorrectionOnInputData::~QnCorrectionsCorrectionOnInputData() {

}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCorrectionOnQvector);
/// \endcond


/// Default constructor
QnCorrectionsCorrectionOnQvector::QnCorrectionsCorrectionOnQvector() :
    QnCorrectionsCorrectionStepBase() {

}

/// Normal constructor
/// \param name of the correction step
/// \param key the associated ordering key
QnCorrectionsCorrectionOnQvector::QnCorrectionsCorrectionOnQvector(const char *name, const char *key) :
    QnCorrectionsCorrectionStepBase(name, key) {

}

/// Default destructor
QnCorrectionsCorrectionOnQvector::~QnCorrectionsCorrectionOnQvector() {

}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCorrectionsSetOnInputData);
/// \endcond

/// Default constructor
QnCorrectionsCorrectionsSetOnInputData::QnCorrectionsCorrectionsSetOnInputData() : TList() {

}

/// Default destructor
QnCorrectionsCorrectionsSetOnInputData::~QnCorrectionsCorrectionsSetOnInputData() {

}

/// Adds a new correction to the set.
///
/// The correction is incorporated in its proper place according to
/// its key
void QnCorrectionsCorrectionsSetOnInputData::AddCorrection(QnCorrectionsCorrectionOnInputData *correction) {
  if (IsEmpty()) {
    AddFirst(correction);
  }
  else if (correction->Before((QnCorrectionsCorrectionOnInputData *) First())) {
    AddFirst(correction);
  }
  else if (((QnCorrectionsCorrectionOnInputData *) Last())->Before(correction)) {
    AddLast(correction);
  }
  else {
    for (Int_t ix = 0; ix < GetEntries(); ix++) {
      if (!correction->Before(At(ix))) {
        AddAt(correction, ix-1);
      }
    }
  }
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCorrectionsSetOnQvector);
/// \endcond

/// Default constructor
QnCorrectionsCorrectionsSetOnQvector::QnCorrectionsCorrectionsSetOnQvector() : TList() {

}

/// Default destructor
QnCorrectionsCorrectionsSetOnQvector::~QnCorrectionsCorrectionsSetOnQvector() {

}

/// Adds a new correction to the set.
///
/// The correction is incorporated in its proper place according to
/// its key
void QnCorrectionsCorrectionsSetOnQvector::AddCorrection(QnCorrectionsCorrectionOnQvector *correction) {
  if (IsEmpty()) {
    AddFirst(correction);
  }
  else if (correction->Before((QnCorrectionsCorrectionOnQvector *) First())) {
    AddFirst(correction);
  }
  else if (((QnCorrectionsCorrectionOnQvector *) Last())->Before(correction)) {
    AddLast(correction);
  }
  else {
    for (Int_t ix = 0; ix < GetEntries(); ix++) {
      if (!correction->Before(At(ix))) {
        AddAt(correction, ix-1);
      }
    }
  }
}

