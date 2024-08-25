#include "main.h"

bool hop(char *str, char **abs, char *cwd, char *prevWD, bool *insideWorkingDir, bool *prevWDInsideWorkingDir);
bool cd(char *str, char **abs, char *cwd, char *prevWD, bool *insideWorkingDir, bool *prevWDInsideWorkingDir);