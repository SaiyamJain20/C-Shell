#ifndef __LOG__
#define __LOG__

#include "globalVariable.h"
#define MAX_HISTORY_SIZE 15

void saveToLog(char *str);
void printHistory();
void purgeHistory();
void executeHistory(int commandNumber);
void handleLog(char *str); 
bool containsLog(char *in);

#endif