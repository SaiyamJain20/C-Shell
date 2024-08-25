#include "process.h"

void process(char *in, char **abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, int logFile, bool *exitProgram, bgList *BGList, cmpList CMPList){
    int len = strlen(in);
    char *svPtr = in;
    char *tkn;

    while(1){
        tkn = generateForeGroundToken(svPtr, &svPtr); 
        if(tkn == NULL){
            break;
        }
        int t = strlen(tkn);
        bool lastBG = false;

        if(tkn[t - 1] == '&'){
            lastBG = true;
        }

        char *bk;
        char *prev = NULL;
        char *svBk = tkn;
        bk = strtok_r(svBk, "&", &svBk);
            
        if(t != strlen(bk)){
            while(1){
                if(bk == NULL){
                    break;
                }
                if(prev != NULL){
                    /*
                        do background on prev here
                    */

                    *BGList = backGroundProcessFunction(prev, *BGList, CMPList);

                    printf("%p\n", *BGList);
                }
                prev = bk;
                bk = strtok_r(NULL, "&", &svBk);
            }
                
            /*
                Do foreground on prev here
            */

            if(lastBG == true)
                *BGList = backGroundProcessFunction(prev, *BGList, CMPList);
            else
                foreGroundProcessFunction(prev, abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory, logFile, exitProgram, *BGList, CMPList);
        } else {
            /*
                Do foreground on tkn here
            */

            foreGroundProcessFunction(tkn, abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory, logFile, exitProgram, *BGList, CMPList);
        }

        if(*exitProgram == true) {
            break;
        }
    }

    return;
}

bool foreGroundProcessFunction(char *tkn, char **abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, int logFile, bool *exitProgram, bgList BGList, cmpList CMPList){
    char *saveptr;
    saveptr = malloc((strlen(tkn) + 1) * sizeof(char));
    strcpy(saveptr, tkn);
    char *firstCommand = strtok_r(NULL, " \t\n", &saveptr);

    if(strcmp(firstCommand, "exit") == 0){
        *exitProgram = true;
    } else if (strcmp(firstCommand, "hop") == 0) {
        bool check = hop(tkn, abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory);
        if(check == false){
            return false;
        }
    } else if (strcmp(firstCommand, "cd") == 0) {
        bool check = cd(tkn, abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory);
        if(check == false){
            return false;
        }
    } else if (strcmp(firstCommand, "reveal") == 0) {
        bool check = reveal(tkn, cwd, PrevWD, *abs);
        if(check == false){
            return false;
        }
    } else if (strcmp(firstCommand, "log") == 0) {
        bool check = handleLog(tkn, logFile, *abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory, exitProgram, BGList, CMPList);
        if(check == false){
            return false;
        }
    } else {
        int fc = fork();
        if(fc < 0){
            perror("fork failed");
            exit(1);
        } else if(fc == 0){
            if (execlp("/bin/sh", "sh", "-c", tkn, NULL) == -1) {
                perror("Error executing command");
                return false;
            }
        } else {
            wait(NULL);
        }
    }

    return true;
}

bgList backGroundProcessFunction(char *tkn, bgList BGList, cmpList CMPList) {
    backGroundProcess *process;
    process = (backGroundProcess *)malloc(sizeof(backGroundProcess));
    backGroundProcess **processPtr = &process;
    strcpy(process->cmdName, tkn);
    process->running_status = Running;

    process->ptr = NULL;
    BGList = addNewProcessBgList(BGList, process);

    printf("%p\n", BGList);

    int fc = fork();
    if(fc < 0){
        perror("fork failed");
        exit(1);
    } else if(fc == 0){
        int pid = getpid();
        process->pid = pid;

        int fc2 = fork();
        if(fc2 < 0){
            perror("fork failed");
            exit(1);
        } else if(fc2 == 0){
            if (execlp("/bin/sh", "sh", "-c", tkn, NULL) == -1) {
                (*processPtr)->running_status = Finished;
                (*processPtr)->exit_status = Abnormal;
                // CMPList = revomeProcessBgList(BGList, CMPList, process);

                perror("Error executing command");
                exit(1);
            }
        } else {
            wait(NULL);
            (*processPtr)->running_status = Finished;
            (*processPtr)->exit_status = Normal;

            // process->running_status = Finished;
            // process->exit_status = Normal;
            // CMPList = revomeProcessBgList(BGList, CMPList, process);
            // if (CMPList == NULL) {
                // printf("CMPList is still NULL after revomeProcessBgList.\n");
            // } else {
            //     printf("Process with PID %d moved to CMPList successfully.\n", process->pid);
            // }
            exit(0);
        }
    } else {
        process->pid = fc;
        printf("%d\n", fc);
    }

    printf("%p\n", BGList);

    return BGList;
}