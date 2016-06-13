#ifndef QNCORRECTIONS_DATAVECTORS_H
#define QNCORRECTIONS_DATAVECTORS_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsDataVector.h
/// \brief Class that model data vectors from detectors within the Q vector correction framework
///
/// As it is today, a data vector is just an azimuthal angle. As
/// it is intended to be incorporated into an array of clones the
/// constructor is really simple and instead the setters are used
/// to initialize its members
///

#include <TObject.h>
/// \class QnCorrectionsDataVector
/// \brief Class that models and encapsulates a data vector
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 01, 2016
class QnCorrectionsDataVector : public TObject {
public:
  QnCorrectionsDataVector();
  QnCorrectionsDataVector(Int_t id, Float_t phi, Float_t weight);
  virtual ~QnCorrectionsDataVector();

  /// Sets the data vector azimuthal angle
  /// \param phi the azimuthal angle
  virtual void SetPhi(Float_t phi) { fPhi = phi; }
  /// Sets the channel id associated with the data vector
  /// \param id channel id
  void SetId(Int_t id) { fId = id; }

  /// Gets the channel id associated with the data vector
  /// \return the channel id
  Int_t GetId() { return fId; }
  /// Sets the raw weight
  /// \param weight raw weight from the detector channel
  void SetWeight(Float_t weight) { fWeight = weight; }
  /// Gets the azimuthal angle for the data vector
  /// \return phi
  virtual Float_t Phi() { return fPhi; }
  /// Gets the weight for the data vector
  /// \return defaults to 1.0
  virtual Float_t Weight() { return fWeight; }
  /// Gets the equalized weight for the data vector
  /// \return defaults to weights
  virtual Float_t EqualizedWeight() { return fWeight; }

protected:
  Float_t fPhi;                                   //!<! the azimuthal angle of the data vector
  Int_t   fId;                    //!<! the id associated with the data vector
  Float_t fWeight;                //!<! raw weight assigned to the data vector

  static const Float_t fMinimumSignificantValue;  ///< the minimum value that will be considered as meaningful for processing

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDataVector, 3);
/// \endcond
};

#endif /* QNCORRECTIONS_DATAVECTORS_H */
