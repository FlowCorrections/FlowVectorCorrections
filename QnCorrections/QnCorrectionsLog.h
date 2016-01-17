#ifndef QNCORRECTIONS_LOG_H
#define QNCORRECTIONS_LOG_H

#ifndef ALICEFRAMEWORK
#include <TClass.h>
#include <TError.h>

#ifdef _MODULE_
# define MODULENAME() _MODULE_
#else
# define MODULENAME() "NoModule"
#endif

// function name macro
#if defined(__GNUC__) || defined(__ICC) || defined(__ECC) || defined(__APPLE__)
# define FUNCTIONNAME() __FUNCTION__
// #elif defined(__HP_aCC) || defined(__alpha) || defined(__DECCXX)
// #define FUNCTIONNAME() __FUNC__
#else
# define FUNCTIONNAME() "???"
#endif

extern void QnCorrectionsPrintMessageHandler(UInt_t type, const char* message,
                          const char* module, const char* className,
                          const char* function, const char* file, Int_t line);
extern void QnCorrectionsSetTracingLevel(UInt_t level);

#define QnCorrectionsMessage(lvl,message) do { \
      QnCorrectionsPrintMessageHandler(lvl, message, MODULENAME(), ClassName(), FUNCTIONNAME(), __FILE__, __LINE__);} while(false)


#define QnCorrectionsInfo(message)               QnCorrectionsMessage(kInfo, message)
#define QnCorrectionsWarning(message)            QnCorrectionsMessage(kWarning, message)
#define QnCorrectionsError(message)              QnCorrectionsMessage(kError, message)
#define QnCorrectionsFatal(message)              QnCorrectionsMessage(kFatal, message)

#else /* ALICEFRAMEWORK */
#include "AliLog.h"
#endif /* ALICEFRAMEWORK */
#endif /* QNCORRECTIONS_LOG_H */
