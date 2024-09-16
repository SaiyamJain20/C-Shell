#ifndef __MAIN__
#define __MAIN__

#include "globalVariable.h"
typedef struct backGroundProcessList backGroundProcessList;

typedef enum exitStatus {
    Abnormal = 0,
    Normal = 1
} exitStatus;

typedef enum runningStatus {
    Running = 0, 
    Blocked = 1,
    Zombie = 2, 
    Finished = 3
} runningStatus;

typedef struct backGroundProcess {
    int pid;
    char cmdName[5001];
    runningStatus running_status;
    exitStatus exit_status;
    int pipeParentID;
} backGroundProcess;


typedef struct aliasList aliasList;
typedef struct funcList funcList;

typedef struct aliasList {
    char aliasCmd[MAX_INPUT_LENGTH];
    char Cmd[MAX_INPUT_LENGTH];
    aliasList *next;
} aliasList;

typedef struct funcList {
    char funcName[MAX_INPUT_LENGTH];
    int arguments;
    int numberOfFunctions;
    char funcBody[MAX_FUNCTION_LENGTH][MAX_INPUT_LENGTH];
    funcList *next;
} funcList;


extern char abslutePath[MAX_PATH_LENGTH];
extern char cwd[MAX_PATH_LENGTH];
extern char PrevWD[MAX_PATH_LENGTH];
extern bool prevInsideWorkingDirectory;
extern bool insideWorkingDirectory;
extern int logFile;
extern bool exitProgram;
extern FILE *inputFile;
extern FILE *outputFile;
extern char extraTime[MAX_TIME_STR_LENGTH];
extern bool pipeLine;
extern int pipefd[2];
extern int bgPipefd[2];
extern bool isLastCommandBG;
extern backGroundProcess *bgList;
extern int numberOfBackGroundProcess;
extern int shellPID;
extern int currentPID;
extern volatile bool neonateCalled;
extern struct termios original_termios;
extern aliasList *aliaslist;
extern funcList *funclist;

#include "username.h"
#include "tokenize.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "seek.h"
#include "backGroundProcess.h"
#include "process.h"
#include "alias.h"
#include "IO.h"
#include "pipe.h"
#include "activities.h"
#include "ping.h"
#include "fgbg.h"
#include "neonate.h"
#include "iMan.h"

void bootShell();
void freeShell();
void changeCurrentPID(int pid);
void trim_spaces(char *str);

#endif