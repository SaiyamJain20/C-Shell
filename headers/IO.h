#ifndef __IO__
#define __IO__

#include "globalVariable.h"

void changeInputFile(char *inputFileName);
void changeOutputFileWrite(char *outputFileName);
void changeOutputFileAppend(char *outputFileName);
void handleIORedirection(char *str);
void restoreFiles();
bool checkForMultipleIO(char *tkn);

#endif