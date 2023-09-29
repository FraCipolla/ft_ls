#include "../include/dir_list.h"
#include "../include/utils.h"
#include "../include/pf.h"

void set_permission(t_dir_list **list)
{
    (*list)->perm = malloc(sizeof(char) * 11);
    if (!(*list)->perm) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    if (S_ISDIR((*list)->stat->st_mode)) {
        (*list)->color = blue;
        (*list)->perm[i++] = 'd';
    } else if (S_ISLNK((*list)->stat->st_mode)) {
        (*list)->color = cyan;
        (*list)->perm[i++] = 'l';
        char tmp_buff[256];
        int size = readlink((*list)->path, tmp_buff, 256);
        if (size) {
            char *buff = malloc(sizeof(char) * size + 1);
            buff[size] = 0;
            readlink((*list)->path, buff, size);
            (*list)->link = buff;
        }
    } else 
        (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IRUSR) (*list)->perm[i++] = 'r'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IWUSR) (*list)->perm[i++] = 'w'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IXUSR) {
        if ((*list)->color != cyan && (*list)->color != blue)
            (*list)->color = green;

        (*list)->perm[i++] = 'x';
    } else
        (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IRGRP) (*list)->perm[i++] = 'r'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IWGRP) (*list)->perm[i++] = 'w'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IXGRP) (*list)->perm[i++] = 'x'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IROTH) (*list)->perm[i++] = 'r'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IWOTH) (*list)->perm[i++] = 'w'; else (*list)->perm[i++] = '-';
    if ((*list)->stat->st_mode & S_IXOTH) (*list)->perm[i++] = 'x'; else (*list)->perm[i++] = '-';
    (*list)->perm[i] = '\0';
}

char *get_ext_attr(char *path)
{
    char *buf, *key, *val;
    ssize_t buflen, keylen, vallen;
    // pf("path %s\n", path);
    buflen = listxattr(path, NULL, 0);
    if (buflen == -1) {
        // perror("listxattr");
        // exit(EXIT_FAILURE);
        return NULL;
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
                pf("xattr: %s", val);
            }
            free(val);
        }
        pf("\n");
        keylen = strlen(key) + 1;
        buflen -= keylen;
        key += keylen;
    }
    free(buf);
    return NULL;
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

int ft_comp_alph(char *s1, char *s2)
{
    int i = 0;
    int j = 0;
    char ch1;
    char ch2;
    if (s1[i] == '.') i++;
    if (s2[j] == '.') j++;
    while (s1[i] && s2[j]) {
        ch1 = s1[i];
        ch2 = s2[j];
        if (ch1 >= 65 && ch1 <= 90) ch1 += 32;
        if (ch2 >= 65 && ch2 <= 90) ch2 += 32;
        if (ch1 != ch2)
            break;
        ++i;
        ++j;
    }
    return (ch1 - ch2);
}