#include "main.h"   

char *getSystemName(){
    char *sysName;
    struct utsname Uname;
    int x = uname(&Uname);

    if(x != -1){
        int l = strlen(Uname.nodename) + 10;
        sysName = malloc(sizeof(char) * l);

        strcpy(sysName, Uname.nodename);
        return sysName;
    } else {
        return NULL;
    }
}

char *getHostName(){
    char *hostbuffer;
    hostbuffer = malloc(sizeof(char) * 257);
    
    hostbuffer = getlogin();
    return hostbuffer;
}

char *currentWorkingDirectory(){
    char *New;
    New = malloc(sizeof(char) * 5001);
    getcwd(New, 5001);
    return New;
}

void displayRequirments(){
    char *sysName = getSystemName();
    char *hostName = getHostName();
    char *display;
    display = malloc(sizeof(char) * 5001);
    memset(display, 0, sizeof(char) * 5001);

    display[0] = '<';
    strcat(display, hostName);  
    strcat(display, "@");
    strcat(display, sysName);
    strcat(display, ":");

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
    strcat(display, ">");
    printf("%s", display);

    return;
}