#ifndef __MAIN__
#define __MAIN__

typedef struct backGroundProcessList backGroundProcessList;
typedef struct completedBackGroundProcessList completedBackGroundProcessList;
typedef backGroundProcessList *bgList;
typedef completedBackGroundProcessList *cmpList;

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
    struct backGroundProcessList *ptr;
} backGroundProcess;

struct backGroundProcessList {
    struct backGroundProcess *processDetails;
    struct backGroundProcessList *next;
    struct backGroundProcessList *prev;
};

struct completedBackGroundProcessList {
    struct backGroundProcess *processDetails;
    struct completedBackGroundProcessList *next;
};

#include "globalVariable.h"


extern char *abslutePath;
extern char *cwd;
extern char *PrevWD;
extern bool prevInsideWorkingDirectory;
extern bool insideWorkingDirectory;
extern int logFile;
extern bool exitProgram;
extern bgList BGList;
extern cmpList CMPList;


#include "username.h"
#include "tokenize.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "backGroundProcess.h"
#include "process.h"
#include "proclore.h"

#endif