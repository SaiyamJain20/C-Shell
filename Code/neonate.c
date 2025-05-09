#include "../main.h"

int get_most_recent_pid() {
    FILE *fd = fopen("/proc/loadavg", "r");
    if (fd == NULL)
    {
        perror("Can't open /proc/loadavg file\n");
        return -1;
    }
    char data[1024];
    fgets(data, 1024, fd);
    char *tp = strtok(data, " ");
    for (int i = 0; i < 4; i++)
        tp = strtok(NULL, " ");
    return atoi(tp);
}

void enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    struct termios raw;
    raw = original_termios;

    raw.c_iflag &= ~(ICRNL | IXON); 
    // raw.c_oflag &= ~OPOST;          
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cflag |= (CS8);          
    raw.c_cc[VMIN] = 0;         
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
    return;
}

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios) == -1) {
        perror("tcsetattr");
        return;
    }
    return;
}

void neonate(char *tkn) {
    char *temp = malloc(strlen(tkn) + 1);
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

    if(svPtr == NULL || strstr(svPtr, "-n") == NULL) {
        perror("Invalid neonate arguments");
        return;
    }

    svPtr = strtok_r(NULL, " \t\n", &temp);

    if(svPtr == NULL) {
        perror("Invalid neonate arguments");
        return;
    }

    int time_arg = atoi(svPtr);

    enable_raw_mode();
    neonateCalled = true;
    struct timeval prev, now;
    gettimeofday(&prev, NULL);

    while (neonateCalled == true) {
        char ch = 0;
        if (read(STDIN_FILENO, &ch, 1) > 0 && ch == 'x') {
            neonateCalled = false;
            break;
        }

        gettimeofday(&now, NULL); 
        double elapsed = (now.tv_sec - prev.tv_sec) + (now.tv_usec - prev.tv_usec) / 1e6;

        if (elapsed >= time_arg) {
            int pid = get_most_recent_pid();
            if (pid != -1) {
                fprintf(outputFile, MAGENTA BOLD "%d" RESET, pid);
            } else {
                fprintf(outputFile ,RED BOLD "Failed to retrieve PID" RESET);
            }
            fprintf(outputFile ,"\n");
            fflush(outputFile);

            gettimeofday(&prev, NULL);
        }
    }

    disable_raw_mode();
    return;
}
