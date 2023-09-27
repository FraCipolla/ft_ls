#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include "../include/dir_list.h"
#include "../include/utils.h"

void check_args(char **argv, int flags, int argc);

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

void print(t_sized_list **sized_list, int flags)
{
    if (flags & t) {
        sort_by_time(&(*sized_list));
    } else if (!(flags & f)) {
        sort_by_name(&(*sized_list));
    }
    if (flags & r) {
        if (flags & l) {
            print_rev_dir_list_l(&(*sized_list));
        } else {
            print_rev_dir_list(&(*sized_list), flags);
        }
    } else if (flags & l) {
        print_dir_list_l(&(*sized_list));
    } else {
        print_dir_list(&(*sized_list), flags);
    }
}

void open_dir(char *path, int flags)
{
    DIR *dir = NULL;
    dir = opendir(path);
    if (!dir) {
        perror("ls");
        exit(errno);
    }
    t_sized_list *sized_list = dir_init(dir, flags, path);
    print(&sized_list, flags);
    if (flags & R) {
        t_dir_list *list = sized_list->head;
        while (list) {
            if (!strcmp(list->path, "..") || !strcmp(list->path, ".")) {
                list = list->next;
                continue;
            }
            char *new_path = NULL;
            new_path = (char *)malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(list->path) + 2));
            if (!new_path) {
                perror("ls");
                exit(errno);
            }
            strcpy(new_path, path);
            strcat(new_path, "/");
            strcat(new_path, list->path);
            new_path[ft_strlen(path) + ft_strlen(list->path) + 1] = '\0';
            DIR *dir = opendir(new_path);
            if (dir && (!S_ISLNK(list->stat.st_mode))) {
                print_form("\n%s:\n", new_path);
                open_dir(new_path, flags);
            }
            free(new_path);
            closedir(dir);
            list = list->next;
        }
    }
    free_sized_list(sized_list);
}

void check_args(char **argv, int flags, int argc)
{
    char *path[argc];
    t_sized_list *sized_list = malloc(sizeof(t_sized_list));
    sized_list->list_size = 0;
    sized_list->max_st_nlink = 0;
    sized_list->max_size = 0;
    sized_list->max_len = 0;
    sized_list->head = NULL;
    sized_list->tail = NULL;
    int i = 0;
    while(*argv) {
        DIR *dir = opendir(*argv);
        if (!dir)
            add_node(*argv, flags, &sized_list);
        else {
            path[i++] = *argv;
        }
        argv++;
        closedir(dir);
    }
    path[i] = NULL;
    if (sized_list->list_size > 0)
        print(&sized_list, flags);
    for (i = 0; path[i]; i++) {
        if (sized_list->list_size > 0)
            print_form("\n");
        print_form("%s:\n", path[i]);
        open_dir(path[i], flags);
    }
    free_sized_list(sized_list);
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
    if (j == 0) {
        if (flags & R) {
            print_form(".:\n");
        }
        open_dir(".", flags);
    } else {
        while (j < argc)
            argv[j++] = NULL;
        check_args(argv, flags, argc);
    }
    return 0;
}