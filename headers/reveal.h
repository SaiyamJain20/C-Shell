#ifndef __REVEAL__
#define __REVEAL__

#include "globalVariable.h"

#define MAX_DIR_ITEMS 1024

int compare(const void *arg1, const void *arg2);
char file_type(mode_t mode);
int ls_print(char **file);
int ls_l_print(char **file);
void reveal (char *str);
void revealHelper (char *dest, bool has_a, bool has_l);

#endif