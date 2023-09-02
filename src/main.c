#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../include/dir_list.h"

enum flags {
    l = 1 << 0, // 1
    R = 1 << 1, // 2
    a = 1 << 2, // 4
    r = 1 << 3, // 8
    t = 1 << 4, // 16
    u = 1 << 5, // 32
    f = 1 << 6, // 64
    g = 1 << 7, // 128
    d = 1 << 8, // 256
};

int check_flags(char *s, int *flags)
{
    if (*s == '-') {
        s++;
    } else {
        return 0;
    }

    while (*s) {
        switch (*s) {
            case 'l':
                *flags |= l;
                break;
            case 'R':
                *flags |= R;
                break;
            case 'a':
                *flags |= a;
                break;
            case 'r':
                *flags |= r;
                break;
            case 't':
                *flags |= t;
                break;
            case 'u':
                *flags |= u;
                break;
            case 'f':
                *flags |= f;
                break;
            case 'g':
                *flags |= g;
                break;
            case 'd':
                *flags |= d;
                break;
            default:
                return 0;
        }
        s++;
    }
    return 1;
}

void print_unordered_dir(DIR *dir)
{
    t_dir_list *list = dir_init(dir);
    char** dir_arr = sort_dir_list(list);
    for (unsigned int i = 0; i < list->size; i++) {
        if (dir_arr[i][0] != '.')
            printf("%s  ", dir_arr[i]);
    }
    free_dir_list(list);
}

void no_flags_no_path(char* argv)
{
    DIR *dir = opendir(argv);
    if (!dir) {
        printf("%s  ", argv);
    } else {
        print_unordered_dir(dir);
    }
    printf("\n");
}

void no_flags_path(char** argv)
{
    if (*argv == NULL) {
        return;
    }
    no_flags_path(argv + 1);
    printf("%s:\n", *argv);
    DIR *dir = opendir(*argv);
    if (!dir) {
        printf("%s  ", *argv);
    } else {
        print_unordered_dir(dir);
    }
    printf("\n");
}

void choose_path(char** argv, int flags)
{
    int i = (!(flags == 0) + (!(*argv == NULL) * 2));
    /* case: 1 flags no path
       case: 2 no flags with paths
       case: 3 flags with paths */
    switch (i) {
        case 2:
            no_flags_path(argv);
            break;
        default:
            no_flags_no_path(".");
            break;
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        no_flags_no_path(".");
        return 0;
    }
    int flags = 0;
    int i = 1;
    while (argv[i]) {
        check_flags(argv[i], &flags);
        i++;
    }
    while (--i > 0) {
        if (!check_flags(argv[i], &flags)) {
            if (i != argc - 1)
                printf("\n");
            printf("%s:\n", argv[i]);
            no_flags_no_path(argv[i]);

        }
    }
    // choose_path(argv + 1, flags);
    return 0;
}