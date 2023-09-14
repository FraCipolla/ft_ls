#include "../include/dir_list.h"
#include "../include/utils.h"

void print_dir_list(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    while (tmp) {
        printf("%s  ", tmp->path);
        // t_dir_list *to_free = tmp;
        // if (!opendir(tmp->path)) {
        //     tmp->prev = tmp->next;
        //     free(to_free->stat);
        //     free(to_free);
        // } else {
        //     tmp = tmp->next;
        // }
        tmp = tmp->next;
    }
    printf("\n");
}

void print_rev_dir_list(t_sized_list *list)
{
    t_dir_list *tmp = list->tail;
    while (tmp) {
        printf("%s  ", tmp->path);
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
        printf("%s\n", tmp->path);
        tmp = tmp->next;
    }
}

void print_rev_dir_list_l(t_sized_list *list)
{
    t_dir_list *tmp = list->tail;
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
        printf("%s\n", tmp->path);
        tmp = tmp->prev;
    }
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
        new->path = entry->d_name;
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

void add_node(char *path, int flags, t_sized_list *list)
{
    t_dir_list *new = malloc(sizeof(t_dir_list));
    new->path = path;
    new->prev = NULL;
    new->next = NULL;
    if (flags & l || flags & t || flags & u) {
        new->stat = malloc(sizeof(struct stat));
        stat((const char *)path, new->stat);
        if (new->stat->st_nlink > list->max_st_nlink)
            list->max_st_nlink = new->stat->st_nlink;
        if (new->stat->st_size > list->max_size)
            list->max_size = new->stat->st_size;
    }
    if (!list->head) {
        list->head = new;
        list->tail = new;
    } else {
        new->prev = list->tail;
        list->tail->next = new;
        list->tail = list->tail->next;
    }
    list->list_size++;
}

void sort_by_name(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    t_dir_list *tmp2 = list->head;
    while (tmp) {
        tmp2 = tmp->next;
        while (tmp2) {
            if (strcmp(tmp->path, tmp2->path) > 0) {
                char *swap = tmp->path;
                tmp->path = tmp2->path;
                tmp2->path = swap;
                struct stat *tmp_stat = tmp->stat;
                tmp->stat = tmp2->stat;
                tmp2->stat = tmp_stat;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}

void sort_by_time(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    t_dir_list *tmp2 = list->head;
    while (tmp) {
        tmp2 = tmp->next;
        while (tmp2) {
            if (tmp->stat->st_mtime < tmp2->stat->st_mtime) {
                char *swap = tmp->path;
                tmp->path = tmp2->path;
                tmp2->path = swap;
                struct stat *tmp_stat = tmp->stat;
                tmp->stat = tmp2->stat;
                tmp2->stat = tmp_stat;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}