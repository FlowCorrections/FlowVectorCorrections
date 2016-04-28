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
/// \file QnCorrectionsLog.cxx
/// \brief Implementation of the logging function support

#include <TString.h>
#include "QnCorrectionsLog.h"

UInt_t nLoggingLevel = kError; ///< the current logging level

/// Sets the logging level
/// Supported levels match ROOT logging levels currently
/// Info, Warning, Error and Fatal. Fatal produces a runtime
/// error that stops execution while the other provide a
/// meaningful message output.
///
/// Error and Fatal levels cannot be masked
///
/// \param level the desired logging level
void QnCorrectionsSetTracingLevel(UInt_t level) {
  if (level < UInt_t(kError))
    /* don't mask errors */
    nLoggingLevel = level;
}

/// Support function for printing logging messages. The message
/// will be printed if its level is higher than the current logging level.
/// All parameters are incorporated by the message macro
///
/// \param type the level of the logging message
/// \param message additional message to incorporate
/// \param module the module the source file belongs to
/// \param className the name of the class the function belongs to
/// \param function the name of the function where the message was raised
/// \param file source file name
/// \param line source file line
void QnCorrectionsPrintMessageHandler(UInt_t type, const char* message,
                          const char* module, const char* className,
                          const char* function, const char* file, Int_t line)
{
// print the given message

  TString sLocation = Form("%s/%s::%s: (%s:%.0d)",module,className,function,file,line);

  if (message != NULL) {
    if (!(type < nLoggingLevel)) {
      switch (type) {
      case kInfo:
        Info(Form("I-%s", (const char *) sLocation), "%s", message);
        break;
      case kWarning:
        Warning(Form("W-%s", (const char *) sLocation), "%s", message);
        break;
      case kError:
        Error(Form("E-%s", (const char *) sLocation), "%s", message);
        break;
      case kFatal:
        Fatal(Form("FATAL-%s", (const char *) sLocation), "%s", message);
        break;
      }
    }
  }
  else {
    if (!(type < nLoggingLevel)) {
      switch (type) {
      case kInfo:
        Info(Form("I-%s", (const char *) sLocation), "%s", " ");
        break;
      case kWarning:
        Warning(Form("W-%s", (const char *) sLocation), "%s", " ");
        break;
      case kError:
        Error(Form("E-%s", (const char *) sLocation), "%s", " ");
        break;
      case kFatal:
        Fatal(Form("FATAL-%s", (const char *) sLocation), "%s", " ");
        break;
      }
    }
  }
}

