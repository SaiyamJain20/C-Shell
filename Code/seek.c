#include "../main.h"

bool startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

void printSeek(char *path, char *search, int *arr, bool *matchFound, int *cnt, int len, char *match, bool *isDirectory) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char fullPath[MAX_PATH_LENGTH];
        strcpy(fullPath, path);
        strcat(fullPath, "/");
        strcat(fullPath, entry->d_name);

        struct stat fileStat;
        if (stat(fullPath, &fileStat) == -1) {
            perror("Error getting file status");
            continue;
        }

        int isDir = S_ISDIR(fileStat.st_mode);
        int isFile = S_ISREG(fileStat.st_mode);

        if (startsWith(search, entry->d_name) == 1){
            if(isDir && !arr[1]) {
                fprintf(outputFile, BLUE BOLD ".%s" RESET, fullPath + len);
                fprintf(outputFile, "\n");
                *isDirectory = true;
                strcpy(match, fullPath);
                *cnt = *cnt + 1;
                *matchFound = true;
            } else if (isFile && !arr[0]) {
                fprintf(outputFile, WHITE BOLD ".%s" RESET ,fullPath + len);   
                fprintf(outputFile, "\n");
                *isDirectory = false;
                strcpy(match, fullPath);
                *cnt = *cnt + 1;
                *matchFound = true;
            }
        }

        if (isDir) {
            printSeek(fullPath, search, arr, matchFound, cnt, len, match, isDirectory);
        }
    }

    closedir(dir);
    return;
}


void seek(char *str) {
    if(str == NULL){
        return;
    }

    char path[MAX_PATH_LENGTH];
    char search[MAX_PATH_LENGTH] = "\0";
    int arr[3] = {0, 0, 0}; 

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
    
    while(token != NULL){
        if(token[0] == '-' && token[1] != ' '){
            for(int i = 1; i<strlen(token); i++) {
                if(token[i] == 'd'){
                    arr[0] = 1;
                } else if(token[i] == 'f'){
                    arr[1] = 1;
                } else if(token[i] == 'e'){
                    arr[2] = 1;
                } else {
                    return;
                }
            }
        } else {
            strcpy(search, token);
            token = strtok_r(NULL, " \t\n", &svPtr);
            if(token != NULL){
                trim_spaces(token);
                while(token != NULL && (token[0] == '<' || token[0] == '>')) {
                    trim_spaces(token);
                    token = strtok_r(NULL, " \t\n", &svPtr);
                    if(token == NULL) {
                        perror("Invalid IO Redirection");
                        return;
                    } 

                    token = strtok_r(NULL, " \t\n", &svPtr);
                }
            }
            
            if(token == NULL) 
                strcpy(path, abslutePath);
            else if (token[0] == '~')
                strcpy(path, cwd);
            else if (token[0] == '-'){
                if(PrevWD[0] != '\0')
                    strcpy(path, PrevWD);
                else {
                    perror("Not Present Previous Dictionary\n");
                    return;
                }
            } 
            else if (token[0] == '.' && token[1] != '.')
                strcpy(path, abslutePath);
            else
                strcpy(path, token);

            token = strtok_r(NULL, " \t\n", &svPtr);
            if(token != NULL){
                trim_spaces(token);
                if(token[0] == '<' || token[0] == '>') {
                    token = strtok_r(NULL, " \t\n", &svPtr);
                    token = strtok_r(NULL, " \t\n", &svPtr);
                } else {
                    return;
                }
            }
        }

        token = strtok_r(NULL, " \t\n", &svPtr);
        if(token != NULL){
            trim_spaces(token);
            if(token[0] == '<' || token[0] == '>') {
                token = strtok_r(NULL, " \t\n", &svPtr);
                token = strtok_r(NULL, " \t\n", &svPtr);
            }
        }
    }
    
    if(search[0] == '\0'){
        perror("No arrguments found\n");
        return;
    }

    if(arr[0] == 1 && arr[1] == 1){
        perror("Invalid flags!\n");
        return;
    }

    int cnt = 0;
    bool matchFound = false;
    char match[MAX_PATH_LENGTH];
    bool isDirectory = true;

    printSeek(path, search, arr, &matchFound, &cnt, strlen(path), match, &isDirectory);
    if(matchFound == false) {
        fprintf(outputFile, MAGENTA BOLD "No match found" RESET);
        fprintf(outputFile, "\n");
        return;
    }

    if(arr[2] == 1 && cnt == 1) {
        if(isDirectory == true) {
            char str[MAX_PATH_LENGTH];
            strcpy(str, "cd ");
            strcat(str, match);
            cd(str);
        } else {
            FILE *ptr = fopen(match, "r");
            if (ptr == NULL) {
                if (errno == EACCES) {
                    perror("Missing permissions for task!");
                } else {
                    perror("Error opening the file to be read");
                }
                return;
            }

            char str[MAX_PATH_LENGTH];
            while (fgets(str, 5000, ptr) != NULL) {
                fprintf(outputFile, "%s", str);
            }
            fprintf(outputFile, "\n");

            fclose(ptr);
        }
    }

    return;
}
