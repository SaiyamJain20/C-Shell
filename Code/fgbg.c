#include "../main.h"

void fg(char *tkn) {
    char *temp;
    temp = malloc(MAX_INPUT_LENGTH);
    if(temp == NULL) {
        perror("malloc");
        return;
    }

    strcpy(temp, tkn);

    char *svPtr;
    svPtr = strtok_r(temp, " \t\n", &temp);

    while(svPtr != NULL && (svPtr[0] == '<' || svPtr[0] == '>')) {
        trim_spaces(svPtr);
        svPtr = strtok_r(NULL, " \t\n", &temp);
        if(svPtr == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        svPtr = strtok_r(NULL, " \t\n", &temp);
    }

    if(svPtr == NULL) {
        perror("Invalid Command");
        return;
    }

    svPtr = strtok_r(NULL, " \t\n", &temp);

    if(svPtr == NULL) {
        perror("Invalid arguments");
        return;
    }

    int pid = atoi(svPtr);

    if(kill(pid, 0) == -1){
        perror("No such process found\n");
        return;
    }

    if (tcsetpgrp(STDIN_FILENO, pid) < 0) {
        perror("Error setting foreground process group");
        return;
    }

    if (kill(pid, SIGCONT) < 0) {
        perror("Error performing SIGCONT");
        return;
    }

    int status;
    if (waitpid(pid, &status, WUNTRACED) < 0) {
        perror("Error waiting for process");
        return;
    }

    if (WIFSTOPPED(status)) {
        fprintf(outputFile ,RED BOLD "Process %d is still stopped." RESET, pid);
        fprintf(outputFile, "\n");
        return;
    }

    if (WIFEXITED(status) || WIFSIGNALED(status)) {
        for (int i = 0; i < numberOfBackGroundProcess; i++) {
            if (bgList[i].pid == pid) {
                deleteBackGroundProcess(i);
                break;
            }
        }
    }

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
            
    if (tcsetpgrp(STDIN_FILENO, shellPID) < 0) {
        perror("Error restoring shell as foreground process group");
    }

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    return;
}

void bg(char *tkn) {
    char *temp;
    temp = malloc(MAX_INPUT_LENGTH);
    if(temp == NULL) {
        perror("malloc");
        return;
    }
    strcpy(temp, tkn);

    char *svPtr;
    svPtr = strtok_r(temp, " \t\n", &temp);

    while(svPtr != NULL && (svPtr[0] == '<' || svPtr[0] == '>')) {
        trim_spaces(svPtr);
        svPtr = strtok_r(NULL, " \t\n", &temp);
        if(svPtr == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        svPtr = strtok_r(NULL, " \t\n", &temp);
    }

    if(svPtr == NULL) {
        perror("Invalid Command");
        return;
    }

    svPtr = strtok_r(NULL, " \t\n", &temp);

    if(svPtr == NULL) {
        perror("Invalid arguments");
        return;
    }

    int pid = atoi(svPtr);

    if(kill(pid, 0) == -1){
        perror("No such process found\n");
        return;
    }

    if (kill(pid, SIGCONT) < 0) {
        perror("Error SIGCONT\n");
        return;
    }

    fprintf(outputFile, CYAN BOLD "Changed to background process witd pid : %d" RESET, pid);
    fprintf(outputFile, "\n");
    return;
}