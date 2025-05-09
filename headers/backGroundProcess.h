#ifndef __BACKGROUNDPROCESS__
#define __BACKGROUNDPROCESS__

#include "globalVariable.h"

void addNewBgProcess(backGroundProcess *process);
void deleteBackGroundProcess(int idx);
void killAllBackGroundProcess();
void printCompletedBgProcesses();

#endif