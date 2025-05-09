#include "../main.h"

int prevPipe;

void handlePipeing(char *str) {
    trim_spaces(str);
    if(str[0] == '|' || str[strlen(str) - 1] == '|') {
        perror("Invalid use of pipe");
        return;
    }
    if(strstr(str, "||") != NULL) {
        perror("Invalid use of pipe");
        return;
    }

    char *svPtr;
    svPtr = malloc(MAX_INPUT_LENGTH);
    strcpy(svPtr, str);

    char *tempTkn, *next;
    tempTkn = malloc(MAX_INPUT_LENGTH);
    tempTkn = strtok_r(svPtr, "|", &svPtr);
    next = strtok_r(NULL, "|", &svPtr);
    int i = 1;
    
    if(tempTkn == NULL || tempTkn[0] == '\0') {
        perror("Invalid use of pipe");
        return;
    }

    while(next != NULL) {
        if(i != 1) {
            if(strchr(tempTkn, '<') != NULL) {
                perror("Invalid use of pipe");
                return;
            }
        } else {
            if (strchr(tempTkn, '&') != NULL) {
                char *svPtr2;
                svPtr2 = malloc(MAX_INPUT_LENGTH);
                strcpy(svPtr2, str);
                char *tempTkn2, *next2;
                tempTkn2 = malloc(MAX_INPUT_LENGTH);
                tempTkn2 = strtok_r(svPtr2, "&", &svPtr2);
                next2 = strtok_r(NULL, "&", &svPtr2);

                trim_spaces(next2);

                if(next2[0] == '|'){
                    perror("Invalid use of pipe");
                    return;
                } else {
                    char tempTkn3[MAX_INPUT_LENGTH];
                    strcpy(tempTkn3, tempTkn2);
                    strcat(tempTkn3, " &");
                    strcpy(str, str + strlen(str) - strlen(strstr(str, "&")) + 1);
                    trim_spaces(str);
                    process(tempTkn3);
                }
                i--;
            }
        }

        if(i != 0) {
            tempTkn = next;
            next = strtok_r(NULL, "|", &svPtr);
        } else {
            strcpy(svPtr, str);
            tempTkn = strtok_r(svPtr, "|", &svPtr);
            next = strtok_r(NULL, "|", &svPtr);
        }
        i++;

        if(i != 1 && next == NULL) {
            if(strchr(tempTkn, '<') != NULL) {
                perror("Invalid use of pipe");
                return;
            }
            break;
        } else if (i != 1 && next != NULL) {
            if(strchr(tempTkn, '>') != NULL || strchr(tempTkn, '&') != NULL) {
                perror("Invalid use of pipe");
                return;
            }
        }

        trim_spaces(tempTkn);
        if(tempTkn[0] == '\0'){
            perror("Invalid use of pipe");
            return;
        }
    }

    pipeLine = true;

    char *svPtr2;
    svPtr2 = malloc(strlen(str) + 10);
    strcpy(svPtr2, str);

    char *tkn;
    tkn = malloc(strlen(str) + 10);
    tkn = strtok_r(svPtr2, "|\n", &svPtr2);
    int j = 1;
    prevPipe = -1;


    while(j <= i) {
        if(j == 1)
            DoPipeLine(tkn, true, false);
        else if(j == i) 
            DoPipeLine(tkn, false, true);
        else
            DoPipeLine(tkn, false, false);

        tkn = strtok_r(NULL, "|\n", &svPtr2);
        j++;
    }

    pipeLine = false;
    prevPipe = -1;
    return;
}

void DoPipeLine(char *tkn, bool isFirstCommand, bool isLastCommand) {
    trim_spaces(tkn);
    int pipefd[2];
    isLastCommandBG = false;

    if (!isLastCommand) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(1);
        }
    } else {
        if(strchr(tkn, '&') != NULL) {
            isLastCommandBG = true;
            if (pipe(bgPipefd) == -1) {
                perror("pipe");
                exit(1);
            }
        }
    }

    int fc = fork();
    if (fc < 0) {
        perror("fork");
        exit(1);
    } 
    else if (fc == 0) {
        if (!isFirstCommand) {
            if (dup2(prevPipe, STDIN_FILENO) == -1) {
                perror("dup2");
                exit(1);
            }
        } else {
            handleIORedirection(tkn);
        }

        if (!isLastCommand) {
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(1);
            }
        } else {
            handleIORedirection(tkn);
        }

        if (!isFirstCommand) {
            close(prevPipe);
        }

        if (!isLastCommand) {
            close(pipefd[0]);
        }

        if(isLastCommandBG == true) {
            close(bgPipefd[0]);
        }

        process(tkn);
        exit(0);
    } 
    else {
        int lastBgpid;
        if(isLastCommandBG == true) {
            close(bgPipefd[1]);
            read(bgPipefd[0], &lastBgpid, sizeof(lastBgpid));
            close(bgPipefd[0]);

            char *tempTkn = malloc(MAX_INPUT_LENGTH);
            strcpy(tempTkn, tkn);
            findInAliasList(tempTkn);

            backGroundProcess *process;
            process = (backGroundProcess *)malloc(sizeof(backGroundProcess));
            strcpy(process->cmdName, tempTkn);
            process->running_status = Running;
            process->pid = lastBgpid;
            process->pipeParentID = getpid();
            addNewBgProcess(process);
        }

        int status;
        pid_t result;
        result = waitpid(fc, &status, WNOHANG); 

        if (!isFirstCommand) {
            close(prevPipe);
        }

        if (!isLastCommand) {
            close(pipefd[1]);
            prevPipe = pipefd[0];
        }
    }

    return;
}