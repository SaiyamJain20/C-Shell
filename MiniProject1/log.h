#ifndef __LOG__
#define __LOG__

#include "globalVariable.h"
#define MAX_HISTORY_SIZE 15

bool saveToLog(char *str);
bool printHistory();
bool purgeHistory();
bool executeHistory(int commandNumber);
bool handleLog(char *str); 

#endif