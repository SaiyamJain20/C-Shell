#include "main.h"

void process(char *in) {
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

                    BGList = backGroundProcessFunction(prev);

                    // printf("%p\n", *BGList);
                }
                prev = bk;
                bk = strtok_r(NULL, "&", &svBk);
            }
                
            /*
                Do foreground on prev here
            */

            if(lastBG == true)
                BGList = backGroundProcessFunction(prev);
            else
                foreGroundProcessFunction(prev);
        } else {
            /*
                Do foreground on tkn here
            */

            foreGroundProcessFunction(tkn);
        }

        if(exitProgram == true) {
            break;
        }
    }

    return;
}

bool foreGroundProcessFunction(char *tkn){
    char *saveptr;
    saveptr = malloc((strlen(tkn) + 1) * sizeof(char));
    strcpy(saveptr, tkn);
    char *firstCommand = strtok_r(NULL, " \t\n", &saveptr);

    if(strcmp(firstCommand, "exit") == 0){
        exitProgram = true;
    } else if (strcmp(firstCommand, "hop") == 0) {
        bool check = hop(tkn);
        if(check == false){
            return false;
        }
    } else if (strcmp(firstCommand, "cd") == 0) {
        bool check = cd(tkn);
        if(check == false){
            return false;
        }
    } else if (strcmp(firstCommand, "reveal") == 0) {
        bool check = reveal(tkn);
        if(check == false){
            return false;
        }
    } else if (strcmp(firstCommand, "log") == 0) {
        bool check = handleLog(tkn);
        if(check == false){
            return false;
        }
    } else if(strcmp(firstCommand, "proclore") == 0) {
        bool check = proclore(tkn);
        if(check == false){
            return false;
        }

    } else {
        printf("%s\n", tkn);
        int fc = fork();
        if(fc < 0){
            perror("fork failed");
            exit(1);
        } else if(fc == 0){
            char *args[256]; 
            memset(args, 0, sizeof(args));
            int i = 0;

            char *svPtr;
            svPtr = malloc(sizeof(char) * (sizeof(tkn) + 10));
            strcpy(svPtr, tkn);
            args[i] = strtok_r(svPtr, " ", &svPtr);
            while (args[i] != NULL) {
                printf("%s ", args[i]);
                i++;
                args[i] = strtok_r(NULL, " ", &svPtr);
            }

            printf("%s\n", args[0]);

            if (execvp(args[0], args) == -1) {
                perror("Error executing command");
                exit(1);
            }

            exit(0);
        } else {
            wait(NULL);
        }
    }

    return true;
}

bgList backGroundProcessFunction(char *tkn) {
    // backGroundProcess *process;
    // process = (backGroundProcess *)malloc(sizeof(backGroundProcess));
    // backGroundProcess **processPtr = &process;
    // strcpy(process->cmdName, tkn);
    // process->running_status = Running;

    // process->ptr = NULL;
    // BGList = addNewProcessBgList(BGList, process);

    int fc = fork();
    if(fc < 0){
        perror("fork failed");
        exit(1);
    } else if(fc == 0){
        int pid = getpid();
        // process->pid = pid;

        int fc2 = fork();
        if(fc2 < 0){
            perror("fork failed");
            exit(1);
        } else if(fc2 == 0){
            char *args[256]; 
            memset(args, 0, sizeof(args));
            int i = 0;

            char *svPtr;
            svPtr = malloc(sizeof(char) * (sizeof(tkn) + 10));
            strcpy(svPtr, tkn);
            args[i] = strtok_r(svPtr, " ", &svPtr);
            while (args[i] != NULL) {
                i++;
                args[i] = strtok_r(NULL, " ", &svPtr);
            }

            if (execvp(args[0], args) == -1) {
                perror("Error executing command");
                exit(1);
            }

            exit(0);
        } else {
            int exitStatus = wait(NULL);

            if(exitStatus == 1){
                printf("%sexited abnormally (%d)\n", tkn, pid);
                // process->running_status = Finished;
                // process->exit_status = Abnormal;
                // CMPList = revomeProcessBgList(BGList, CMPList, process);
            } else {
                printf("%sexited normally (%d)\n", tkn, pid);
                // process->running_status = Finished;
                // process->exit_status = Normal;
                // CMPList = revomeProcessBgList(BGList, CMPList, process);
            }

            exit(0);
        }
    } else {
        // process->pid = fc;
        printf("%d\n", fc);
    }

    return BGList;
}