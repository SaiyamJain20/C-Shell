#include "../main.h"

void hop(char *str) {
    if(str == NULL){
        return;
    }

    char *token;
    char *svPtr = str;
    token = strtok_r(svPtr, " \t\n", &svPtr);   

    while(token != NULL && (token[0] == '<' || token[0] == '>')) {
        trim_spaces(token);
        token = strtok_r(NULL, " \t\n", &svPtr);
        if(token == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        token = strtok_r(NULL, " \t\n", &svPtr);
    }

    if(token == NULL) {
        perror("Invalid Command");
        return;
    }

    token = strtok_r(NULL, " \t\n", &svPtr);

    if(token == NULL){
        int try;
        try = chdir(cwd);
        if(try == -1){
            perror("Failed to change directory\n");
            return;
        }
        strcpy(abslutePath, currentWorkingDirectory());
        insideWorkingDirectory = true;
        fprintf(outputFile, YELLOW BOLD "%s" RESET, abslutePath);
        fprintf(outputFile, "\n");
    } else {
        while(token != NULL) {
            int try;
            if(strcmp(token,"~") == 0){
                try = chdir(cwd);
            } else if (token[0] == '~') {
                char *temp;
                temp = malloc(MAX_PATH_LENGTH * sizeof(char));
                strcpy(temp, cwd);
                strcat(temp, token + 1);
                try = chdir(temp);
            }else if(strcmp(token, "-") == 0) {
                if(PrevWD[0] == '\0'){
                    perror("Failed to change directory\n");
                    token = strtok_r(NULL, " \t\n", &svPtr);
                    continue;
                }
                try = chdir(PrevWD);
            }
            else {
                try = chdir(token);
            }
            if(try == -1){
                perror("Failed to change directory\n");
                return;
            }

            strcpy(PrevWD, abslutePath);
            prevInsideWorkingDirectory = insideWorkingDirectory;

            strcpy(abslutePath, currentWorkingDirectory());
            if(strncmp(abslutePath, cwd, strlen(cwd) - 1) == 0){
                insideWorkingDirectory = true;
            } else {
                insideWorkingDirectory = false;
            }

            fprintf(outputFile, YELLOW BOLD "%s" RESET, abslutePath);
            fprintf(outputFile, "\n");
            token = strtok_r(NULL, " \t\n", &svPtr);
            if(token != NULL && (token[0] == '<' || token[0] == '>')){
                return;
            }
        }
    }

    return;
}

void cd(char *str) {
    if(str == NULL){
        return;
    }

    char *token;
    char *svPtr = str;
    token = strtok_r(svPtr, " \t\n", &svPtr);  

    while(token != NULL && (token[0] == '<' || token[0] == '>')) {
        trim_spaces(token);
        token = strtok_r(NULL, " \t\n", &svPtr);
        if(token == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        token = strtok_r(NULL, " \t\n", &svPtr);
    }

    if(token == NULL) {
        perror("Invalid Command");
        return;
    }

    token = strtok_r(NULL, " \t\n", &svPtr);

    if(token == NULL){
        int try;
        try = chdir(cwd);
        if(try == -1){
            perror("Failed to change directory\n");
            return;
        }
        strcpy(abslutePath, currentWorkingDirectory());
        insideWorkingDirectory = true;
    } else {
        char *token2 = strtok_r(NULL, " \t\n", &svPtr);
        if(token2 != NULL){
            trim_spaces(token2);
            if((token2[0] != '<' && token2[0] != '>' && token2[0] != '|')){
                return;
            }
        }

        int try;
        if(strcmp(token,"~") == 0){
            try = chdir(cwd);
        } else if (token[0] == '~') {
            char *temp;
            temp = malloc(MAX_PATH_LENGTH * sizeof(char));
            strcpy(temp, cwd);
            strcat(temp, token + 1);
            try = chdir(temp);
        } else if(strcmp(token, "-") == 0) {
            if(PrevWD[0] == '\0'){
                perror("Failed to change directory\n");
                return;
            }
            try = chdir(PrevWD);
        }
        else {
            try = chdir(token);
        }
        if(try == -1){
            perror("Failed to change directory\n");
            return;
        }

        strcpy(PrevWD, abslutePath);
        prevInsideWorkingDirectory = insideWorkingDirectory;

        strcpy(abslutePath, currentWorkingDirectory());
        if(strncmp(abslutePath, cwd, strlen(cwd) - 1) == 0){
            insideWorkingDirectory = true;
        } else {
            insideWorkingDirectory = false;
        }

        if(strcmp(token, "-") == 0){
            fprintf(outputFile, MAGENTA BOLD "%s" RESET, abslutePath);
            fprintf(outputFile, "\n");
        }
    }

    return;
}