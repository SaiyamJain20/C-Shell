#include "../main.h"

void addNewBgProcess(backGroundProcess *process){
    bgList[numberOfBackGroundProcess].pid = process->pid;
    strcpy(bgList[numberOfBackGroundProcess].cmdName, process->cmdName);
    bgList[numberOfBackGroundProcess].running_status = process->running_status;
    bgList[numberOfBackGroundProcess].exit_status = process->exit_status;
    numberOfBackGroundProcess++;
    return;
}

void deleteBackGroundProcess(int idx) {
    if(numberOfBackGroundProcess < idx) {
        return;
    }

    if(numberOfBackGroundProcess - 1 == idx) {
        numberOfBackGroundProcess--;
    } else {
        numberOfBackGroundProcess--;
        bgList[idx].pid = bgList[numberOfBackGroundProcess].pid;
        strcpy(bgList[idx].cmdName, bgList[numberOfBackGroundProcess].cmdName);
        bgList[idx].running_status = bgList[numberOfBackGroundProcess].running_status;
        bgList[idx].exit_status = bgList[numberOfBackGroundProcess].exit_status;
    }

    return;
}

void killAllBackGroundProcess() {
    for(int i = numberOfBackGroundProcess - 1; i>=0 ; i--) {
        sendSignal(bgList[i].pid, SIGKILL);
    }

    for(int i = numberOfBackGroundProcess - 1; i>=0 ; i--) {
        fprintf(outputFile, RED BOLD "%s exited abnormally (%d)" RESET, bgList[i].cmdName, bgList[i].pid);
        fprintf(outputFile, "\n");
        deleteBackGroundProcess(i); 
    }

    return;
}

void printCompletedBgProcesses() {
    for(int i = 0; i < numberOfBackGroundProcess; ) {
        int status;
        pid_t result = waitpid(bgList[i].pid, &status, WNOHANG); 

        if(result == 0) {
            i++;
        } else if(result > 0) {
            if(WIFEXITED(status)) {
                if(WEXITSTATUS(status) == 0) {
                    bgList[i].exit_status = Normal;
                    fprintf(outputFile, GREEN BOLD "%s exited normally (%d)" RESET, bgList[i].cmdName, bgList[i].pid);
                } else {
                    bgList[i].exit_status = Abnormal;
                    fprintf(outputFile, RED BOLD "%s exited abnormally (%d)" RESET, bgList[i].cmdName, bgList[i].pid);
                }
            } else if(WIFSIGNALED(status)) {
                bgList[i].exit_status = Abnormal;
                fprintf(outputFile, RED BOLD "%s exited abnormally (%d)" RESET, bgList[i].cmdName, bgList[i].pid);
            }

            fprintf(outputFile, "\n");

            deleteBackGroundProcess(i); 
        } else {
            deleteBackGroundProcess(i);
        }
    }
}
