#include "proclore.h"

bool proclore(char *tkn) {
    if(tkn == NULL) return false;

    char *saveptr;
    saveptr = malloc((strlen(tkn) + 1) * sizeof(char));
    strcpy(saveptr, tkn);
    char *firstCommand = strtok_r(NULL, " \t\n", &saveptr);
    char *lastCommand = strtok_r(NULL," \t\n", &saveptr);
    pid_t processId;

    if(lastCommand == NULL) {
        processId = 0;
    } else {
        processId = atoi(lastCommand);
    }

    pid_t processGroupId = getpgid(processId);
    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", processGroupId);

    int fd = open(proc_path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open proc file");
        return false;
    }

    char buffer[5001];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        int pgrp, tgpid;
        long unsigned vsize;
        char state;
        sscanf(buffer, "%*d %*s %c %*d %d %*d %*d %d %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d %*d %lu", 
            &state, &pgrp, &tgpid, &vsize);

        snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", processGroupId);
        char exe_path[5001];
        ssize_t len = readlink(proc_path, exe_path, sizeof(exe_path) - 1);


        if (len != -1) {
            exe_path[len] = '\0';
            printf("PID: %d\n", processId);
            printf("Process status: %c", state);
            if(tgpid == pgrp){
                printf("+");
            }
            printf("\nProcess Group: %d %d\n", pgrp, tgpid);
            printf("Virtual memory: %lu\n", vsize);
            printf("Executable path: %s\n", exe_path);
        } else {
            perror("Failed to read executable path");
        }
    } else {
        perror("Failed to read proc file");
    }

    close(fd);
    return true;
}   