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
ClassImp(QnCorrectionsCutAbove);
/// \endcond

/// Default constructor
QnCorrectionsCutAbove::QnCorrectionsCutAbove() : QnCorrectionsCutBase() {
  fThreshold = 1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsCutAbove::QnCorrectionsCutAbove(const QnCorrectionsCutAbove &cut) :
    QnCorrectionsCutBase(cut) {
  fThreshold = cut.fThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param threshold the value the variable content should be above
QnCorrectionsCutAbove::QnCorrectionsCutAbove(Int_t varId, Float_t threshold) :
    QnCorrectionsCutBase(varId) {
  fThreshold = threshold;
}

/// Default destructor. Does nothing
QnCorrectionsCutAbove::~QnCorrectionsCutAbove() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutBelow);
/// \endcond

/// Default constructor
QnCorrectionsCutBelow::QnCorrectionsCutBelow() : QnCorrectionsCutBase() {
  fThreshold = -1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsCutBelow::QnCorrectionsCutBelow(const QnCorrectionsCutBelow &cut) :
    QnCorrectionsCutBase(cut) {
  fThreshold = cut.fThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param threshold the value the variable content should be below
QnCorrectionsCutBelow::QnCorrectionsCutBelow(Int_t varId, Float_t threshold) :
    QnCorrectionsCutBase(varId) {
  fThreshold = threshold;
}

/// Default destructor. Does nothing
QnCorrectionsCutBelow::~QnCorrectionsCutBelow() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutWithin);
/// \endcond

/// Default constructor
QnCorrectionsCutWithin::QnCorrectionsCutWithin() :  QnCorrectionsCutBase() {
  fMinThreshold = 1e10;
  fMaxThreshold = -1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsCutWithin::QnCorrectionsCutWithin(const QnCorrectionsCutWithin &cut) :
    QnCorrectionsCutBase(cut) {
  fMinThreshold = cut.fMinThreshold;
  fMaxThreshold = cut.fMaxThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param min the value the variable content should be above
/// \param max the value the variable content should be below
QnCorrectionsCutWithin::QnCorrectionsCutWithin(Int_t varId, Float_t min, Float_t max) :
    QnCorrectionsCutBase(varId) {
  fMinThreshold = min;
  fMaxThreshold = max;
}

/// Default destructor. Does nothing
QnCorrectionsCutWithin::~QnCorrectionsCutWithin() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutOutside);
/// \endcond

/// Default constructor
QnCorrectionsCutOutside::QnCorrectionsCutOutside() :  QnCorrectionsCutBase() {
  fMinThreshold = -1e10;
  fMaxThreshold = 1e10;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsCutOutside::QnCorrectionsCutOutside(const QnCorrectionsCutOutside &cut) :
    QnCorrectionsCutBase(cut) {
  fMinThreshold = cut.fMinThreshold;
  fMaxThreshold = cut.fMaxThreshold;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param min the value the variable content should be below
/// \param max the value the variable content should be above
QnCorrectionsCutOutside::QnCorrectionsCutOutside(Int_t varId, Float_t min, Float_t max) :
    QnCorrectionsCutBase(varId) {
  fMinThreshold = min;
  fMaxThreshold = max;
}

/// Default destructor. Does nothing
QnCorrectionsCutOutside::~QnCorrectionsCutOutside() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutValue);
/// \endcond

/// Default constructor
QnCorrectionsCutValue::QnCorrectionsCutValue() : QnCorrectionsCutBase() {
  fValue = 0.0;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsCutValue::QnCorrectionsCutValue(const QnCorrectionsCutValue &cut) :
    QnCorrectionsCutBase(cut) {
  fValue = cut.fValue;
}


/// Normal constructor
/// \param varId external Id for the affected variable
/// \param value the value the variable content should be equal to
QnCorrectionsCutValue::QnCorrectionsCutValue(Int_t varId, Float_t value) :
    QnCorrectionsCutBase(varId) {
  fValue = value;
}

/// Default destructor. Does nothing
QnCorrectionsCutValue::~QnCorrectionsCutValue() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutBitSet);
/// \endcond

/// Default constructor
QnCorrectionsCutBitSet::QnCorrectionsCutBitSet() : QnCorrectionsCutBase() {
  fBitMask = 0x00000000;
  fExpectedResult = 0xFFFFFFFF;
}

/// Copy constructor
/// \param cut the cut object to be cloned
QnCorrectionsCutBitSet::QnCorrectionsCutBitSet(const QnCorrectionsCutBitSet &cut) :
    QnCorrectionsCutBase(cut) {
  fBitMask = cut.fBitMask;
  fExpectedResult = cut.fExpectedResult;
}

/// Normal constructor
/// \param varId external Id for the affected variable
/// \param bitNo the bit on the variable content to test (from 0 to 31)
/// \param set (kFALSE)kTRUE for cut on the bit (un)set
QnCorrectionsCutBitSet::QnCorrectionsCutBitSet(Int_t varId, Int_t bitNo, Bool_t set) :
    QnCorrectionsCutBase(varId) {
  if (nHighestBitNumberSupported < bitNo) {
    QnCorrectionsFatal(Form("You requested a cut on bit %d but the highest bit number supported by the framework is currently %d",
        bitNo, nHighestBitNumberSupported));
  }
  fBitMask = 0x00000001 << bitNo;
  fExpectedResult = (set ? fBitMask : 0x00000000);
}

/// Default destructor. Does nothing
QnCorrectionsCutBitSet::~QnCorrectionsCutBitSet() {
}

/// \cond CLASSIMP
ClassImp(QnCorrectionsCutsSet);
/// \endcond



