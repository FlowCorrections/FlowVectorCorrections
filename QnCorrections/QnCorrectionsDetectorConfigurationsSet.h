#ifndef QNCORRECTIONS_DETECTORCONFSET_H
#define QNCORRECTIONS_DETECTORCONFSET_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsDetectorConfigurationsSet.h
/// \brief Set of detector configuration class within Q vector correction framework
///

#include "QnCorrectionsDetectorConfigurationBase.h"

/// \class QnCorrectionsDetectorConfigurationsSet
/// \brief Array of detector configurations within Q vector correction framework
///
/// Each detector within the Q vector correction framework is
/// able to be configured in different ways by assigning set of cuts,
/// ordering its data within channels or even channel groups, etc.
/// Each of these ways constitute a detector configuration. This class
/// stores the whole set of detector configurations assigned to a concrete
/// detector.
///
/// \author Jaap Onderwaater <jacobus.onderwaater@cern.ch>, GSI
/// \author Ilya Selyuzhenkov <ilya.selyuzhenkov@gmail.com>, GSI
/// \author Víctor González <victor.gonzalez@cern.ch>, UCM
/// \date Feb 09, 2016

class QnCorrectionsDetectorConfigurationsSet : public TObjArray {
public:
  QnCorrectionsDetectorConfigurationsSet();
  virtual ~QnCorrectionsDetectorConfigurationsSet();

  /// Access the detector configuration at the passed position
  /// \param i position in the list (starting at zero)
  /// \return the detector configuration object a position i
  virtual QnCorrectionsDetectorConfigurationBase *At(Int_t i) const
    { return (QnCorrectionsDetectorConfigurationBase *) TObjArray::At(i);}

/// \cond CLASSIMP
  ClassDef(QnCorrectionsDetectorConfigurationsSet, 1);
/// \endcond
};

#endif // QNCORRECTIONS_DETECTORCONFSET_H
