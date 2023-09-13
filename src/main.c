#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "../include/dir_list.h"
#include "../include/utils.h"

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
                *flags &= ~f;
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
                *flags |= a;
                *flags &= ~l;
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

void check_args(char **argv, int flags)
{
    if (strcmp(*argv, ".") == 0) {
        DIR *dir = opendir(*argv);
        t_sized_list *list = dir_init(dir, flags);
        if (flags & t) {
            sort_by_time(list);
        } else {
            sort_by_name(list);
        }
        if (flags & r) {
            if (flags & l) {
                print_rev_dir_list_l(list);
            } else {
                print_rev_dir_list(list);
            }
        } else if (flags & l) {
            print_dir_list_l(list);
        } else {
            print_dir_list(list);
        }
        free_sized_list(list);
    }
}

int main(int argc, char *argv[])
{
    int flags = 0;
    int i = 0, j = 0;
    while (argv[++i]) {
        if (!check_flags(argv[i], &flags)) {
            argv[j++] = argv[i];
        }
    }
    if (j == 0)
        argv[j++] = ".";
    while (j < argc)
        argv[j++] = NULL;
    check_args(argv, flags);
    return 0;
}