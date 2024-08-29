#include "backGroundProcess.h"

bgList addNewProcessBgList(bgList list, backGroundProcess *process) {
    bgList newList;
    newList = (bgList)malloc(sizeof(backGroundProcessList));
    newList->processDetails = process;
    newList->next = list;
    newList->prev = NULL;
    if(list != NULL) {
        list->prev = newList;
    }
    process->ptr = newList;
    return newList;
}  

cmpList revomeProcessBgList(bgList list, cmpList completeList, backGroundProcess *process) {
    bgList temp = process->ptr;
    bgList prev = temp->prev;
    bgList next = temp->next;
    if(prev != NULL)
        prev->next = next;
    if(next != NULL)
        next->prev = prev;
    if (prev == NULL) 
        list = next;
    
    // cmpList newList;
    // newList = (cmpList)malloc(sizeof(completedBackGroundProcessList));
    // newList->processDetails = process;
    // newList->next = completeList;
    // return newList;

    return completeList;
}

cmpList printProceesCmpList(cmpList CMPList, bgList list) {
    cmpList temp = CMPList;
    while (temp!= NULL) {
        if(temp->processDetails->exit_status == Normal)
            printf("%s exited normally (%d)\n", temp->processDetails->cmdName, temp->processDetails->pid);
        else 
            printf("%s exited abnormally (%d)\n", temp->processDetails->cmdName, temp->processDetails->pid);
        temp = temp->next;
    }

    // return NULL;

    // bgList temp = list;
    // while(temp != NULL){
    //     printf("%p %d\n", temp, temp->processDetails->running_status);
    //     if(temp->processDetails->running_status == Finished){
    //         if(temp->processDetails->exit_status == Normal)
    //             printf("%s exited normally (%d)\n", temp->processDetails->cmdName, temp->processDetails->pid);
    //         else 
    //             printf("%s exited abnormally (%d)\n", temp->processDetails->cmdName, temp->processDetails->pid);   

    //         revomeProcessBgList(list, CMPList, temp->processDetails);
    //     }

    //     temp = temp->next;
    // }

    return NULL;
}