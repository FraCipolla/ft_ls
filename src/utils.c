#include "../include/dir_list.h"
#include "../include/utils.h"
#include "../include/print_form.h"

void print_permission(mode_t stat, enum colors *color)
{
    char perm[10];
    int i = 0;
    if (S_ISDIR(stat)) {
        *color = blue;
        perm[i++] = 'd';
    } else
        perm[i++] = '-';
    if (stat & S_IRUSR) perm[i++] = 'r'; else perm[i++] = '-';
    if (stat & S_IWUSR) perm[i++] = 'w'; else perm[i++] = '-';
    if (stat & S_IXUSR) {
        *color = *color == blue ? blue : green;
        perm[i++] = 'x';
    } else
        perm[i++] = '-';
    if (stat & S_IRGRP) perm[i++] = 'r'; else perm[i++] = '-';
    if (stat & S_IWGRP) perm[i++] = 'w'; else perm[i++] = '-';
    if (stat & S_IXGRP) perm[i++] = 'x'; else perm[i++] = '-';
    if (stat & S_IROTH) perm[i++] = 'r'; else perm[i++] = '-';
    if (stat & S_IWOTH) perm[i++] = 'w'; else perm[i++] = '-';
    if (stat & S_IXOTH) perm[i++] = 'x'; else perm[i++] = '-';
    write (1, perm, 10);
}
void set_permission(mode_t stat, enum colors *color, char (*perm)[11])
{
    int i = 0;
    if (S_ISDIR(stat)) {
        *color = blue;
        (*perm)[i++] = 'd';
    } else if (S_ISLNK(stat)) {
        *color = cyan;
        (*perm)[i++] = 'l';
    } else 
        (*perm)[i++] = '-';
    if (stat & S_IRUSR) (*perm)[i++] = 'r'; else (*perm)[i++] = '-';
    if (stat & S_IWUSR) (*perm)[i++] = 'w'; else (*perm)[i++] = '-';
    if (stat & S_IXUSR) {
        if (*color != cyan && *color != blue)
            *color = green;

        (*perm)[i++] = 'x';
    } else
        (*perm)[i++] = '-';
    if (stat & S_IRGRP) (*perm)[i++] = 'r'; else (*perm)[i++] = '-';
    if (stat & S_IWGRP) (*perm)[i++] = 'w'; else (*perm)[i++] = '-';
    if (stat & S_IXGRP) (*perm)[i++] = 'x'; else (*perm)[i++] = '-';
    if (stat & S_IROTH) (*perm)[i++] = 'r'; else (*perm)[i++] = '-';
    if (stat & S_IWOTH) (*perm)[i++] = 'w'; else (*perm)[i++] = '-';
    if (stat & S_IXOTH) (*perm)[i++] = 'x'; else (*perm)[i++] = '-';
    (*perm)[i] = '\0';
}

// void print_item(const char *str, int num_sp)
// {
//     char sp = ' ';

//     if (str[0] == '.')
//         color_print(str, BLUE);
//     else
//         write(1, str, strlen(str));
//     while (num_sp--)
//         write(1, &sp, 1);
// }

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
