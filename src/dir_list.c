#include "../include/dir_list.h"

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

t_dir_list *dir_init(DIR *dir)
{
    t_dir_list *list = NULL;
    struct dirent *entry;
    unsigned int i = 0;
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
        i++;
    }
    list->size = i;
    return list;
}

char** dir_to_arr(t_dir_list *list)
{
    char **arr = malloc(sizeof(char*) * list->size + 1);
    int i = 0;
    t_dir_list *head = list->head;
    while (head) {
        arr[i] = head->dir->d_name;
        head = head->next;
        i++;
    }
    arr[i] = NULL;
    return arr;
}

char** sort_dir_list(t_dir_list *list)
{
    char **arr = dir_to_arr(list);
    unsigned int i = 0;
    unsigned int j = 0;
    char *tmp;
    while (i < list->size) {
        j = i + 1;
        while (j < list->size) {
            if (strcmp(arr[i], arr[j]) > 0) {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
            j++;
        }
        i++;
    }
    return arr;
}

char** sort_dir_by_time(t_dir_list *list)
{
    char **arr = dir_to_arr(list);
    struct stat *buf = malloc(sizeof(struct stat));
    unsigned int i = 0;
    unsigned int j = 0;
    char *tmp;
    while (i < list->size) {
        j = i + 1;
        while (j < list->size) {
            stat(arr[i], buf);
            time_t time1 = buf->st_mtime;
            stat(arr[j], buf);
            time_t time2 = buf->st_mtime;
            if (time1 < time2) {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
            j++;
        }
        i++;
    }
    return arr;
}