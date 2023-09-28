#include "../include/dir_list.h"
#include "../include/utils.h"
#include "../include/print_form.h"

void set_permission(t_dir_list **list)
{
    mode_t stat = (*list).stat;
    (*list)->perm = malloc(sizeof(char) * 10);
    char *perm = (*list)->perm;
    int i = 0;
    if (S_ISDIR(stat)) {
        (*list)->color = blue;
        perm[i++] = 'd';
    } else if (S_ISLNK(stat)) {
        (*list)->color = cyan;
        perm[i++] = 'l';
        char tmp_buff[256];
        int size = readlink((*list)->path, tmp_buff, 256);
        if (size) {
            char *buff = malloc(sizeof(char) * size + 1);
            readlink((*list)->path, buff, size);
            buff[size] = 0;
            (*list)->link = buff;
        }
    } else 
        perm[i++] = '-';
    if (stat & S_IRUSR) perm[i++] = 'r'; else perm[i++] = '-';
    if (stat & S_IWUSR) perm[i++] = 'w'; else perm[i++] = '-';
    if (stat & S_IXUSR) {
        if ((*list)->color != cyan && (*list)->color != blue)
            (*list)->color = green;

        perm[i++] = 'x';
    } else
        perm[i++] = '-';
    if (stat & S_IRGRP) perm[i++] = 'r'; else perm[i++] = '-';
    if (stat & S_IWGRP) perm[i++] = 'w'; else perm[i++] = '-';
    if (stat & S_IXGRP) perm[i++] = 'x'; else perm[i++] = '-';
    if (stat & S_IROTH) perm[i++] = 'r'; else perm[i++] = '-';
    if (stat & S_IWOTH) perm[i++] = 'w'; else perm[i++] = '-';
    if (stat & S_IXOTH) perm[i++] = 'x'; else perm[i++] = '-';
    perm[i] = '\0';
}

char *get_ext_attr(char *path)
{
    char *buf, *key, *val;
    ssize_t buflen, keylen, vallen;
    print_form("%s\n", path);
    buflen = listxattr(path, NULL, 0);
    if (buflen == -1) {
        perror("listxattr");
        exit(EXIT_FAILURE);
    }
    if (buflen == 0) {
        return NULL;
    }
    buf = malloc(buflen);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    buflen = listxattr(path, buf, buflen);
    if (buflen == -1) {
        perror("listxattr");
        exit(EXIT_FAILURE);
    }
    key = buf;
    while (buflen > 0) {
        vallen = getxattr(path, key, NULL, 0);
        if (vallen == -1)
            perror("getxattr");
        if (vallen > 0) {
            val = malloc(vallen + 1);
            if (val == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            vallen = getxattr(path, key, val, vallen);
            if (vallen == -1) {
                perror("getxattr");
            } else {
                val[vallen] = 0;
                // print_form("xattr: %s", val);
            }
        }
        keylen = strlen(key) + 1;
        buflen -= keylen;
        key += keylen;
    }
    free(buf);
    return val;
}

struct winsize get_term_size()
{
    struct winsize ws;
    int fd;

    /* Open the controlling terminal. */
    fd = open("/dev/tty", O_RDWR);
    if (fd < 0)
        err(1, "/dev/tty");

    /* Get window size of terminal. */ 
    if (ioctl(fd, TIOCGWINSZ, &ws) < 0)
        err(1, "/dev/tty");
    close(fd);
    return ws;
}

int ft_strlen(const char *s)
{
    const char *p = s;
    while (*p)
        p++;
    return (p - s);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return (*s1 - *s2);
}

void ft_strdup(char **dest, const char *src)
{
    *dest = malloc(sizeof(char) * (ft_strlen(src) + 1));
    strcpy(*dest, src);
}

void print_l(long int n)
{
    char c;
    if (n < 0) {
        write(1, "-", 1);
        n *= -1;
    }
    if (n / 10) {
        print_l(n / 10);
    }
    c = n % 10 + '0';
    write(1, &c, 1);
}

void print_d(int n)
{
    char c;
    if (n < 0) {
        write(1, "-", 1);
        n *= -1;
    }
    if (n / 10) {
        print_d(n / 10);
    }
    c = n % 10 + '0';
    write(1, &c, 1);
}

int ft_strchr(const char *s, int c)
{
    while (*s) {
        if (*s == c)
            return 1;
        s++;
    }
    return 0;
}