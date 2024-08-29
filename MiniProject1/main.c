#include "main.h"

int main(){
    char *abs;
    abs = malloc(sizeof(char) * 5001);
    abs = currentWorkingDirectory();

    char *cwd;
    cwd = malloc(sizeof(char) * 5001);
    cwd = currentWorkingDirectory();

    char *PrevWD;
    PrevWD = malloc(sizeof(char) * 5001);
    PrevWD[0] = '\0';

    bool prevInsideWorkingDirectory = true;
    bool insideWorkingDirectory = true;

    char *in;
    in = malloc(sizeof(char) * 5001);

    int logFile = open("log.txt", O_RDWR | O_CREAT, 0644);
    if (logFile < 0) {
        perror("Error opening history file");
        return false;
    }
    
    bool exitProgram = false;
    bgList BGList = NULL;
    cmpList CMPList = NULL;

    while(1){
        displayRequirments(abs, cwd, insideWorkingDirectory); 
        fgets(in, 5001, stdin);
        CMPList = printProceesCmpList(CMPList, BGList);

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
            bool savedToLOG = saveToLog(in, logFile);
            if(!saveToLog){
                perror("saveToLog");
            }
        }

        process(in, &abs, cwd, PrevWD, &insideWorkingDirectory, &prevInsideWorkingDirectory, logFile, &exitProgram, &BGList, CMPList);
        if(exitProgram == true){
            break;
        }
    }

    close(logFile);

    return 0;
}