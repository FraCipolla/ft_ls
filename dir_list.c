#include "dir_list.h"

void print_dir_list(t_dir_list *list)
{
    list = list->head;
    while (list) {
        printf("%s  ", list->dir->d_name);
        list = list->next;
    }
    printf("\n");
}

void print_rev_dir_list(t_dir_list *list)
{
    while (list) {
        printf("%s  ", list->dir->d_name);
        list = list->prev;
    }
    printf("\n");
}

void free_dir_list(t_dir_list *list)
{
    t_dir_list *tmp;
    while (list) {
        tmp = list;
        list = list->next;
        free(tmp);
    }
}

t_dir_list *f_init(DIR *dir)
{
    t_dir_list *list = NULL;
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (!entry) {
            break;
        }
        t_dir_list *new = malloc(sizeof(t_dir_list));
        new->dir = entry;
        new->prev = NULL;
        new->next = NULL;
        if (!list) {
            list = new;
            list->head = new;
        } else {
            new->head = list->head;
            list->next = new;
            new->prev = list;
            list = list->next;
        }
    }
    return list;
}
