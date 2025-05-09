#include "../main.h"

struct tm *startTime;
time_t startTimetm;

void process(char *in) {
    printf("%s\n", in);
    int len = strlen(in);
    char *svPtr = in;
    char *tkn;

    memset(extraTime, 0, sizeof(extraTime));
    extraTime[0] = '\0';
    startTimetm = time(NULL);
    startTime = localtime(&startTimetm);

    while(1){
        tkn = generateForeGroundToken(svPtr, &svPtr); 
        if(tkn == NULL){
            break;
        }
        trim_spaces(tkn);

        if(strchr(tkn, '|') != NULL) {
            char temp[strlen(tkn) + 10];
            strcpy(temp, tkn);
            handlePipeing(temp);
            continue;
        }

        int t = strlen(tkn);
        bool lastBG = false;
        int q = t;
        if(tkn[q - 1] == '&'){
            lastBG = true;
        }

        while(!lastBG && q >= 0 && (tkn[q - 1] == ' ' || tkn[q - 1] == '\t' || tkn[q - 1] == '\n')){
            q--;
            if(tkn[q] == '&'){
                lastBG = true;
                break;
            }
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
                char temp[strlen(bk) + 1];
                strcpy(temp, bk);

                if(strtok(temp, " \t\n") == NULL){
                    break;
                }

                if(prev != NULL){
                    /*
                        do background on prev here
                    */

                    backGroundProcessFunction(prev);
                }
                prev = bk;
                bk = strtok_r(NULL, "&", &svBk);
            }
                
            /*
                Do foreground on prev here
            */

            if(lastBG == true)
                backGroundProcessFunction(prev);
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

void foreGroundProcessFunction(char *tkn){
    char *tempTkn = malloc(MAX_INPUT_LENGTH);
    strcpy(tempTkn, tkn);

    if(findInFuncList(tempTkn) == true) {
        return;
    }
    findInAliasList(tempTkn);

    char *saveptr;
    saveptr = malloc((strlen(tempTkn) + 1) * sizeof(char));
    strcpy(saveptr, tempTkn);
    char *firstCommand = strtok_r(saveptr, " \t\n", &saveptr);
    trim_spaces(firstCommand);

    while((firstCommand != NULL) && (firstCommand[0] == '<' || firstCommand[0] == '>')) {
        trim_spaces(firstCommand);
        firstCommand = strtok_r(NULL, " \t\n", &saveptr);
        if(firstCommand == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        firstCommand = strtok_r(NULL, " \t\n", &saveptr);
    }

    if(firstCommand == NULL) {
        perror("Invalid Command");
        return;
    }

    if(pipeLine == false){
        restoreFiles();

        if(checkForMultipleIO(tempTkn) == true) {
            return;
        }

        char temp[strlen(tempTkn) + 10];
        strcpy(temp, tempTkn);

        handleIORedirection(temp);
    }


    time_t processStartTimetm = time(NULL);
    struct tm *processStartTime;
    processStartTime = localtime(&processStartTimetm);

    if(strcmp(firstCommand, "exit") == 0){
        exitProgram = true;
        return;
    } else if (strcmp(firstCommand, "hop") == 0) {
        hop(tempTkn);
    } else if (strcmp(firstCommand, "cd") == 0) {
        cd(tempTkn);
    } else if (strcmp(firstCommand, "reveal") == 0) {
        reveal(tempTkn);
    } else if (strcmp(firstCommand, "log") == 0) {
        handleLog(tempTkn);
    } else if(strcmp(firstCommand, "proclore") == 0) {
        proclore(tempTkn);
    } else if(strcmp(firstCommand, "seek") == 0) {
        seek(tempTkn);
    } else if(strcmp(firstCommand, "activities") == 0) {
        printActivities();
    } else if(strcmp(firstCommand, "ping") == 0) {
        handleSignal(tempTkn);
    } else if(strcmp(firstCommand, "fg") == 0) {
        fg(tempTkn);
    } else if(strcmp(firstCommand, "bg") == 0) {
        bg(tempTkn);
    } else if(strcmp(firstCommand, "neonate") == 0) {
        neonate(tempTkn);
    } else if(strcmp(firstCommand, "iMan") == 0) {
        fetch_man_page(tempTkn);
    } else {
        bool hasTilde = false;
        char *temp;
        if(strcmp(firstCommand, "ls") == 0) {
            for(int i = 0; i<strlen(tempTkn); i++){
                if(tempTkn[i] == '~'){
                    tempTkn[i] = ' ';
                    temp = malloc(MAX_PATH_LENGTH * sizeof(char));
                    strcpy(temp, tempTkn);
                    strcat(temp, cwd);
                    tempTkn = temp;
                    break;
                }
                if(tempTkn[i] == '-' && (i + 1 == strlen(tempTkn) || tempTkn[i+1] == ' ')){
                    tempTkn[i] = ' ';
                    temp = malloc(MAX_PATH_LENGTH * sizeof(char));
                    strcpy(temp, tempTkn);
                    if (PrevWD[0] != '\0')
                        strcat(temp, PrevWD);
                    else{
                        perror("Not Present Previous Directory\n");
                        return;
                    }
                    tempTkn = temp;
                    break;
                }
            }
        }

        char *args[256]; 
        memset(args, 0, sizeof(args));
        int i = 0;

        char *svPtr;
        svPtr = malloc(sizeof(char) * (strlen(tempTkn) + 10));
        strcpy(svPtr, tempTkn);
        args[i] = strtok_r(svPtr, " \t\n", &svPtr);
        while (args[i] != NULL) {
            while(args[i] != NULL && (args[i][0] == '<' || args[i][0] == '>')){
                args[i] = strtok_r(NULL, " \t\n", &svPtr);
                args[i] = strtok_r(NULL, " \t\n", &svPtr);
            }

            if(args[i] == NULL) {
                break;
            }

            i++;
            args[i] = strtok_r(NULL, " \t\n", &svPtr);
        }

        i++;
        args[i] = NULL;

        int fc2 = fork();
        if(fc2 < 0){
            perror("fork failed");
            exit(1);
        } else if(fc2 == 0) {
            char temp[strlen(tempTkn) + 10];
            strcpy(temp, tempTkn);

            changeCurrentPID(getpid());

            if(pipeLine == false) {;
                handleIORedirection(temp);
                if (execvp(args[0], args) == -1) {
                    perror("execvp");
                    exit(1);
                }
            } else {
                if (execvp(args[0], args) == -1) {
                    perror("execvp");
                    exit(1);
                }
            }

            exit(1);
        } else {
            changeCurrentPID(fc2);
            int status;
            waitpid(fc2, &status, WUNTRACED);
            changeCurrentPID(shellPID);
            restoreFiles();
        }
    }

    time_t processEndTimetm = time(NULL);
    struct tm* processEndTime;
    processEndTime = localtime(&processEndTimetm);

    int diffTime = difftime(processEndTimetm, processStartTimetm);
    if(diffTime >= 3){
        char *temp;
        char *temp2 = strtok_r(tempTkn, " \t\n", &temp);
        if(extraTime[0] == '\0'){
            strcpy(extraTime, temp2);
        } else {
            strcat(extraTime, " | ");
            strcat(extraTime, temp2);
        }

        while(temp2 != NULL){
            temp2 = strtok_r(NULL, " \t\n", &temp);
            if(temp2 != NULL){
                strcat(extraTime, " ");
                strcat(extraTime, temp2);
            }
        }

        strcat(extraTime, " : [");
        char time[101];
        int totalTime = difftime(processEndTimetm, startTimetm);
        snprintf(time, sizeof(time), "%d", totalTime);
        strcat(extraTime, time);
        strcat(extraTime, "s]");
    }

    if(pipeLine == false){
        restoreFiles();
    }
    return;
}

void backGroundProcessFunction(char *tkn) {
    char *tempTkn = malloc(MAX_INPUT_LENGTH);
    strcpy(tempTkn, tkn);

    findInAliasList(tempTkn);

    if(checkForMultipleIO(tempTkn) == true) {
        return;
    }
    backGroundProcess *process;
    process = (backGroundProcess *)malloc(sizeof(backGroundProcess));
    backGroundProcess **processPtr = &process;
    strcpy(process->cmdName, tempTkn);
    process->running_status = Running;

    char *args[256]; 
    memset(args, 0, sizeof(args));
    int i = 0;

    char *svPtr;
    svPtr = malloc(sizeof(char) * (strlen(tempTkn) + 10));
    strcpy(svPtr, tempTkn);
    args[i] = strtok_r(svPtr, " \t\n", &svPtr);
    while (args[i] != NULL) {
        if(args[i] != NULL && (args[i][0] == '<' || args[i][0] == '>')){
            args[i] = strtok_r(NULL, " \t\n", &svPtr);
            args[i] = strtok_r(NULL, " \t\n", &svPtr);
        }

        if(args[i] == NULL) {
            break;
        }

        i++;
        args[i] = strtok_r(NULL, " \t\n", &svPtr);
    }

    i++;
    args[i] = NULL;

    if(isLastCommandBG) {
        process->pipeParentID = getpid();
    } else {
        process->pipeParentID = -1;
    }

    int fc = fork();
    if(fc < 0){
        perror("fork failed");
        return;
    } else if(fc == 0) {
        setpgrp();

        char temp2[MAX_INPUT_LENGTH];
        strcpy(temp2, tempTkn);
        handleIORedirection(temp2);  

        if(isLastCommandBG) {
            trim_spaces(tkn);
            int status = handleFork(args);
            if(status == 0) {
                fprintf(outputFile ,GREEN BOLD"%s exited normally (%d)" RESET, tkn, getpid());
                fprintf(outputFile ,"\n");
                exit(0);
            }
            fprintf(outputFile ,GREEN BOLD"%s exited abnormally (%d)" RESET, tkn, getpid());
            fprintf(outputFile ,"\n");
            exit(1);
        } else {
            if (execvp(args[0], args) == -1) {
                perror("execvp");
                kill(getpid(), SIGUSR2);
                exit(1);
            }
        }

        exit(0);
    } else {
        process->pid = fc;
        if(isLastCommandBG) {
            write(bgPipefd[1], &fc, sizeof(fc));
            close(bgPipefd[1]);
        }
        
        addNewBgProcess(process);
        fprintf(outputFile, "%d\n", fc);
    }

    return;
}

int handleFork(char *args[256]) {
    int fc2 = fork();
    if(fc2 < 0){
        perror(RED "fork failed" WHITE);
        exit(1);
    } else if(fc2 == 0) {
        execvp(args[0], args);
        perror(RED "Error executing command\n" WHITE);
        exit(1);
    } else {
        int status;
        waitpid(fc2, &status, 0);
        return status;
    }
}