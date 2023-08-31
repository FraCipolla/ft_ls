#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "dir_list.h"

enum flags {
    l = 1 << 0,
    R = 1 << 1,
    a = 1 << 2,
    r = 1 << 3,
    t = 1 << 4,
    u = 1 << 5,
    f = 1 << 6,
    g = 1 << 7,
    d = 1 << 8,
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
}

void print_unordered_dir(DIR *dir)
{
    t_dir_list *list = f_init(dir);
    // print_dir_list(list);
    print_rev_dir_list(list);
    free_dir_list(list);
    printf("\n");
}

void no_flags(char** argv)
{
    while (++argv && *argv) {
        if (*argv[0] != '-') {
            DIR *dir = opendir(*argv);
            if (!dir) {
                printf("ls: cannot access '%s': %s\n", *argv, strerror(errno));
            } else {
                print_unordered_dir(dir);
            }
        }
        return;
    } 
    DIR *dir = opendir(".");
    print_unordered_dir(dir);
}

int main(int argc, char *argv[]) {

    int flags = 0;
    char **tmp = argv;
    if (argc >= 2) {
        while (argv[1]) {
            check_flags(argv[1], &flags);
            argv++;
        }
        printf("flags: %d\n", flags);
    }
    no_flags(tmp);

    return 0;
}