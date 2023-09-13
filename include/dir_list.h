#ifndef DIR_LIST_H
#define DIR_LIST_H

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

typedef struct s_dir_list {
    struct dirent *dir;
    struct stat *stat;
    struct s_dir_list *prev;
    struct s_dir_list *next;
}   t_dir_list;

typedef struct sized_list {
    unsigned int list_size;
    unsigned int max_st_nlink;
    unsigned int max_size;
    struct s_dir_list *head;
    struct s_dir_list *tail;
}   t_sized_list;

t_sized_list *dir_init(DIR *dir, int flags);

/* Utility */
void free_sized_list(t_sized_list *list);
void sort_by_name(t_sized_list *list);
void sort_by_time(t_sized_list *list);
void print_dir_list(t_sized_list *dir_list);
void print_rev_dir_list(t_sized_list *list);
void print_dir_list_l(t_sized_list *list);
void print_rev_dir_list_l(t_sized_list *list);

#endif