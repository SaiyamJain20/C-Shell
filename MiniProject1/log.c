#include "log.h"

void removeNewline(char *str) {
    char *newlinePos = strchr(str, '\n');
    if (newlinePos != NULL) {
        *newlinePos = '\0';
    }
}

int convertToNumber(char *str){
    int x = 0;
    int i = 0;
    char c;
    c = str[i];
    i++;

    while(c != '\0'){
        c -= '0';
        x *= 10;
        x += c;
        c = str[i];
        i++;
    }

    return x;
}

bool saveToLog(char *str, int logFile) {
    char History[MAX_HISTORY_SIZE + 1][5001];
    char OldHistory[(MAX_HISTORY_SIZE + 1) * 5001];
    int i = 0;
    char buffer[5001];
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
            return true;
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

    return true;
}

bool printHistory(int logFile) {
    char History[MAX_HISTORY_SIZE + 1][5001];
    char OldHistory[(MAX_HISTORY_SIZE + 1) * 5001];
    int i = 0;
    char buffer[5001];
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
    
    for (int j = 0 ? i <= 15 : i - 15; j < i; j++) {
        printf("%s\n", History[j]);
    }

    return true;
}

bool purgeHistory(int logFile) {
    ftruncate(logFile, 0);
    lseek(logFile, 0, SEEK_SET);
    return true;
}

bool executeHistory(int commandNumber, int logFile, char *abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, bool *exitProgram, bgList BGList, cmpList CMPList) {
    char History[MAX_HISTORY_SIZE + 1][5001];
    char OldHistory[(MAX_HISTORY_SIZE + 1) * 5001];
    int i = 0;
    char buffer[5001];
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

    saveToLog(History[i - commandNumber], logFile);
    
    process(History[i - commandNumber], &abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory, logFile, exitProgram, &BGList, CMPList);
    return true;
}

bool handleLog(char *str, int logFile, char * abs, char *cwd, char *PrevWD, bool *insideWorkingDirectory, bool *prevInsideWorkingDirectory, bool *exitProgram, bgList BGList, cmpList CMPList) {
    if(str == NULL) return false;

    char *cmd;
    cmd = (char *)malloc(sizeof(char) * (strlen(str) + 10));
    strcpy(cmd, str);
    char *svPtr = cmd;
    char *token = strtok_r(svPtr, " \t\n", &svPtr);     //1st command is log
    token = strtok_r(NULL, " \t\n", &svPtr);

    if(token == NULL){
        printHistory(logFile);
    } else if(strcmp(token, "purge") == 0){
        purgeHistory(logFile);
    } else if(strcmp(token, "execute") == 0){
        token = strtok_r(NULL, " \t\n", &svPtr);    // gets the number
        int commandNumber = convertToNumber(token);

        executeHistory(commandNumber, logFile, abs, cwd, PrevWD, insideWorkingDirectory, prevInsideWorkingDirectory, exitProgram, BGList, CMPList);
    } else {
        return false;
    }

    return true;
}