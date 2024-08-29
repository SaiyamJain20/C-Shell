#include "main.h"

bgList addNewProcessBgList(backGroundProcess *process) {
    bgList newList;
    newList = (bgList)malloc(sizeof(backGroundProcessList));
    newList->processDetails = process;
    newList->next = BGList;
    newList->prev = NULL;
    if(BGList != NULL) {
        BGList->prev = newList;
    }
    process->ptr = newList;
    return newList;
}  

cmpList revomeProcessBgList(backGroundProcess *process) {
    bgList temp = process->ptr;
    bgList prev = temp->prev;
    bgList next = temp->next;
    if(prev != NULL)
        prev->next = next;
    if(next != NULL)
        next->prev = prev;
    if (BGList == NULL) 
        BGList = next;
    
    cmpList newList;
    newList = (cmpList)malloc(sizeof(completedBackGroundProcessList));
    newList->processDetails = process;
    newList->next = CMPList;
    return newList;

    // return CMPList;
}

cmpList printProceesCmpList() {
    cmpList temp = CMPList;
    while (temp!= NULL) {
        if(temp->processDetails->exit_status == Normal)
            printf("%s exited normally (%d)\n", temp->processDetails->cmdName, temp->processDetails->pid);
        else 
            printf("%s exited abnormally (%d)\n", temp->processDetails->cmdName, temp->processDetails->pid);
        temp = temp->next;
    }

    return NULL;
}