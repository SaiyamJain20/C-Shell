#ifndef __PING__
#define __PING__

#include "globalVariable.h"

void handleSignal(char *tkn);
void sendSignal(int pid, int signal);
void handleCtrlC();
void handleCtrlD();
void handleCtrlZ();

#endif