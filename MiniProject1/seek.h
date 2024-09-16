#ifndef __SEEK__
#define __SEEK__

#include "globalVariable.h"

void seek(char *str);
void printSeek(char *path, char *search, int *arr, bool *matchFound, int *cnt, int len, char *match, bool *isDirectory);
bool startsWith(const char *pre, const char *str);

#endif