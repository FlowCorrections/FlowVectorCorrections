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
/// \file QnCorrectionsCuts.cxx
/// \brief Implementation of the classes that model the cuts support

#include "QnCorrectionsCuts.h"
#include "QnCorrectionsLog.h"

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutBase);
/// \endcond

/// The highest bit number the framework support
const Int_t QnCorrectionsCutBase::nHighestBitNumberSupported = 31;

/// Default constructor
QnCorrectionsCutBase::QnCorrectionsCutBase() : TObject() {
  fVarId = -1;
}

/// Copy constructor
/// \param cut the cut object to clone
QnCorrectionsCutBase::QnCorrectionsCutBase(const QnCorrectionsCutBase &cut) :
    TObject(cut) {
  fVarId = cut.fVarId;
}

/// Normal constructor. Stores the affected variable Id
/// \param varId External Id for the affected variable
QnCorrectionsCutBase::QnCorrectionsCutBase(Int_t varId) :
    TObject() {
  fVarId = varId;
}

/// Default destructor. Does nothing
QnCorrectionsCutBase::~QnCorrectionsCutBase() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsAboveCut);
/// \endcond

/// Default constructor
QnCorrectionsAboveCut::QnCorrectionsAboveCut() : QnCorrectionsCutBase() {
  fThreshold = 1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsAboveCut::QnCorrectionsAboveCut(const QnCorrectionsAboveCut &cut) :
    QnCorrectionsCutBase(cut) {
  fThreshold = cut.fThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param threshold the value the variable content should be above
QnCorrectionsAboveCut::QnCorrectionsAboveCut(Int_t varId, Float_t threshold) :
    QnCorrectionsCutBase(varId) {
  fThreshold = threshold;
}

/// Default destructor. Does nothing
QnCorrectionsAboveCut::~QnCorrectionsAboveCut() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsBelowCut);
/// \endcond

/// Default constructor
QnCorrectionsBelowCut::QnCorrectionsBelowCut() : QnCorrectionsCutBase() {
  fThreshold = -1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsBelowCut::QnCorrectionsBelowCut(const QnCorrectionsBelowCut &cut) :
    QnCorrectionsCutBase(cut) {
  fThreshold = cut.fThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param threshold the value the variable content should be below
QnCorrectionsBelowCut::QnCorrectionsBelowCut(Int_t varId, Float_t threshold) :
    QnCorrectionsCutBase(varId) {
  fThreshold = threshold;
}

/// Default destructor. Does nothing
QnCorrectionsBelowCut::~QnCorrectionsBelowCut() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsWithinCut);
/// \endcond

/// Default constructor
QnCorrectionsWithinCut::QnCorrectionsWithinCut() :  QnCorrectionsCutBase() {
  fMinThreshold = 1e10;
  fMaxThreshold = -1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsWithinCut::QnCorrectionsWithinCut(const QnCorrectionsWithinCut &cut) :
    QnCorrectionsCutBase(cut) {
  fMinThreshold = cut.fMinThreshold;
  fMaxThreshold = cut.fMaxThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param min the value the variable content should be above
/// \param max the value the variable content should be below
QnCorrectionsWithinCut::QnCorrectionsWithinCut(Int_t varId, Float_t min, Float_t max) :
    QnCorrectionsCutBase(varId) {
  fMinThreshold = min;
  fMaxThreshold = max;
}

/// Default destructor. Does nothing
QnCorrectionsWithinCut::~QnCorrectionsWithinCut() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsOutsideCut);
/// \endcond

/// Default constructor
QnCorrectionsOutsideCut::QnCorrectionsOutsideCut() :  QnCorrectionsCutBase() {
  fMinThreshold = -1e10;
  fMaxThreshold = 1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsOutsideCut::QnCorrectionsOutsideCut(const QnCorrectionsOutsideCut &cut) :
    QnCorrectionsCutBase(cut) {
  fMinThreshold = cut.fMinThreshold;
  fMaxThreshold = cut.fMaxThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param min the value the variable content should be below
/// \param max the value the variable content should be above
QnCorrectionsOutsideCut::QnCorrectionsOutsideCut(Int_t varId, Float_t min, Float_t max) :
    QnCorrectionsCutBase(varId) {
  fMinThreshold = min;
  fMaxThreshold = max;
}

/// Default destructor. Does nothing
QnCorrectionsOutsideCut::~QnCorrectionsOutsideCut() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsValueCut);
/// \endcond

/// Default constructor
QnCorrectionsValueCut::QnCorrectionsValueCut() : QnCorrectionsCutBase() {
  fValue = 0.0;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsValueCut::QnCorrectionsValueCut(const QnCorrectionsValueCut &cut) :
    QnCorrectionsCutBase(cut) {
  fValue = cut.fValue;
}


/// Normal constructor
/// \param varId external Id for the affected variable
/// \param value the value the variable content should be equal to
QnCorrectionsValueCut::QnCorrectionsValueCut(Int_t varId, Float_t value) :
    QnCorrectionsCutBase(varId) {
  fValue = value;
}

/// Default destructor. Does nothing
QnCorrectionsValueCut::~QnCorrectionsValueCut() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsBitSetCut);
/// \endcond

/// Default constructor
QnCorrectionsBitSetCut::QnCorrectionsBitSetCut() : QnCorrectionsCutBase() {
  fBitMask = 0x00000000;
  fExpectedResult = 0xFFFFFFFF;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsBitSetCut::QnCorrectionsBitSetCut(const QnCorrectionsBitSetCut &cut) :
    QnCorrectionsCutBase(cut) {
  fBitMask = cut.fBitMask;
  fExpectedResult = cut.fExpectedResult;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param bitNo the bit on the variable content to test (from 0 to 31)
/// \param set (kFALSE)kTRUE for cut on the bit (un)set
QnCorrectionsBitSetCut::QnCorrectionsBitSetCut(Int_t varId, Int_t bitNo, Bool_t set) :
    QnCorrectionsCutBase(varId) {
  if (nHighestBitNumberSupported < bitNo) {
    QnCorrectionsFatal(Form("You requested a cut on bit %d but the highest bit number supported by the framework is currently %d",
        bitNo, nHighestBitNumberSupported));
  }
  fBitMask = 0x00000001 < bitNo;
  fExpectedResult = (set ? fBitMask : 0x00000000);
}

/// Default destructor. Does nothing
QnCorrectionsBitSetCut::~QnCorrectionsBitSetCut() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutsSet);
/// \endcond



