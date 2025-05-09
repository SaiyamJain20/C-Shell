#include "../main.h"   

char *getSystemName(){
    char *sysName;
    struct utsname Uname;
    int x = uname(&Uname);

    if(x != -1){
        int l = strlen(Uname.nodename) + 10;
        sysName = malloc(MAX_DISPLAY_LENGTH);
        if(sysName == NULL) {
            perror("malloc");
            exit(0);
        }
        strcpy(sysName, Uname.nodename);
        return sysName;
    } else {
        return NULL;
    }
}

char *getHostName(){
    char *hostbuffer;
    hostbuffer = malloc(MAX_DISPLAY_LENGTH);
    if(hostbuffer == NULL) {
        perror("malloc");
        exit(0);
    }
    hostbuffer = getlogin();
    return hostbuffer;
}

char *currentWorkingDirectory(){
    char *New;
    New = malloc(sizeof(char) * MAX_DISPLAY_LENGTH);
    if(New == NULL) {
        perror("malloc");
        exit(0);
    }
    getcwd(New, MAX_PATH_LENGTH);
    return New;
}

void displayRequirments(){
    fflush(stdout);
    char *sysName = getSystemName();
    char *hostName = getHostName();
    char *display;
    display = malloc(sizeof(char) * MAX_DISPLAY_LENGTH);
    if(display == NULL) {
        perror("malloc");
        return;
    }
    memset(display, 0, sizeof(char) * MAX_DISPLAY_LENGTH);

    fprintf(outputFile, WHITE "<" GREEN BOLD"%s@%s" RESET WHITE ":", hostName, sysName);

    /*
        Everything will be added here
    */

    if(insideWorkingDirectory == true){
        strcat(display, "~");
        int len  = strlen(cwd);
        strcat(display, abslutePath + len);
    } else {
        strcat(display, abslutePath);
    }
    
    fprintf(outputFile, BLUE BOLD "%s", display);

    if(extraTime[0] != '\0'){
        fprintf(outputFile, MAGENTA BOLD " %s", extraTime);
    }

    fprintf(outputFile, RESET WHITE "> ");

    free(display);

    return;
}