#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void print_process_info(pid_t pid) {
    char proc_path[256];
    char buffer[1024];
    int fd;

    // Print the PID
    printf("PID: %d\n", pid);

    // Open /proc/<pid>/stat
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);
    fd = open(proc_path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open stat file");
        return;
    }

    // Read the /proc/<pid>/stat content
    if (read(fd, buffer, sizeof(buffer) - 1) <= 0) {
        perror("Failed to read stat file");
        close(fd);
        return;
    }
    close(fd);

    // Parse the needed fields from /proc/<pid>/stat
    int pgrp;
    long unsigned vsize;
    char state;
    sscanf(buffer, "%*d %*s %c %*d %d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*d %*d %lu", 
           &state, &pgrp, &vsize);

    // Print process status
    printf("Process status: %c\n", state);

    // Print process group
    printf("Process Group: %d\n", pgrp);

    // Print virtual memory size
    printf("Virtual memory: %lu\n", vsize);

    // Get the executable path
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    char exe_path[5001];
    ssize_t len = readlink(proc_path, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';  // Null-terminate the path
        printf("Executable path: %s\n", exe_path);
    } else {
        perror("Failed to read executable path");
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;

    if (argc > 1) {
        // If an argument is provided, use it as the PID
        pid = atoi(argv[1]);
    } else {
        // If no argument is provided, use the PID of the shell
        pid = getppid(); // Get parent process ID, which should be the shell
    }

    // Print process information
    print_process_info(pid);

    return 0;
}
