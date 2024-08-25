#include "username.h"   

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
    char *cwd;
    cwd = malloc(sizeof(char) * 5001);
    getcwd(cwd, 5001);
    return cwd;
}

void displayRequirments(char *abs, char *cwd, bool insideWorkingDirectory){
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
        strcat(display, abs + len);
    } else {
        strcat(display, abs);
    }
    strcat(display, ">");
    printf("%s", display);

    return;
}