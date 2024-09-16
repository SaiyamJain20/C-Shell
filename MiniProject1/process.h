#ifndef __PROCESS__
#define __PROCESS__

#include "globalVariable.h"

void process(char *in);
void foreGroundProcessFunction(char *tkn);
void backGroundProcessFunction(char *tkn);
int handleFork(char *args[256]);

#endif