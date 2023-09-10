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
    struct s_dir_list *head;
    struct s_dir_list *prev;
    struct s_dir_list *next;
    unsigned int size;
}   t_dir_list;

// typedef struct size_list {
//     unsigned int size;
//     struct s_dir_list *list;
// }   t_size_list;

void print_dir_list(t_dir_list *dir_list);
void print_rev_dir_list(t_dir_list *list);
void free_dir_list(t_dir_list *dir_list);
t_dir_list *dir_init(DIR *dir);

/* Utility */
char** dir_to_arr(t_dir_list *list);
char** sort_dir_list(t_dir_list *list);
char** sort_dir_by_time(t_dir_list *list);

#endif