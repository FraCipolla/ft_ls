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

#include "print_form.h"

#define RESET  "\033[0m"
#define BLACK  "\033[30m"
#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define BLUE   "\033[34m"
#define MAGENTA    "\033[35m"
#define CYAN   "\033[36m"
#define WHITE  "\033[37m"

#define PRINT_S(s) const char *p = s; while (*p) write(1, p++, 1);
#define PRINT_D(x) print_d(x)
#define PRINT_L(x) print_l(x)
#define COL_PRINT(col, s, reset) {write(1, col, 5); write(1, s, ft_strlen(s)); write(1, reset, 5);}

// static char PRINT_BUF[INT_MAX];
// static int PRINT_BUF_INDEX = 0;

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
    reset = 0,
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white
};

void print_permission(mode_t stat, enum colors *color);
int get_cols();
void print_item(const char *str, int num_sp);
void color_print(const char* str, int color);
size_t ft_strlen(const char *s);
int ft_strcmp(const char *s1, const char *s2);
void ft_strdup(char **dest, const char *src);
void print_arg(int n, ...);
void print_d(int n);
void print_l(long int n);
void write_and_clear(char **buffer, int *index);

#endif