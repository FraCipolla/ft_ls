#include "../include/dir_list.h"
#include "../include/utils.h"

void print_permission(mode_t stat)
{
    S_ISDIR(stat) ? printf("d") : printf("-");
    stat & S_IRUSR ? printf("r") : printf("-");
    stat & S_IWUSR ? printf("w") : printf("-");
    stat & S_IXUSR ? printf("x") : printf("-");
    stat & S_IRGRP ? printf("r") : printf("-");
    stat & S_IWGRP ? printf("w") : printf("-");
    stat & S_IXGRP ? printf("x") : printf("-");
    stat & S_IROTH ? printf("r") : printf("-");
    stat & S_IWOTH ? printf("w") : printf("-");
    stat & S_IXOTH ? printf("x") : printf("-");
    // printf(" %ld ", file_stat.st_nlink);
    // struct passwd *pw = getpwuid(file_stat.st_uid);
    // struct group  *gr = getgrgid(file_stat.st_gid);
    // printf("%s %s ", pw->pw_name, gr->gr_name);
}

