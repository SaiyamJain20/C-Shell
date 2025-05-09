#include "../main.h"

int compareByPid(const void *a, const void *b) {
    const backGroundProcess *processA = (const backGroundProcess *)a;
    const backGroundProcess *processB = (const backGroundProcess *)b;

    if (processA->pid < processB->pid) return -1;
    if (processA->pid > processB->pid) return 1;
    return 0;
}

void printActivities() {
    if(numberOfBackGroundProcess == 0) {
        fprintf(outputFile, BLUE BOLD "No process found" RESET);
        fprintf(outputFile, "\n");
        return;
    }

    if(numberOfBackGroundProcess != 1) {
        qsort(bgList, numberOfBackGroundProcess, sizeof(backGroundProcess), compareByPid);
    }

    for (int i = 0; i < numberOfBackGroundProcess; i++) {
        char proc_path[256];
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", bgList[i].pid);

        int fd = open(proc_path, O_RDONLY);
        if (fd < 0) {
            perror("Failed to open proc file");
            return;
        }

        char buffer[MAX_PATH_LENGTH];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Failed to read proc file");
            close(fd);
            continue; 
        }
        close(fd);
        char state[10];

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            if (sscanf(buffer, "%*d %*s %s", state) != 1) {
                fprintf(stderr, "Failed to parse process state for PID %d\n", bgList[i].pid);
                continue; 
            }
        }

        if(state[0] == 'T') {
            printf(RED BOLD "%d : %s - Stopped" RESET, bgList[i].pid, bgList[i].cmdName);
        } else {
            printf(GREEN BOLD "%d : %s - Running" RESET, bgList[i].pid, bgList[i].cmdName);
        }

        printf("\n");
    }

    return;
}