#include "main.h"

#define MAX_DIR_ITEMS 1024

int compare(const void *arg1, const void *arg2);
char file_type(mode_t mode);
int ls_print(char **file);
int ls_l_print(char **file);
bool reveal (char *str, char *cwd, char *PrevWD, char *abs);
void revealHelper (char *dest, bool has_a, bool has_l, char *cwd, char *abs);