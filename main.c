#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "dir_list.h"

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
        printf("%s  ", dir_arr[i]);
    }
    free_dir_list(list);
    printf("\n");
}

void no_flags(char** argv)
{
    while (argv && *argv) {
        DIR *dir = opendir(*argv);
        if (!dir) {
            printf("%s  ", *argv);
        } else {
            print_unordered_dir(dir);
        }
        argv++;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    int flags = 0;
    int i = 1, j = 0;
    argv[0] = ".";
    if (argc >= 2) {
        while (argv[i]) {
            if (check_flags(argv[i], &flags) == 0) {
                argv[j] = argv[i];
                j++;
            }
            i++;
        }
        while (argv[j]) {
            argv[j] = NULL;
            j++;
        }
    }
    no_flags(argv);
    return 0;
}