#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>  /* ioctl, TIOCGWINSZ */
#include <err.h>    /* err */
#include <fcntl.h>  /* open */
#include <string.h>
#include <unistd.h>

// #define BLACK  "\033[30m"
// #define RED    "\033[31m"
// #define GREEN  "\033[32m"
// #define YELLOW "\033[33m"
// #define BLUE   "\033[34m"
// #define MAGENTA    "\033[35m"
// #define CYAN   "\033[36m"
// #define WHITE  "\033[37m"

enum flags {
    l = 1 << 0, // 1
    R = 1 << 1, // 2
    a = 1 << 2, // 4
    r = 1 << 3, // 8
    t = 1 << 4, // 16
    u = 1 << 5, // 32
    f = 1 << 6, // 64
    g = 1 << 7, // 128
    d = 1 << 8, // 256
};

enum colors {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

void print_permission(mode_t stat, enum colors *color);
int get_cols();
void print_item(const char *str, int num_sp);
void color_print(const char* str, int color);
size_t ft_strlen(const char *s);
int ft_strcmp(const char *s1, const char *s2);
void ft_strdup(char **dest, const char *src);

#endif