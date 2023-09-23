#include "../include/dir_list.h"
#include "../include/utils.h"
#include "../include/print_form.h"

void print_permission(mode_t stat, enum colors *color)
{
    *color = WHITE;
    if (S_ISDIR(stat)) {
        *color = BLUE;
        print_form("d");
    } else
        print_form("-");
    stat & S_IRUSR ? print_form("r") : print_form("-");
    stat & S_IWUSR ? print_form("w") : print_form("-");
    if (stat & S_IXUSR) {
        *color = *color == BLUE ? BLUE : GREEN;
        print_form("x");
    } else
        print_form("-");
    stat & S_IRGRP ? print_form("r") : print_form("-");
    stat & S_IWGRP ? print_form("w") : print_form("-");
    stat & S_IXGRP ? print_form("x") : print_form("-");
    stat & S_IROTH ? print_form("r") : print_form("-");
    stat & S_IWOTH ? print_form("w") : print_form("-");
    stat & S_IXOTH ? print_form("x") : print_form("-");
}

void color_print(const char* str, int color)
{
    print_form("\033[1;%dm%s\033[0m", color, str);
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

int get_cols()
{
    struct winsize ws;
    int fd;
    int cols;

    cols = 0;
    /* Open the controlling terminal. */
    fd = open("/dev/tty", O_RDWR);
    if (fd < 0)
        err(1, "/dev/tty");

    /* Get window size of terminal. */ 
    if (ioctl(fd, TIOCGWINSZ, &ws) < 0)
        err(1, "/dev/tty");
    close(fd);
    cols = ws.ws_col;
    return (cols);
}

size_t ft_strlen(const char *s)
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