#ifndef DIR_LIST_H
#define DIR_LIST_H

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

enum colors {
    reset = 0,
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white
};

typedef struct s_dir_list {
    char *path;
    struct stat *stat;
    int color;
    char *perm;
    char *link;
    int len;
    struct s_dir_list *prev;
    struct s_dir_list *next;
}   t_dir_list;

typedef struct sized_list {
    int list_size;
    unsigned int max_st_nlink;
    int max_size;
    int max_len;
    int total_len;
    int total_blocks;
    struct s_dir_list *head;
    struct s_dir_list *tail;
}   t_sized_list;

t_sized_list *dir_init(DIR *dir, int flags, char *path);
void add_node(char *path, int flags, t_sized_list **list);

/* Utility */
void free_sized_list(t_sized_list *list);
void sort_by_name(t_sized_list **list);
void sort_by_time(t_sized_list **list);
void sort_by_access_time(t_sized_list **list);
void print_dir_list(t_sized_list **dir_list, int flags);
void print_rev_dir_list(t_sized_list **list, int flags);
void print_dir_list_l(t_sized_list **list, int flags);
void print_rev_dir_list_l(t_sized_list **list, int flags);

#endif