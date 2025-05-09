#include "../main.h"

int compare(const void *arg1, const void *arg2) {
    char **arr1 = *(char ***)arg1;
    char **arr2 = *(char ***)arg2;
    return strcmp(arr1[0], arr2[0]);
}

char file_type(mode_t mode) {
    char c;

    if (S_ISREG(mode))          c = '-';
    else if (S_ISDIR(mode))     c = 'd';
    else if (S_ISCHR(mode))     c = 'c';
    else if (S_ISBLK(mode))     c = 'b';
    else if (S_ISFIFO(mode))    c = 'p';
    else if (S_ISLNK(mode))     c = 'l';
    else if (S_ISSOCK(mode))    c = 's';
    else                        c = '?';

    return c;
}

int ls_print(char** file) {
    struct stat per;
    if(stat(file[1],&per)!=0){
        perror("Stat erreo\n");
        return 0;
    }

    if(per.st_mode & S_IXUSR) fprintf(outputFile, GREEN);
    if (S_ISDIR(per.st_mode)) fprintf(outputFile, BLUE);

    fprintf(outputFile, "%s" WHITE,file[0]);
    fprintf(outputFile, "\n");
    return 1;
}

int ls_l_print(char** file) {
    struct stat per;
    if(stat(file[1],&per)!=0){
        perror("Stat erreo\n");
        return 0;
    }

    fprintf(outputFile, "%c",file_type(per.st_mode));
    fprintf(outputFile,  (per.st_mode & S_IRUSR) ? "r" : "-");
    fprintf(outputFile,  (per.st_mode & S_IWUSR) ? "w" : "-");
    fprintf(outputFile,  (per.st_mode & S_IXUSR) ? "x" : "-");
    fprintf(outputFile,  (per.st_mode & S_IRGRP) ? "r" : "-");
    fprintf(outputFile,  (per.st_mode & S_IWGRP) ? "w" : "-");
    fprintf(outputFile,  (per.st_mode & S_IXGRP) ? "x" : "-");
    fprintf(outputFile,  (per.st_mode & S_IROTH) ? "r" : "-");
    fprintf(outputFile,  (per.st_mode & S_IWOTH) ? "w" : "-");
    fprintf(outputFile,  (per.st_mode & S_IXOTH) ? "x" : "-");
    fprintf(outputFile, " ");
    fprintf(outputFile, "%5ld ",per.st_nlink);
    fprintf(outputFile, "%s ", getpwuid(per.st_uid)->pw_name);
    fprintf(outputFile, "%s ", getgrgid(per.st_gid)->gr_name);
    fprintf(outputFile, "\t%8ld\t ", per.st_size);
    char date[100];
    if (time(0) - per.st_mtime < 15780000)
        strftime(date, 20, "%b %d %H:%M", localtime(&(per.st_mtime)));
    else
        strftime(date, 20, "%b %d  %Y", localtime(&(per.st_mtime)));
    fprintf(outputFile, "%s ",date);
    
    if(per.st_mode & S_IXUSR) fprintf(outputFile, GREEN);
    if (S_ISDIR(per.st_mode)) fprintf(outputFile, BLUE);

    fprintf(outputFile, "%s" WHITE,file[0]);
    fprintf(outputFile, "\n");
    return 1;
}

void reveal (char *str) {
    if(str == NULL) return;

    char *token;
    char *svPtr = str;
    bool helperDone = false;
    bool has_a = false, has_l = false;
    token = strtok_r(svPtr, " \t\n", &svPtr);   // 1st command is reveal

    while(token != NULL && (token[0] == '<' || token[0] == '>')) {
        trim_spaces(token);
        token = strtok_r(NULL, " \t\n", &svPtr);
        if(token == NULL) {
            perror("Invalid IO Redirection");
            return;
        } 

        token = strtok_r(NULL, " \t\n", &svPtr);
    }

    token = strtok_r(svPtr, " \t\n", &svPtr); 

    if (token == NULL) {
        revealHelper(".", has_a, has_l); 
        helperDone = true;
    } else {
        while (helperDone != true && token != NULL) {
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
            if (token == NULL) {
                revealHelper(".", has_a, has_l); 
                helperDone = true;
            } else if(token[0] == '-'){
                if(token[1] == '\0'){
                    if (PrevWD[0] == '\0'){
                        perror("Not Present Previous Dictionary\n");
                        return;
                    }

                    revealHelper(PrevWD, has_a, has_l);
                    helperDone = true;
                } else {
                    for(int i = 1; i<strlen(token); i++) {
                        if(token[i] == 'a'){
                            has_a = true;
                        } else if(token[i] == 'l'){
                            has_l = true;
                        } else {
                            perror("Invalid flags\n");
                            return;
                        }
                    }
                }
            } else if (strcmp(token, "~") == 0) {
                revealHelper(cwd, has_a, has_l);
                helperDone = true;
            } else if (token[0] == '~') {
                char *temp;
                temp = malloc(MAX_PATH_LENGTH * sizeof(char));
                strcpy(temp, cwd);
                strcat(temp, token + 1);
                revealHelper(temp, has_a, has_l);
                helperDone = true;
                free(temp);
            } else {
                revealHelper(token, has_a, has_l);
                helperDone = true;
            }

            token = strtok_r(NULL, " \t\n", &svPtr);
        }

        if(helperDone == false){
            revealHelper(".", has_a, has_l);
            helperDone = true;
        }
    }

    return;
}

void revealHelper (char *dest, bool has_a, bool has_l) {
    if (strcmp(dest, "~") == 0){
        chdir(cwd);
    } else {
        chdir(dest);
    }
    DIR *dir = opendir(".");
    chdir(abslutePath);

    struct dirent* Dirent;
    char **arr[MAX_FILES];
    for(int i = 0; i<MAX_FILES; i++) {
        arr[i] = malloc(2 * sizeof(char *));
    }
    int i = 0, j = 0;

    while((Dirent = readdir(dir)) != NULL) {
        if(has_a || Dirent->d_name[0] != '.'){
            arr[i][0] = malloc(MAX_PATH_LENGTH);
            arr[i][1] = malloc(MAX_PATH_LENGTH);
            strcpy(arr[i][0], Dirent->d_name);
            strcpy(arr[i][1], dest);
            strcat(arr[i][1], "/");
            strcat(arr[i][1], Dirent->d_name);
            i++;
            j++;
        }
    }

    qsort(arr, i, sizeof(char *), compare);

    if(has_l) {
        long long int cnt = 0;

        for (int j = 0; j < i; ++j){
            struct stat per;
            if(stat(arr[j][1],&per)!=0){
                perror("Stat erreo\n");
                return;
            }

            cnt += per.st_blocks;
        }

        cnt /= 2;

        fprintf(outputFile, BLUE BOLD "total %lld" RESET, cnt);
        fprintf(outputFile, "\n");

        for (int j = 0; j < i; ++j){
            ls_l_print(arr[j]);
        }
    } else {
        for (int j = 0; j < i; ++j){
            ls_print(arr[j]);
        }
    }

    closedir(dir);
    for(int i = 0; i<MAX_FILES; i++) {
        if(i < j){
            free(arr[i][0]);
            free(arr[i][1]);
        }
        free(arr[i]);
    }
    return;
}