#include <TString.h>
#include "QnCorrectionsLog.h"

void QnCorrectionsPrintMessageHandler(UInt_t type, const char* message,
                          const char* module, const char* className,
                          const char* function, const char* file, Int_t line)
{
// print the given message

  TString sLocation = Form("I-%s/%s::%s: (%s:%.0d)",module,className,function,file,line);

  if (message != NULL) {
    Info((const char *) sLocation, "\n   %s", message);
  }
  else {
    Info((const char *) sLocation,"%s", " ");
  }
}

