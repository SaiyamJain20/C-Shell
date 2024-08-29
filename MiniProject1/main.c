#include "main.h"

char *abslutePath;
char *cwd;
char *PrevWD;
bool prevInsideWorkingDirectory;
bool insideWorkingDirectory;
int logFile;
bool exitProgram;
bgList BGList;
cmpList CMPList;


int main(){
    abslutePath = malloc(sizeof(char) * 5001);
    abslutePath = currentWorkingDirectory();

    cwd = malloc(sizeof(char) * 5001);
    cwd = currentWorkingDirectory();

    PrevWD = malloc(sizeof(char) * 5001);
    PrevWD[0] = '\0';

    prevInsideWorkingDirectory = true;
    insideWorkingDirectory = true;

    char *in;
    in = malloc(sizeof(char) * 5001);

    logFile = open("log.txt", O_RDWR | O_CREAT, 0644);
    if (logFile < 0) {
        perror("Error opening history file");
        return 1;
    }
    
    exitProgram = false;
    BGList = NULL;
    CMPList = NULL;

    while(1){
        displayRequirments(); 
        fgets(in, 5001, stdin);
        CMPList = printProceesCmpList();

        char *logSavePtr = (char *)malloc(sizeof(char) * (strlen(in) + 10));
        bool containsLog = false;
        strcpy(logSavePtr, in);
        char *temp;
        temp = strtok_r(logSavePtr, " \t\n;&", &logSavePtr);
        
        while(temp != NULL){
            if(strcmp(temp, "log") == 0){
                containsLog = true;
                break;
            }
            temp = strtok_r(NULL, " \t\n;&", &logSavePtr);
        }

        if(containsLog == false) {
            bool savedToLOG = saveToLog(in);
            if(!saveToLog){
                perror("saveToLog");
            }
        }

        process(in);
        if(exitProgram == true){
            break;
        }
    }

    close(logFile);

    return 0;
}