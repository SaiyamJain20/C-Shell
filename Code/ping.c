#include "../main.h"

void handleSignal(char *tkn) {
    char *temp;
    temp = malloc(strlen(tkn) + 1);
    strcpy(temp, tkn);

    char *svPtr;
    svPtr = malloc(strlen(tkn) + 1);
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

    int pid = atoi(svPtr);

    if(kill(pid, 0) == -1){
        perror("No such process found");
        return;
    }


    svPtr = strtok_r(NULL, " \t\n", &temp);
    int signal = atoi(svPtr);

    signal = signal % 32;

    sendSignal(pid, signal);

    return;
}

void sendSignal(int pid, int signal) {
    if (kill(pid, signal) == -1) {
        perror("Error sending signal");
        return;
    }
    fprintf(outputFile ,GREEN BOLD "Sent signal %d to process with pid %d" RESET, signal, pid);
    fprintf(outputFile, "\n");
    return;
}

void handleCtrlC() {
    if(currentPID == shellPID) {
        return;
    }

    printf("\n");
    sendSignal(currentPID, SIGINT);
    changeCurrentPID(shellPID);
    return;
}

void handleCtrlD() {
    fprintf(outputFile ,MAGENTA BOLD "Logging Out" RESET);
    fprintf(outputFile ,"\n");
    killAllBackGroundProcess();
    exit(0);
}

void handleCtrlZ() {
    int tempPID = currentPID;
    if(currentPID == shellPID) {
        return;
    }

    printf("\n");
    changeCurrentPID(shellPID);
    sendSignal(tempPID, SIGTSTP);
    
    backGroundProcess *process;
    process = malloc(sizeof(backGroundProcess));
    process->pid = tempPID;

    char path[MAX_PATH_LENGTH];
    char cmdName[MAX_INPUT_LENGTH];
    snprintf(path, sizeof(path), "/proc/%d/comm", process->pid);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening /proc/pid/comm");
        return;
    }

    if (fgets(cmdName, MAX_INPUT_LENGTH, file) == NULL) {
        perror("Error reading /proc/pid/comm");
        fclose(file);
        return;
    }

    size_t len = strlen(cmdName);
    if (len > 0 && cmdName[len - 1] == '\n') {
        cmdName[len - 1] = '\0';
    }

    fclose(file);
    strcpy(process->cmdName, cmdName);
    process->running_status = Blocked;
    addNewBgProcess(process);

    return;
}