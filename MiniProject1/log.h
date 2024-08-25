#include "main.h"
#define MAX_HISTORY_SIZE 15

bool saveToLog(char *str, int logFile);
bool printHistory(int logFile);
bool executeHistory(int commandNumber, int logFile, char * abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, bool *exitProgram, bgList BGList, cmpList CMPList);
bool handleLog(char *str, int logFile, char * abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, bool *exitProgram, bgList BGList, cmpList CMPList); 