#ifndef __ALIAS__
#define __ALIAS__

#include "globalVariable.h"

void readFile();
void removeComments(char *line);
void addAliasToList(char *line);
void findInAliasList(char *in);
bool findInFuncList(char *in);
void replace_placeholders(char *str, int args_count, char args[args_count][MAX_INPUT_LENGTH]);

#endif