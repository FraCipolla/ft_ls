#ifndef DIR_LIST_H
#define DIR_LIST_H

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_dir_list {
    struct dirent *dir;
    struct s_dir_list *head;
    struct s_dir_list *prev;
    struct s_dir_list *next;
    unsigned int size;
}   t_dir_list;

void print_dir_list(t_dir_list *dir_list);
void print_rev_dir_list(t_dir_list *list);
void free_dir_list(t_dir_list *dir_list);
t_dir_list *init(DIR *dir);

/* Utility */
char** to_arr(t_dir_list *list);
char** sort_dir_list(t_dir_list *list);

#endif