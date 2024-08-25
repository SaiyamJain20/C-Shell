#ifndef __MAIN__
#define __MAIN__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>         //for strtok
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

#include <sys/utsname.h>    //for uname
#include <unistd.h>         //for hostname, getpid, exec, fork, getopt, getcwd, sleep
#include <sys/signal.h>     //for signal
#include <sys/wait.h>       //for wait
#include <signal.h>         //for kill
#include <dirent.h>         //for readdir, opendir, closedir
#include <sys/types.h>      //for opendir, closedir
#include <sys/stat.h>       //for struct stat
#include <fcntl.h>
#include <termios.h>
#include <netdb.h>
#include <pwd.h>
#include <grp.h>

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

#include "username.h"
#include "tokenize.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "backGroundProcess.h"
#include "process.h"

#endif