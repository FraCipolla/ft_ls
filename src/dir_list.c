#include "../include/dir_list.h"
#include "../include/utils.h"
#include "../include/print_form.h"

void print_dir_list(t_sized_list *list)
{
    t_dir_list *tmp = list->head;
    // unsigned int tmp_cols = get_cols();
    while (tmp) {
        // unsigned int curr_len = strlen(tmp->path);
        // if (curr_len > tmp_cols) {
        //     print_form("\n");
        //     tmp_cols = get_cols();
        // }
        // print_item(tmp->path, list->max_len - curr_len + 1);
        // tmp_cols -= list->max_len + 1;
        // t_dir_list *to_free = tmp;
        // if (!opendir(tmp->path)) {
        //     tmp->prev = tmp->next;
        //     free(to_free->stat);
        //     free(to_free);
        // } else {
        //     tmp = tmp->next;
        // }
        if (!tmp->path) {
            tmp = tmp->next;
            continue;
        }
        // if (tmp->stat && S_ISDIR(tmp->stat->st_mode))
        //     print_form("#%s\033[0m  ", BLUE, tmp->path);
        // else if (tmp->stat->st_mode & S_IXUSR)
        //     print_form("#%s\033[0m  ", GREEN, tmp->path);
        // else
        //     print_form("#%s\033[0m  ", WHITE, tmp->path);
        printf("%s  ", tmp->path);
        tmp = tmp->next;
    }
    printf("\n");
}

void print_rev_dir_list(t_sized_list *list)
{
    t_dir_list *tmp = list->tail;
    while (tmp) {
        print_form("%s  ", tmp->path);
        tmp = tmp->prev;
    }
    print_form("\n");
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
        print_permission(tmp->stat->st_mode, &tmp->color);
        unsigned int i = 0, j = tmp->stat->st_nlink;
        while (j /= 10)
            i++;
        while (i++ < k)
            print_form(" ");
        print_form("%d ", tmp->stat->st_nlink);
        struct passwd *pw = getpwuid(tmp->stat->st_uid);
        struct group  *gr = getgrgid(tmp->stat->st_gid);
        print_form("%s %s", pw->pw_name, gr->gr_name);
        i = 0, j = tmp->stat->st_size;
        while (j /= 10)
            i++;
        while (i++ < n)
            print_form(" ");
        print_form("%d ", tmp->stat->st_size);
        char *time = ctime(&tmp->stat->st_mtime);
        time[16] = '\0';
        print_form("%s #%s\033[0m\n", time + 4, tmp->color, tmp->path);
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
        print_permission(tmp->stat->st_mode, &tmp->color);
        unsigned int i = 0, j = tmp->stat->st_nlink;
        while (j /= 10)
            i++;
        while (i++ < k)
            print_form(" ");
        print_form("%l ", tmp->stat->st_nlink);
        struct passwd *pw = getpwuid(tmp->stat->st_uid);
        struct group  *gr = getgrgid(tmp->stat->st_gid);
        print_form("%s %s", pw->pw_name, gr->gr_name);
        i = 0, j = tmp->stat->st_size;
        while (j /= 10)
            i++;
        while (i++ < n)
            print_form(" ");
        print_form("%l ", tmp->stat->st_size);
        char *time = ctime(&tmp->stat->st_mtime);
        time[16] = '\0';
        print_form("%s #%s\033[0m\n", time + 4, tmp->color, tmp->path);
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
        free(tmp->path);
        free(tmp);
    }
    free(list);
}

void free_dir_list(t_dir_list *list)
{
    t_dir_list *tmp;
    while (list) {
        tmp = list;
        list = list->next;
        free(tmp->stat);
        free(tmp->path);
        free(tmp);
    }
}

t_sized_list *dir_init(DIR *dir, int flags, char *path)
{
    t_sized_list *sized_list = malloc(sizeof(t_sized_list));
    sized_list->list_size = 0;
    sized_list->max_st_nlink = 0;
    sized_list->max_size = 0;
    sized_list->max_len = 0;
    sized_list->head = NULL;
    sized_list->tail = NULL;
    t_dir_list *list = NULL;
    struct dirent *entry;
    unsigned int i = 0;
    while ((entry = readdir(dir))) {
        if (!entry) {
            break;
        } else if ((entry->d_name[0] == '.' && !(flags & a) && !(flags & f)) || !strncmp(entry->d_name, "..", 2)) {
            continue;
        }
        t_dir_list *new = malloc(sizeof(t_dir_list));
        new->path = NULL;
        new->stat = NULL;
        new->color = WHITE;
        ft_strdup(&new->path, entry->d_name);
        new->prev = NULL;
        new->next = NULL;
        new->stat = malloc(sizeof(struct stat));
        sized_list->max_len = strlen(entry->d_name) > sized_list->max_len ? strlen(entry->d_name) : sized_list->max_len;
        char *new_path = malloc(sizeof(char) * (strlen(path) + strlen(entry->d_name) + 2));
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, entry->d_name);
        new_path[strlen(path) + strlen(entry->d_name) + 1] = '\0';
        if (stat(new_path, new->stat) < 0) {
            printf("entry: %s\n", entry->d_name);
            perror("ls");
            exit(errno);
        }
        free(new_path);
        // printf("entry->d_name: %s\n", entry->d_name);
        if (flags & l || flags & t || flags & u) {
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
    closedir(dir);
    return sized_list;
}

void add_node(char *path, int flags, t_sized_list *list)
{
    t_dir_list *new = malloc(sizeof(t_dir_list));
    ft_strdup(&new->path, path);
    new->prev = NULL;
    new->next = NULL;
    list->max_len = strlen(path) > list->max_len ? strlen(path) : list->max_len;
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
            if (tmp->path && tmp2->path && strcmp(tmp->path, tmp2->path) > 0) {
                char *swap = tmp->path;
                struct stat *swap_stat = tmp->stat;
                tmp->path = tmp2->path;
                tmp->stat = tmp2->stat;
                tmp2->path = swap;
                tmp2->stat = swap_stat;
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
                struct stat *swap_stat = tmp->stat;
                tmp->path = tmp2->path;
                tmp->stat = tmp2->stat;
                tmp2->path = swap;
                tmp2->stat = swap_stat;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}