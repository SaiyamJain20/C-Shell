#include "../main.h"

void removeNewline(char *str) {
    char *newlinePos = strchr(str, '\n');
    if (newlinePos != NULL) {
        *newlinePos = '\0';
    }
}

void saveToLog(char *str) {
    char History[MAX_HISTORY_SIZE + 1][MAX_PATH_LENGTH];;
    char OldHistory[(MAX_HISTORY_SIZE + 1) * MAX_PATH_LENGTH];
    memset(OldHistory, 0, sizeof(OldHistory));

    int i = 0;
    char buffer[MAX_PATH_LENGTH];
    ssize_t bytesRead;
    off_t offset = 0;

    lseek(logFile, 0, SEEK_SET);

    while ((bytesRead = read(logFile, buffer, 5000)) > 0 && i < MAX_HISTORY_SIZE) {
        buffer[bytesRead] = '\0';
        strcpy(OldHistory, buffer);
    }

    char *historyElement = OldHistory;
    char *token;
    token = strtok_r(historyElement,"\n", &historyElement);

    while(token != NULL){
        strcpy(History[i], token);
        i++;
        token = strtok_r(NULL, "\n", &historyElement);
    }

    if (i > 0) {
        removeNewline(str);

        if(strcmp(History[i - 1], str) == 0)
            return;
    }
    
    if (i < MAX_HISTORY_SIZE) {
        strcpy(History[i], str);
        i++;
    } else {
        strcpy(History[i], str);
        i++;
    }

    ftruncate(logFile, 0);
    lseek(logFile, 0, SEEK_SET);
    
    for (int j = 0 ? i <= 15 : i - 15; j < i; j++) {
        if(History[j][0] != '\0') {
            write(logFile, History[j], strlen(History[j]));
            write(logFile, "\n", 1);
        }
    }

    return;
}

void printHistory() {
    char History[MAX_HISTORY_SIZE + 1][MAX_PATH_LENGTH];
    char OldHistory[(MAX_HISTORY_SIZE + 1) * MAX_PATH_LENGTH];
    memset(OldHistory, 0, sizeof(OldHistory));

    int i = 0;
    char buffer[MAX_PATH_LENGTH];
    ssize_t bytesRead;
    off_t offset = 0;
    char *historyElement = OldHistory;
    char *token;

    lseek(logFile, 0, SEEK_SET);

    while ((bytesRead = read(logFile, buffer, MAX_PATH_LENGTH - 1)) > 0 && i < MAX_HISTORY_SIZE) {
        buffer[bytesRead] = '\0';
        strcpy(OldHistory, buffer);
        token = strtok_r(historyElement,"\n", &historyElement);

        while(token != NULL){
            strcpy(History[i], token);
            i++;
            token = strtok_r(NULL, "\n", &historyElement);
        }
    }

    lseek(logFile, 0, SEEK_SET);

    bool printed = false;
    
    for (int j = 0 ? i <= 15 : i - 15; j < i; j++) {
        if(History[j][0] != '\0'){
            fprintf(outputFile, CYAN BOLD "%s" RESET, History[j]);
            fprintf(outputFile, "\n");
            printed = true;
        }
    }

    if(!printed){
        fprintf(outputFile, RED BOLD "No History to Display" RESET);
        fprintf(outputFile, "\n");
    }

    return;
}

void purgeHistory() {
    ftruncate(logFile, 0);
    lseek(logFile, 0, SEEK_SET);
    fprintf(outputFile, MAGENTA BOLD "Purge Successful" RESET);
    fprintf(outputFile, "\n");
    return;
}

void executeHistory(int commandNumber) {
    char History[MAX_HISTORY_SIZE + 1][MAX_PATH_LENGTH];
    char OldHistory[(MAX_HISTORY_SIZE + 1) * MAX_PATH_LENGTH];
    memset(OldHistory, 0, sizeof(OldHistory));

    int i = 0;
    char buffer[MAX_PATH_LENGTH];
    ssize_t bytesRead;
    off_t offset = 0;

    lseek(logFile, 0, SEEK_SET);

    while ((bytesRead = read(logFile, buffer, 5000)) > 0 && i < MAX_HISTORY_SIZE) {
        buffer[bytesRead] = '\0';
        strcpy(OldHistory, buffer);
    }

    char *historyElement = OldHistory;
    char *token;
    token = strtok_r(historyElement,"\n", &historyElement);

    while(token != NULL){
        strcpy(History[i], token);
        i++;
        token = strtok_r(NULL, "\n", &historyElement);
    }

    lseek(logFile, 0, SEEK_SET);

    if(i < commandNumber) {
        fprintf(outputFile, RED BOLD "Command Number Out of Range" RESET);
        fprintf(outputFile, "\n");
        return;
    }

    if(commandNumber != 1)
        saveToLog(History[i - commandNumber]);
    
    process(History[i - commandNumber]);
    return;
}

void handleLog(char *str) {
    if(str == NULL) return;

    char *cmd;
    cmd = (char *)malloc(sizeof(char) * (strlen(str) + 10));
    strcpy(cmd, str);
    char *svPtr = cmd;
    char *token = strtok_r(svPtr, " \t\n", &svPtr); 
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

    token = strtok_r(NULL, " \t\n", &svPtr);

    if(token == NULL){
        printHistory(logFile);
    } else if(strcmp(token, "purge") == 0){
        purgeHistory(logFile);
    } else if(strcmp(token, "execute") == 0){
        token = strtok_r(NULL, " \t\n", &svPtr);    // gets the number
        int commandNumber = atoi(token);
        executeHistory(commandNumber);
    } else {
        return;
    }

    return;
}

bool containsLog(char *in) {
    char *temp = malloc(strlen(in) + 10);
    strcpy(temp, in);

    char *svPtr;
    svPtr = strtok_r(temp, ";&|\n", &temp);

    while(svPtr != NULL) {
        char *temp2 = malloc(MAX_INPUT_LENGTH);
        strcpy(temp2, svPtr);

        findInAliasList(temp2);
        if(strstr(in, "log") != NULL) {
            return false;
        }

        svPtr = strtok_r(NULL, ";&|\n", &temp);
    }

    return true;
}