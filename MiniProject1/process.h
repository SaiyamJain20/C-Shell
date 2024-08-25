#include "main.h"

void process(char *in, char **abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, int logFile, bool *exitProgram, bgList *BGList, cmpList CMPList);
bool foreGroundProcessFunction(char *tkn, char **abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, int logFile, bool *exitProgram, bgList BGList, cmpList CMPList);
bgList backGroundProcessFunction(char *tkn, bgList BGList, cmpList CMPList);