#include <TString.h>
#include "QnCorrectionsLog.h"

UInt_t nLoggingLevel = kError;

void QnCorrectionsSetTracingLevel(UInt_t level) {
  if (level < kError)
    /* don't mask errors */
    nLoggingLevel = level;
}


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
        Info(Form("I-%s", (const char *) sLocation), "\n   %s", message);
        break;
      case kWarning:
        Warning(Form("W-%s", (const char *) sLocation), "\n   %s", message);
        break;
      case kError:
        Error(Form("E-%s", (const char *) sLocation), "\n   %s", message);
        break;
      case kFatal:
        Fatal(Form("FATAL-%s", (const char *) sLocation), "\n   %s", message);
        break;
      }
    }
  }
  else {
    if (!(type < nLoggingLevel)) {
      switch (type) {
      case kInfo:
        Info(Form("I-%s", (const char *) sLocation), "\n   %s", " ");
        break;
      case kWarning:
        Warning(Form("W-%s", (const char *) sLocation), "\n   %s", " ");
        break;
      case kError:
        Error(Form("E-%s", (const char *) sLocation), "\n   %s", " ");
        break;
      case kFatal:
        Fatal(Form("FATAL-%s", (const char *) sLocation), "\n   %s", " ");
        break;
      }
    }
  }
}

