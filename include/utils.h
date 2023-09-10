#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


void print_permission(char* file);

void print_array(char** arr)
{
    int i = 0;
    while (arr[i] != NULL)
    {
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
}

#endif