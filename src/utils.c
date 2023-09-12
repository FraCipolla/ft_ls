#include "../include/utils.h"

void print_array(char** arr)
{
    int i = 0;
    while (arr[i] != NULL)
    {
        if(arr[i][0] != '.')
            printf("%s  ", arr[i]);
        i++;
    }
    printf("\n");
};

void print_l(char** arr)
{
    int i = 0;
    while (arr[i] != NULL)
    {
        if(arr[i][0] != '.')
            print_permission(arr[i]);
        printf("%s\n", arr[i]);
        i++;
    }
    printf("\n");
};

void print_permission(char* file)
{
    struct stat file_stat;
    int ret = stat(file, &file_stat);
    if (ret == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    S_ISDIR(file_stat.st_mode) ? printf("d") : printf("-");
    file_stat.st_mode & S_IRUSR ? printf("r") : printf("-");
    file_stat.st_mode & S_IWUSR ? printf("w") : printf("-");
    file_stat.st_mode & S_IXUSR ? printf("x") : printf("-");
    file_stat.st_mode & S_IRGRP ? printf("r") : printf("-");
    file_stat.st_mode & S_IWGRP ? printf("w") : printf("-");
    file_stat.st_mode & S_IXGRP ? printf("x") : printf("-");
    file_stat.st_mode & S_IROTH ? printf("r") : printf("-");
    file_stat.st_mode & S_IWOTH ? printf("w") : printf("-");
    file_stat.st_mode & S_IXOTH ? printf("x") : printf("-");
    printf(" %ld ", file_stat.st_nlink);
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group  *gr = getgrgid(file_stat.st_gid);
    printf("%s %s ", pw->pw_name, gr->gr_name);
}