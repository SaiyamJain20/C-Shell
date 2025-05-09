#include "../main.h"

void bootShell() {
    strcpy(abslutePath, currentWorkingDirectory());
    strcpy(cwd, currentWorkingDirectory());
    PrevWD[0] = '\0';

    prevInsideWorkingDirectory = true;
    insideWorkingDirectory = true;
    inputFile = stdin;
    outputFile = stdout;

    logFile = open("../log.txt", O_RDWR | O_CREAT, 0644);
    if (logFile < 0) {
        perror("Error opening history file\n");
        perror("CRASHING\n");
        exit(1);
    }
    
    exitProgram = false;
    extraTime[0] = '\0';
    pipeLine = false;
    bgList = malloc(sizeof(backGroundProcess) * MAX_BACKGROUND_PROCESSES);
    numberOfBackGroundProcess = 0;
    shellPID = (int)getpid();
    currentPID = (int)getpid();
    neonateCalled = false;
    aliaslist = NULL;
    funclist = NULL;
    isLastCommandBG = false;

    readFile();

    return;
}

void changeCurrentPID(int pid) {
    currentPID = pid;
    return;
} 

void trim_spaces(char *str) {
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    if (*start == '\0') {
        *str = '\0';
        return;
    }

    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n')) {
        end--;
    }

    *(end + 1) = '\0';
    memmove(str, start, strlen(start) + 1);
    return;
}

void freeShell() {
    close(logFile);
    restoreFiles();
    free(bgList);

    while(aliaslist != NULL) {
        aliasList *new = aliaslist;
        aliaslist = aliaslist->next;
        free(new);
    }

    while(funclist != NULL) {
        funcList *new = funclist;
        funclist = funclist->next;
        free(new);
    }
}