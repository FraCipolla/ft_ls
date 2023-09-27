#include "../include/dir_list.h"
#include "../include/utils.h"
#include "../include/print_form.h"

static const char *COL_ARR[] = { RESET, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };
static const char *SPACES = "                                       ";

void print_dir_list(t_sized_list **list, int flags)
{
    struct winsize ws = get_term_size();
    int n_cols;
    int n_rows;
    if ((*list)->total_len < ws.ws_col) {
        n_cols = 0;
        n_rows = 1;
    } else {
        n_cols = (ws.ws_col / ((*list)->max_len) - 1);
        n_rows = ((*list)->list_size / n_cols);
    }
    t_dir_list *tmp = NULL;
    if (!(tmp = (*list)->head)) {
        return;
    }
    int is_tty = isatty(1);
    if (is_tty)
        write(1, COL_ARR[tmp->color], 7);
    int tmp_len = 0;
    int offset = 0;
    int curr_row = 0;
    while (tmp) {
        if (!is_tty) {
            write(1, tmp->path, ft_strlen(tmp->path));
            tmp = tmp->next;
            if (tmp)
                write(1, "\n", 1);
            continue;
        } 
        if (!(flags & f) && tmp->prev && (tmp->color != tmp->prev->color)) {
            write(1, COL_ARR[tmp->color], 7);
        }
        write(1, tmp->path, ft_strlen(tmp->path));
        if (ft_strlen(tmp->path) > tmp_len)
            tmp_len = ft_strlen(tmp->path);
        tmp = tmp->next;
        if (tmp) {
            write(1, "  ", 2);
            if (n_cols) {
                if(curr_row++ < n_rows) {
                    write(1, "\n", 1);
                    if (offset)
                        print_form("\033[%dC", offset);
                }
                if (curr_row == n_rows) {
                    offset += tmp_len + 2;
                    print_form("\033[%dA", n_rows);
                    print_form("\033[%dD", ws.ws_col);
                    print_form("\033[%dC", offset);
                    tmp_len = 0;
                    curr_row = 0;
                }
            }
        } else if (n_cols) {
            while (++curr_row < n_rows)
                write(1, "\n", 1);
        }
    }
    if (is_tty)
        write(1, RESET, 7);
    write(1, "\n", 1);
}

void print_rev_dir_list(t_sized_list **list)
{
    t_dir_list *tmp = (*list)->tail;
    while (tmp) {
        print_form("%s  ", tmp->path);
        tmp = tmp->prev;
    }
    write(1, "\n", 1);
}

void print_dir_list_l(t_sized_list **list)
{
    t_dir_list *tmp = (*list)->head;
    unsigned int k = 1, n = (*list)->max_st_nlink;
    while (n /= 10) {
        k++;
    }
    unsigned int s = (*list)->max_size;
    n = 1;
    while (s /= 10) {
        n++;
    }
    while (tmp) {
        print_permission(tmp->stat.st_mode, &tmp->color);
        unsigned int i = 0, j = tmp->stat.st_nlink;
        while (j /= 10)
            i++;
        write(1, SPACES, k - i);
        struct passwd *pw = getpwuid(tmp->stat.st_uid);
        struct group  *gr = getgrgid(tmp->stat.st_gid);
        print_form("%d %s %s ", tmp->stat.st_nlink, pw->pw_name, gr->gr_name);
        i = 0, j = tmp->stat.st_size;
        while (j /= 10)
            i++;
        write(1, SPACES, n - i);
        char *time = ctime(&tmp->stat.st_mtime);
        time[16] = '\0';
        print_form("%d %s ", tmp->stat.st_size, time + 4);
        COL_PRINT(COL_ARR[tmp->color], tmp->path, COL_ARR[reset]);
        write(1, "\n", 1);
        tmp = tmp->next;
    }
}

void print_rev_dir_list_l(t_sized_list **list)
{
    t_dir_list *tmp = (*list)->tail;
    unsigned int k = 1, n = (*list)->max_st_nlink;
    while (n /= 10) {
        k++;
    }
    unsigned int s = (*list)->max_size;
    n = 1;
    while (s /= 10) {
        n++;
    }
    while (tmp) {
        print_permission(tmp->stat.st_mode, &tmp->color);
        unsigned int i = 0, j = tmp->stat.st_nlink;
        while (j /= 10)
            i++;
        while (i++ < k)
            print_form(" ");
        print_form("%l ", tmp->stat.st_nlink);
        struct passwd *pw = getpwuid(tmp->stat.st_uid);
        struct group  *gr = getgrgid(tmp->stat.st_gid);
        print_form("%s %s", pw->pw_name, gr->gr_name);
        i = 0, j = tmp->stat.st_size;
        while (j /= 10)
            i++;
        while (i++ < n)
            print_form(" ");
        print_form("%l ", tmp->stat.st_size);
        char *time = ctime(&tmp->stat.st_mtime);
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
    sized_list->total_len = 0;
    sized_list->head = NULL;
    sized_list->tail = NULL;
    t_dir_list *list = NULL;
    struct dirent *entry;
    unsigned int i = 0;
    while ((entry = readdir(dir))) {
        if (!entry) {
            break;
        } else if ((!(strncmp(entry->d_name, ".", 1)) && !(flags & a) && !(flags & f)) || (!strncmp(entry->d_name, "..", 2) && !(flags & f) && !(flags & a))) {
            continue;
        }
        t_dir_list *new = malloc(sizeof(t_dir_list));
        new->path = NULL;
        new->color = reset;
        ft_strdup(&new->path, entry->d_name);
        new->prev = NULL;
        new->next = NULL;
        new->color = reset;
        new->len = ft_strlen(entry->d_name);
        sized_list->total_len += new->len;
        sized_list->max_len = new->len > sized_list->max_len ? new->len : sized_list->max_len;
        char *new_path = malloc(sizeof(char) * (ft_strlen(path) + new->len + 2));
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, entry->d_name);
        new_path[ft_strlen(path) + new->len + 1] = '\0';
        if (lstat(new_path, &new->stat) < 0) {
            print_form("entry: %s\n", entry->d_name);
            perror("ls");
            exit(errno);
        }
        set_permission(new->stat.st_mode, &new->color, &new->perm);
        free(new_path);
        if (flags & l || flags & t || flags & u) {
            if (new->stat.st_nlink > sized_list->max_st_nlink)
                sized_list->max_st_nlink = new->stat.st_nlink;
            if (new->stat.st_size > sized_list->max_size)
                sized_list->max_size = new->stat.st_size;
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

void add_node(char *path, int flags, t_sized_list **list)
{
    t_dir_list *new = malloc(sizeof(t_dir_list));
    ft_strdup(&new->path, path);
    new->prev = NULL;
    new->next = NULL;
    (*list)->max_len = ft_strlen(path) > (*list)->max_len ? ft_strlen(path) : (*list)->max_len;
    if (flags & l || flags & t || flags & u) {
        lstat((const char *)path, &new->stat);
        if (new->stat.st_nlink > (*list)->max_st_nlink)
            (*list)->max_st_nlink = new->stat.st_nlink;
        if (new->stat.st_size > (*list)->max_size)
            (*list)->max_size = new->stat.st_size;
    }
    if (!(*list)->head) {
        (*list)->head = new;
        (*list)->tail = new;
    } else {
        new->prev = (*list)->tail;
        (*list)->tail->next = new;
        (*list)->tail = (*list)->tail->next;
    }
    (*list)->list_size++;
}

void sort_by_name(t_sized_list **list)
{
    t_dir_list *tmp = (*list)->head;
    t_dir_list *tmp2 = (*list)->head;
    while (tmp) {
        tmp2 = tmp->next;
        while (tmp2) {
            if (tmp->path && tmp2->path && strcmp(tmp->path, tmp2->path) > 0) {
                char *swap = tmp->path;
                struct stat swap_stat = tmp->stat;
                enum colors swap_color = tmp->color;
                tmp->path = tmp2->path;
                tmp->stat = tmp2->stat;
                tmp2->path = swap;
                tmp2->stat = swap_stat;
                tmp->color = tmp2->color;
                tmp2->color = swap_color;
            }
            tmp2 = tmp2->next;
        }
        tmp = tmp->next;
    }
}

void sort_by_time(t_sized_list **list)
{
    t_dir_list *tmp = (*list)->head;
    t_dir_list *tmp2 = (*list)->head;
    while (tmp) {
        tmp2 = tmp->next;
        while (tmp2) {
            if (tmp->stat.st_mtime < tmp2->stat.st_mtime) {
                char *swap = tmp->path;
                struct stat swap_stat = tmp->stat;
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