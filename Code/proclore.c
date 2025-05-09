#include "../main.h"

void proclore(char *tkn) {
    if(tkn == NULL) return;

    char *saveptr;
    saveptr = malloc((strlen(tkn) + 1) * sizeof(char));
    strcpy(saveptr, tkn);
    char *firstCommand = strtok_r(NULL, " \t\n", &saveptr);

    while(firstCommand != NULL && (firstCommand[0] == '<' || firstCommand[0] == '>')) {
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

    char *lastCommand = strtok_r(NULL," \t\n", &saveptr);
    pid_t processId;

    if(lastCommand == NULL) {
        processId = shellPID;
    } else {
        processId = atoi(lastCommand);
    }

    pid_t processGroupId = getpgid(processId);
    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", processId);

    int fd = open(proc_path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open proc file" );
        return;
    }

    char buffer[MAX_PATH_LENGTH];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        int pgrp, tgpid;
        long unsigned vsize;
        char state;
        sscanf(buffer, "%*d %*s %c %*d %d %*d %*d %d %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d %*d %lu", &state, &pgrp, &tgpid, &vsize);

        snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", processGroupId);
        char exe_path[MAX_PATH_LENGTH];
        ssize_t len = readlink(proc_path, exe_path, sizeof(exe_path) - 1);

        if (len != -1) {
            exe_path[len] = '\0';
            fprintf(outputFile,MAGENTA BOLD "PID: " "             %d" RESET, processId);
            fprintf(outputFile, "\n");
            fprintf(outputFile,MAGENTA BOLD "Process status: " "  %c" RESET, state);
            if(tgpid == pgrp){
                fprintf(outputFile, MAGENTA BOLD "+" RESET);
            }
            fprintf(outputFile, "\n");
            fprintf(outputFile,MAGENTA BOLD "Process Group: " "   %d" RESET, pgrp);
            fprintf(outputFile, "\n");
            fprintf(outputFile,MAGENTA BOLD "Virtual memory: " "  %lu"RESET, vsize);
            fprintf(outputFile, "\n");
            fprintf(outputFile,MAGENTA BOLD "Executable path: " " %s" RESET, exe_path);
            fprintf(outputFile, "\n");
        } else {
            perror("Failed to read executable path" );
        }
    } else {
        perror("Failed to read proc file" );
    }
    
    close(fd);
    return;
}   