#include "reveal.h"

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
        perror("Error: ");
        return 0;
    }

    if(per.st_mode & S_IXUSR) printf("\033[0;32m");
    if (S_ISDIR(per.st_mode)) printf("\033[0;34m"); // Blue color for directory

    printf("%s\n",file[0]);
    printf("\033[0m");
    return 1;
}

int ls_l_print(char** file) {
    struct stat per;
    if(stat(file[1],&per)!=0){
        perror("Error: ");
        return 0;
    }

    printf("%c",file_type(per.st_mode));
    char date[100];
    printf( (per.st_mode & S_IRUSR) ? "r" : "-");
    printf( (per.st_mode & S_IWUSR) ? "w" : "-");
    printf( (per.st_mode & S_IXUSR) ? "x" : "-");
    printf( (per.st_mode & S_IRGRP) ? "r" : "-");
    printf( (per.st_mode & S_IWGRP) ? "w" : "-");
    printf( (per.st_mode & S_IXGRP) ? "x" : "-");
    printf( (per.st_mode & S_IROTH) ? "r" : "-");
    printf( (per.st_mode & S_IWOTH) ? "w" : "-");
    printf( (per.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%5ld ",per.st_nlink);
    printf("%s ", getpwuid(per.st_uid)->pw_name);
    printf("%s ", getgrgid(per.st_gid)->gr_name);
    printf("\t%8ld\t ", per.st_size);
    if (time(0) - per.st_mtime < 15780000)
        strftime(date, 20, "%d %b %H:%M", localtime(&(per.st_mtime)));
    else
        strftime(date, 20, "%d %b  %Y", localtime(&(per.st_mtime)));
    printf("%s ",date);
    if(per.st_mode & S_IXUSR) printf("\033[0;32m");
    if (S_ISDIR(per.st_mode)) printf("\033[0;34m"); // Blue color for directory

    printf("%s\n",file[0]);
    printf("\033[0m");
    return 1;
}

bool reveal (char *str, char *cwd, char *PrevWD, char *abs) {
    if(str == NULL) return false;

    char *token;
    char *svPtr = str;
    bool helperDone = false;
    bool has_a = false, has_l = false;
    token = strtok_r(svPtr, " \t\n", &svPtr);   // 1st command is reveal
    token = strtok_r(NULL, " \t\n", &svPtr);

    if (token == NULL) {
        revealHelper(".", has_a, has_l, cwd, abs); 
        helperDone = true;
    } else {
        while (token != NULL) {
            if(token[0] == '-'){
                if(token[1] == '\0'){
                    if (PrevWD == NULL){
                        printf("Not Present Previous Dictionary");
                        return false;
                    }

                    revealHelper(PrevWD, has_a, has_l, cwd, abs);
                    helperDone = true;
                } else {
                    for(int i = 1; i<strlen(token); i++) {
                        if(token[i] == 'a'){
                            has_a = true;
                        } else if(token[i] == 'l'){
                            has_l = true;
                        } else {
                            printf("Invalid flags");
                            return false;
                        }
                        if(has_a == true && has_l == true){
                            break;
                        }
                    }
                }
            } else if (strcmp(token, "~") == 0) {
                revealHelper(cwd, has_a, has_l, cwd, abs);
                helperDone = true;
            } else if (token[0] == '~') {
                char *temp;
                temp = malloc(5001 * sizeof(char));
                strcpy(temp, cwd);
                strcat(temp, token + 1);
                revealHelper(temp, has_a, has_l, cwd, abs);
                helperDone = true;
            } else {
                revealHelper(token, has_a, has_l, cwd, abs);
                helperDone = true;
            }

            token = strtok_r(NULL, " \t\n", &svPtr);
        }

        if(helperDone == false){
            revealHelper(".", has_a, has_l, cwd, abs);
            helperDone = true;
        }
    }

    return true;
}

void revealHelper (char *dest, bool has_a, bool has_l, char *cwd, char *abs) {
    if (strcmp(dest, "~") == 0){
        chdir(cwd);
    } else {
        chdir(dest);
    }
    DIR *dir = opendir(".");
    chdir(abs);

    struct dirent* Dirent;
    char **arr[1024];
    for(int i = 0; i<1024; i++) {
        arr[i] = malloc(2 * sizeof(char *));
    }
    int i = 0;

    while((Dirent = readdir(dir)) != NULL) {
        if(has_a || Dirent->d_name[0] != '.'){
            arr[i][0] = malloc(100 * sizeof(char));
            arr[i][1] = malloc(100 * sizeof(char));
            strcpy(arr[i][0], Dirent->d_name);
            strcpy(arr[i][1], dest);
            strcat(arr[i][1], "/");
            strcat(arr[i][1], Dirent->d_name);
            i++;
        }
    }

    qsort(arr, i, sizeof(char *), compare);

    if(has_l) {
        long long int cnt = 0;

        for (int j = 0; j < i; ++j){
            struct stat per;
            if(stat(arr[j][1],&per)!=0){
                perror("Error: ");
                return;
            }

            cnt += per.st_blocks;
        }

        cnt /= 2;

        printf("total %lld\n", cnt);

        for (int j = 0; j < i; ++j){
            ls_l_print(arr[j]);
        }
    } else {
        for (int j = 0; j < i; ++j){
            ls_print(arr[j]);
        }
    }

    closedir(dir);
    return;
}