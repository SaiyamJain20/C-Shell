#include "hop.h"

bool hop(char *str, char **abs, char *cwd, char *prevWD, bool *insideWorkingDir, bool *prevWDInsideWorkingDir) {
    if(str == NULL){
        return false;
    }

    char *token;
    char *svPtr = str;
    token = strtok_r(svPtr, " \t\n", &svPtr);   // 1st command is hop
    token = strtok_r(NULL, " \t\n", &svPtr);

    if(token == NULL){
        int try;
        try = chdir(cwd);
        if(try == -1){
            return false;
        }
        *abs = currentWorkingDirectory();
        *insideWorkingDir = true;
        printf("%s\n", *abs);
    } else {
        while(token != NULL) {
            int try;
            if(strcmp(token,"~") == 0){
                try = chdir(cwd);
            } else if (token[0] == '~') {
                char *temp;
                strcpy(temp, cwd);
                strcat(temp, token + 1);
                try = chdir(temp);
            }else if(strcmp(token, "-") == 0) {
                if(prevWD[0] == '\0'){
                    printf("Not Present Previous Directory\n");
                    token = strtok_r(NULL, " \t\n", &svPtr);
                    continue;
                }
                try = chdir(prevWD);
            }
            else {
                try = chdir(token);
            }
            if(try == -1){
                return false;
            }

            strcpy(prevWD, *abs);
            *prevWDInsideWorkingDir = *insideWorkingDir;

            *abs = currentWorkingDirectory();
            if(strncmp(*abs, cwd, strlen(cwd) - 1) == 0){
                *insideWorkingDir = true;
            } else {
                *insideWorkingDir = false;
            }

            printf("%s\n", *abs);
            token = strtok_r(NULL, " \t\n", &svPtr);
        }
    }

    return true;
}

bool cd(char *str, char **abs, char *cwd, char *prevWD, bool *insideWorkingDir, bool *prevWDInsideWorkingDir) {
    if(str == NULL){
        return false;
    }

    char *token;
    char *svPtr = str;
    token = strtok_r(svPtr, " \t\n", &svPtr);   // 1st command is hop
    token = strtok_r(NULL, " \t\n", &svPtr);

    if(token == NULL){
        int try;
        try = chdir(cwd);
        if(try == -1){
            return false;
        }
        *abs = currentWorkingDirectory();
        *insideWorkingDir = true;
    } else {
        char *token2 = strtok_r(NULL, " \t\n", &svPtr);
        if(token2 != NULL){
            return false;
        }

        int try;
        if(strcmp(token,"~") == 0){
            try = chdir(cwd);
        } else if (token[0] == '~') {
            char *temp;
            strcpy(temp, cwd);
            strcat(temp, token + 1);
            try = chdir(temp);
        }else if(strcmp(token, "-") == 0) {
            if(prevWD[0] == '\0'){
                printf("Not Present Previous Directory\n");
                token = strtok_r(NULL, " \t\n", &svPtr);
            }
            try = chdir(prevWD);
        }
        else {
            try = chdir(token);
        }
        if(try == -1){
            return false;
        }

        strcpy(prevWD, *abs);
        *prevWDInsideWorkingDir = *insideWorkingDir;

        *abs = currentWorkingDirectory();
        if(strncmp(*abs, cwd, strlen(cwd) - 1) == 0){
            *insideWorkingDir = true;
        } else {
            *insideWorkingDir = false;
        }

        if(strcmp(token, "-") == 0)
            printf("%s\n", *abs);
    }

    return true;
}