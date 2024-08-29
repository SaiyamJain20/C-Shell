#include "main.h"

bool hop(char *str) {
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
        abslutePath = currentWorkingDirectory();
        insideWorkingDirectory = true;
        printf("%s\n", abslutePath);
    } else {
        while(token != NULL) {
            int try;
            if(strcmp(token,"~") == 0){
                try = chdir(cwd);
            } else if (token[0] == '~') {
                char *temp;
                temp = malloc(5001 * sizeof(char));
                strcpy(temp, cwd);
                strcat(temp, token + 1);
                try = chdir(temp);
            }else if(strcmp(token, "-") == 0) {
                if(PrevWD[0] == '\0'){
                    printf("Not Present Previous Directory\n");
                    token = strtok_r(NULL, " \t\n", &svPtr);
                    continue;
                }
                try = chdir(PrevWD);
            }
            else {
                try = chdir(token);
            }
            if(try == -1){
                return false;
            }

            strcpy(PrevWD, abslutePath);
            prevInsideWorkingDirectory = insideWorkingDirectory;

            abslutePath = currentWorkingDirectory();
            if(strncmp(abslutePath, cwd, strlen(cwd) - 1) == 0){
                insideWorkingDirectory = true;
            } else {
                insideWorkingDirectory = false;
            }

            printf("%s\n", abslutePath);
            token = strtok_r(NULL, " \t\n", &svPtr);
        }
    }

    return true;
}

bool cd(char *str) {
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
        abslutePath = currentWorkingDirectory();
        insideWorkingDirectory = true;
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
            temp = malloc(5001 * sizeof(char));
            strcpy(temp, cwd);
            strcat(temp, token + 1);
            try = chdir(temp);
        }else if(strcmp(token, "-") == 0) {
            if(PrevWD[0] == '\0'){
                printf("Not Present Previous Directory\n");
                token = strtok_r(NULL, " \t\n", &svPtr);
            }
            try = chdir(PrevWD);
        }
        else {
            try = chdir(token);
        }
        if(try == -1){
            return false;
        }

        strcpy(PrevWD, abslutePath);
        prevInsideWorkingDirectory = insideWorkingDirectory;

        abslutePath = currentWorkingDirectory();
        if(strncmp(abslutePath, cwd, strlen(cwd) - 1) == 0){
            insideWorkingDirectory = true;
        } else {
            insideWorkingDirectory = false;
        }

        if(strcmp(token, "-") == 0)
            printf("%s\n", abslutePath);
    }

    return true;
}