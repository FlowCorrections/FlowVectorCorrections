#ifndef QNCORRECTIONS_LOG_H
#define QNCORRECTIONS_LOG_H

/***************************************************************************
 * Package:       FlowVectorCorrections                                    *
 * Authors:       Jaap Onderwaater, GSI, jacobus.onderwaater@cern.ch       *
 *                Ilya Selyuzhenkov, GSI, ilya.selyuzhenkov@gmail.com      *
 *                Víctor González, UCM, victor.gonzalez@cern.ch            *
 *                Contributors are mentioned in the code where appropriate.*
 * Development:   2012-2016                                                *
 * See cxx source for GPL licence et. al.                                  *
 ***************************************************************************/

/// \file QnCorrectionsLog.h
/// \brief Logging function support for the Q vector correction framework

#ifndef ALICEFRAMEWORK
#include <TClass.h>
#include <TError.h>

#ifdef _MODULE_
/// Macro that extracts the module name
# define MODULENAME() _MODULE_
#else
/// Macro that extracts the module name
# define MODULENAME() "NoModule"
#endif

// function name macro
#if defined(__GNUC__) || defined(__ICC) || defined(__ECC) || defined(__APPLE__)
/// Macro that extracts the function name
# define FUNCTIONNAME() __FUNCTION__
#else
/// Macro that extracts the function name
# define FUNCTIONNAME() "???"
#endif

extern void QnCorrectionsPrintMessageHandler(UInt_t type, const char* message,
                          const char* module, const char* className,
                          const char* function, const char* file, Int_t line);
extern void QnCorrectionsSetTracingLevel(UInt_t level);

/// Actual way to invoke the logging function. It is
/// a macro that incorporates the additional information needed
/// for locating the source code the message was raised.
/// \param lvl level of the logging message
/// \param message meaningful message to print
#define QnCorrectionsMessage(lvl,message) do { \
      QnCorrectionsPrintMessageHandler(lvl, message, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false)

/// User function for an Info message
#define QnCorrectionsInfo(message)               QnCorrectionsMessage(kInfo, message)
/// User function for a Warning message
#define QnCorrectionsWarning(message)            QnCorrectionsMessage(kWarning, message)
/// User function for an Error message
#define QnCorrectionsError(message)              QnCorrectionsMessage(kError, message)
/// User function for a Fatal message: stop execution
#define QnCorrectionsFatal(message)              QnCorrectionsMessage(kFatal, message)

#else /* ALICEFRAMEWORK */
#include "AliLog.h"
#endif /* ALICEFRAMEWORK */
#endif /* QNCORRECTIONS_LOG_H */
