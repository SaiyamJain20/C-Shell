#include "../main.h"

char *generateForeGroundToken(char *s, char **svPtr){
    if(s == NULL){
        return NULL;
    }
    char *token;
    token = strtok_r(s, ";", svPtr);

    return token;
}

