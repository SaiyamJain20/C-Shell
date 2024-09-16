#ifndef __GLOBAL__VARIBLES__
#define __GLOBAL__VARIABLES__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>         //for strtok
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

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
#include <pwd.h>
#include <grp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h> 

#define RESET       "\x1b[0m"
#define BLACK       "\x1b[30m"
#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m"
#define YELLOW      "\x1b[33m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define WHITE       "\x1b[37m"

#define BOLD        "\x1b[1m"
#define UNDERLINE   "\x1b[4m"
#define REVERSED    "\x1b[7m"

#define MAX_INPUT_LENGTH 5001
#define MAX_PATH_LENGTH 5001
#define MAX_TIME_STR_LENGTH 5001
#define MAX_DISPLAY_LENGTH 5001
#define MAX_FILES 5001
#define MAX_BACKGROUND_PROCESSES 5001
#define MAX_FUNCTION_LENGTH 101

#endif