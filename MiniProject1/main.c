#include "main.h"

char abslutePath[MAX_PATH_LENGTH];
char cwd[MAX_PATH_LENGTH];
char PrevWD[MAX_PATH_LENGTH];
bool prevInsideWorkingDirectory;
bool insideWorkingDirectory;
int logFile;
bool exitProgram;
FILE *inputFile;
FILE *outputFile;
char extraTime[MAX_TIME_STR_LENGTH];;
bool pipeLine;
int pipefd[2];
int bgPipefd[2];
bool isLastCommandBG;
backGroundProcess *bgList;
int numberOfBackGroundProcess;
int shellPID;
int currentPID;
volatile bool neonateCalled;
struct termios original_termios;
aliasList *aliaslist;
funcList *funclist;

int main(){
    bootShell();
    char *in;
    in = malloc(sizeof(char) * MAX_INPUT_LENGTH);

    signal(SIGTSTP, handleCtrlZ);
    signal(SIGINT, handleCtrlC);

    while(1){
        printCompletedBgProcesses();
        displayRequirments();
        memset(extraTime, 0, sizeof(char) * MAX_TIME_STR_LENGTH);
        memset(in, 0, sizeof(char) * MAX_INPUT_LENGTH);
        if(fgets(in, MAX_INPUT_LENGTH, inputFile) == NULL) {
            handleCtrlD();
            continue;
        }

        if(in[0] == '\n') {
            continue;
        }

        if(containsLog(in)) {
            saveToLog(in);
        }

        process(in);
        if(exitProgram == true){
            break;
        }
    }

    free(in);
    freeShell();

    return 0;
}