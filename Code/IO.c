#include "../main.h"

void changeInputFile(char *inputFileName) {
    inputFile = freopen(inputFileName, "r", stdin);
    if (inputFile == NULL) {
        perror("Error opening file\n");
        return;
    }
    return;
}

void changeOutputFileWrite(char *outputFileName) {
    outputFile = freopen(outputFileName, "w", stdout);
    if (outputFile == NULL) {
        perror("Error opening file\n");
        return;
    }
    return;
}

void changeOutputFileAppend(char *outputFileName) {
    outputFile = freopen(outputFileName, "a", stdout);
    if (outputFile == NULL) {
        perror("Error opening file\n");
        return;
    }
    return;
}

void handleIORedirection(char *str) {
    if (strchr(str, '<') != NULL) {
        char *temp = malloc((strlen(str) + 1) * sizeof(char));
        if (temp == NULL) {
            perror("Error allocating memory\n");
            return;
        }
        strcpy(temp, str);

        trim_spaces(temp);

        char *svPtr;
        char *afterRedirect; 
        char *beforeRedirect = strtok_r(temp, "<", &svPtr);
        if(temp[0] != '<'){
            afterRedirect = strtok_r(NULL, "<", &svPtr);
        } else {
            afterRedirect = beforeRedirect;
        }

        if (afterRedirect == NULL) {
            perror("No redirection file given");
            free(temp);
            return;
        }

        char *trimmedStart = strtok(afterRedirect, " \t\n");
        if (trimmedStart == NULL) {
            perror("No redirection file given");
            free(temp);
            return;
        }

        changeInputFile(trimmedStart);
        free(temp);
    }

    if (strchr(str, '>') != NULL) {
        if (strstr(str, ">>") != NULL) {
            char *temp = malloc((strlen(str) + 1) * sizeof(char));
            if (temp == NULL) {
                perror("Error allocating memory\n");
                return;
            }
            strcpy(temp, str);

            trim_spaces(temp);

            char *afterRedirect;
            char *svPtr;
            char *beforeRedirect = strtok_r(temp, ">>", &svPtr);
            if(temp[0] == '>' && temp[1] == '>'){
                afterRedirect = beforeRedirect;
            }
            else {
                afterRedirect = strtok_r(NULL, ">>", &svPtr);
            }

            if (afterRedirect == NULL) {
                perror("No redirection file given");
                free(temp);
                return;
            }

            char *trimmedStart = strtok(afterRedirect, " \t\n");
            if (trimmedStart == NULL) {
                perror("No redirection file given");
                free(temp);
                return;
            }

            changeOutputFileAppend(trimmedStart);
            free(temp);
        } 
        if (strstr(str, ">") != NULL && strstr(str, ">>") == NULL) {
            char *temp = malloc((strlen(str) + 1) * sizeof(char));
            if (temp == NULL) {
                perror("Error allocating memory");
                return;
            }
            strcpy(temp, str);

            trim_spaces(temp);

            char *afterRedirect;
            char *svPtr;
            char *beforeRedirect = strtok_r(temp, ">", &svPtr);
            if(temp[0] == '>' && temp[1] != '>'){
                afterRedirect = beforeRedirect;
            }
            else {
                afterRedirect = strtok_r(NULL, ">", &svPtr);
            }

            if (afterRedirect == NULL) {
                perror("No redirection file given");
                free(temp);
                return;
            }

            char *trimmedStart = strtok(afterRedirect, " \t\n");
            if (trimmedStart == NULL) {
                perror("No redirection file given");
                free(temp);
                return;
            }

            changeOutputFileWrite(trimmedStart);
            free(temp);
        }
    }
}

void restoreFiles() {
    fflush(inputFile);
    fflush(outputFile);

    fclose(inputFile);
    fclose(outputFile);

    if (freopen("/dev/tty", "r", stdin) == NULL) {
        perror("Failed to restore stdin");
        exit(1);
    }

    if (freopen("/dev/tty", "w", stdout) == NULL) {
        perror("Failed to restore stdout");
        exit(1);
    }
    return;
}

bool checkForMultipleIO(char *tkn) {
    int input = 0, output = 0, append = 0;
    char *temp = malloc(MAX_INPUT_LENGTH);
    strcpy(temp, tkn);

    char *svPtr = strtok_r(temp, " \t\n", &temp);

    while(svPtr != NULL) {
        trim_spaces(svPtr);
        if(strncmp(svPtr,"<<",2) == 0) {
            append++;
        } else if(strncmp(svPtr, "<", 1) == 0) {
            output++;
        } else if(strncmp(svPtr, ">", 1) == 0) {
            input++;
        }
        svPtr = strtok_r(NULL, " \t\n", &temp);
    }

    if(output != 0 && append != 0) {
        perror("Multiple IO Redirection given");
        return true;
    }
    if(output > 1 || input > 1 || append > 1) {
        perror("Multiple IO Redirection given");
        return true;
    }

    return false;
}