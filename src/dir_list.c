#include "../include/dir_list.h"
#include "../include/utils.h"

void print_dir_list(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    while (tmp) {
        printf("%s  ", tmp->dir->d_name);
        tmp = tmp->next;
    }
    printf("\n");
}

void print_rev_dir_list(t_sized_list *list)
{
    t_dir_list *tmp = list->tail;
    while (tmp) {
        printf("%s  ", tmp->dir->d_name);
        tmp = tmp->prev;
    }
    printf("\n");
}

void print_dir_list_l(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    unsigned int k = 1, n = list->max_st_nlink;
    while (n /= 10) {
        k++;
    }
    unsigned int s = list->max_size;
    n = 1;
    while (s /= 10) {
        n++;
    }
    while (tmp) {
        print_permission(tmp->stat->st_mode);
        unsigned int i = 0, j = tmp->stat->st_nlink;
        while (j /= 10)
            i++;
        while (i++ < k)
            printf(" ");
        printf("%ld ", tmp->stat->st_nlink);
        struct passwd *pw = getpwuid(tmp->stat->st_uid);
        struct group  *gr = getgrgid(tmp->stat->st_gid);
        printf("%s %s", pw->pw_name, gr->gr_name);
        i = 0, j = tmp->stat->st_size;
        while (j /= 10)
            i++;
        while (i++ < n)
            printf(" ");
        printf("%ld ", tmp->stat->st_size);
        char *time = ctime(&tmp->stat->st_mtime);
        time[16] = '\0';
        printf("%s ", time + 4);
        printf("%s\n", tmp->dir->d_name);
        tmp = tmp->next;
    }
    printf("\n");
}

void free_sized_list(t_sized_list *list)
{
    t_dir_list *tmp;
    while (list->head) {
        tmp = list->head;
        list->head = list->head->next;
        free(tmp->stat);
        free(tmp);
    }
    free(list);
}

t_sized_list *dir_init(DIR *dir, int flags)
{
    t_sized_list *sized_list = malloc(sizeof(t_sized_list));
    sized_list->max_st_nlink = 0;
    sized_list->max_size = 0;
    t_dir_list *list = NULL;
    struct dirent *entry;
    unsigned int i = 0;
    while ((entry = readdir(dir))) {
        if (!entry) {
            break;
        } else if (entry->d_name[0] == '.' && !(flags & a) && !(flags & f)) {
            continue;
        }
        t_dir_list *new = malloc(sizeof(t_dir_list));
        new->dir = entry;
        new->prev = NULL;
        new->next = NULL;
        if (flags & l || flags & t || flags & u) {
            new->stat = malloc(sizeof(struct stat));
            stat(entry->d_name, new->stat);
            if (new->stat->st_nlink > sized_list->max_st_nlink)
                sized_list->max_st_nlink = new->stat->st_nlink;
            if (new->stat->st_size > sized_list->max_size)
                sized_list->max_size = new->stat->st_size;
        }
        if (!list) {
            list = new;
            sized_list->head = list;
        } else {
            if (!(flags & t) && !(flags & f))
                new->prev = list;
            list->next = new;
            list = list->next;
        }
        i++;
    }
    sized_list->tail = list;
    sized_list->list_size = i;
    return sized_list;
}

void sort_by_name(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    t_dir_list *tmp2 = list->head;
    while (tmp) {
        tmp2 = tmp->next;
        while (tmp2) {
            if (strcmp(tmp->dir->d_name, tmp2->dir->d_name) > 0) {
                struct dirent *tmp_dir = tmp->dir;
                tmp->dir = tmp2->dir;
                tmp2->dir = tmp_dir;
                struct stat *tmp_stat = tmp->stat;
                tmp->stat = tmp2->stat;
                tmp2->stat = tmp_stat;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}

// TODO: sort by time
void sort_by_time(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    t_dir_list *tmp2 = list->head;
    while (tmp) {
        tmp2 = tmp->next;
        while (tmp2) {
            if (tmp->stat->st_mtime < tmp2->stat->st_mtime) {
                struct dirent *tmp_dir = tmp->dir;
                tmp->dir = tmp2->dir;
                tmp2->dir = tmp_dir;
                struct stat *tmp_stat = tmp->stat;
                tmp->stat = tmp2->stat;
                tmp2->stat = tmp_stat;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}