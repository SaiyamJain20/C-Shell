#include "../main.h"

void readFile() {
    FILE *file = fopen(".myshrc", "r");
    if(file == NULL) {
        perror("Couldn't open .myshrc");
        return;
    }

    char line[2 * MAX_INPUT_LENGTH + 10];
    bool functionStart = false;

    while (fgets(line, sizeof(line), file) != NULL) {
        if((line[0] == '/' && line[1] == '/') || line[0] == '#') {
            continue;
        }
        
        removeComments(line);

        if(strchr(line, '=') != NULL) {
            addAliasToList(line);
        } else if (functionStart || strstr(line, "()") != NULL) {
            trim_spaces(line);
            line[strlen(line) - 2] = '\0';

            funcList *new;
            new = malloc(sizeof(funcList));
            if(new == NULL) {
                perror("malloc");
                return;
            }

            if(strncmp(line, "func  ", 5) == 0) {
                strcpy(new->funcName, line + 5);
            } else {
                strcpy(new->funcName, line);
            }

            new->arguments = 0;
            new->numberOfFunctions = 0;

            while (fgets(line, sizeof(line), file) != NULL){
                if(strchr(line, '}') != NULL) {
                    functionStart = false;
                    break;
                }

                if(strchr(line, '{') == NULL) {
                    removeComments(line);

                    strcpy(new->funcBody[new->numberOfFunctions], line);
                    new->numberOfFunctions++;
                    int j = 0;
                    while(line[j] != '\0') {
                        if(line[j] - '0' >= 0 && line[j] - '0' <= 9) {
                            if(new->arguments - '0' < line[j] - '0') {
                                new->arguments = line[j] - '0';
                            }
                        }
                        j++;
                    }
                }
            }

            new->next = funclist;
            funclist = new;
        }
    }

    fclose(file);
    return;
}

void removeComments(char *line) {
    char *comment_pos = NULL;
    char *double_slash_pos = strstr(line, "//");
    char *hash_pos = strstr(line, "#");

    if (double_slash_pos != NULL && hash_pos != NULL) {
        comment_pos = (double_slash_pos < hash_pos) ? double_slash_pos : hash_pos;
    } else if (double_slash_pos != NULL) {
        comment_pos = double_slash_pos;
    } else if (hash_pos != NULL) {
        comment_pos = hash_pos;
    }

    if (comment_pos != NULL) {
        *(comment_pos) = '\0'; 
    }

    trim_spaces(line);

    return;
}

void addAliasToList(char *line) {
    char *temp = malloc(MAX_INPUT_LENGTH);
    if(temp == NULL) {
        perror("malloc");
        return;
    }
    strcpy(temp, line);

    char *svPtr;
    svPtr = strtok_r(temp, "=\n", &temp);
    char cmd[2][MAX_INPUT_LENGTH];
    int i = 0;

    while(svPtr != NULL) {
        if(i > 2) break;

        trim_spaces(svPtr);
        if(strncmp(svPtr, "alias ", 6) == 0) {
            strcpy(cmd[i], svPtr + 6);
        } else {
            strcpy(cmd[i], svPtr);
        }

        i++;
        svPtr = strtok_r(NULL, "=\n", &temp);
    }

    if (i < 2) {
        perror("Not enough parts in alias line");
        return;
    }
    if(i > 2) {
        perror("Extra parts in alias line");
        return;
    }

    aliasList *new;
    new = malloc(sizeof(aliasList));
    if(new == NULL) {
        perror("malloc");
        return;
    }
    strcpy(new->aliasCmd, cmd[0]);
    strcpy(new->Cmd, cmd[1]);

    new->next = aliaslist;
    aliaslist = new;
    return;
}

void findInAliasList(char *in) {
    trim_spaces(in);

    aliasList *current = aliaslist;
    while(current != NULL) {
        if(strlen(in) == strlen(current->aliasCmd) && strncmp(current->aliasCmd, in, strlen(in)) == 0) {
            strcpy(in, current->Cmd);
            return;
        }

        current = current->next;
    }

    return;
}

bool findInFuncList(char *in) {
    char *tempIn = malloc(MAX_INPUT_LENGTH);
    if(tempIn == NULL) {
        perror("malloc");
        return false;
    }
    strcpy(tempIn, in);
    char *svPtrIn;
    svPtrIn = strtok_r(tempIn, " \t\n", &tempIn);
    trim_spaces(svPtrIn);

    funcList *current = funclist;
    while(current != NULL) {
        if(strlen(svPtrIn) == strlen(current->funcName) && strncmp(current->funcName, svPtrIn, strlen(svPtrIn)) == 0) {
            char *temp = malloc(MAX_INPUT_LENGTH);
            if(temp == NULL) {
                perror("malloc");
                return false;
            }
            strcpy(temp, in);

            int numOfArgs = 0;
            char *svPtr;
            svPtr = strtok_r(temp, " \t\n", &temp);
            svPtr = strtok_r(NULL, " \t\n", &temp);

            while(svPtr != NULL) {
                numOfArgs++;
                svPtr = strtok_r(NULL, " \t\n", &temp);
            }

            char args[numOfArgs][MAX_INPUT_LENGTH];

            if(numOfArgs == current->arguments) {
                char *temp2 = malloc(MAX_INPUT_LENGTH);
                if(temp2 == NULL) {
                    perror("malloc");
                    return false;
                }
                strcpy(temp2, in);

                char *svPtr2;
                svPtr2 = strtok_r(temp2, " \t\n", &temp2);
                svPtr2 = strtok_r(NULL, " \t\n", &temp2);
                int j = 0;

                while(svPtr2 != NULL) {
                    strcpy(args[j], svPtr2);
                    j++;
                    svPtr2 = strtok_r(NULL, " \t\n", &temp2);
                }

                for(int i = 0; i<current->numberOfFunctions; i++) {
                    char *temp = malloc(MAX_INPUT_LENGTH);
                    strcpy(temp, current->funcBody[i]);
                    replace_placeholders(temp, numOfArgs, args);
                    process(temp);
                }

                return true;
            }
        }

        current = current->next;
    }

    return false;
}

void replace_placeholders(char *str, int args_count, char args[args_count][MAX_INPUT_LENGTH]) {
    char result[MAX_INPUT_LENGTH] = ""; 
    char temp[10];        
    char *pos = str;  

    while (*pos) {
        if (*pos == '\"' && *(pos + 1) == '$' && isdigit(*(pos + 2)) && *(pos + 3) == '\"') {
            int num = *(pos + 2) - '1'; 

            if (num >= 0 && num < args_count) {
                strcat(result, args[num]);
            }

            pos += 4; 
        } else {
            strncat(result, pos, 1);
            pos++;
        }
    }

    strcpy(str, result);
    return;
}