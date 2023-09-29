#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/ioctl.h>  /* ioctl, TIOCGWINSZ */
#include <err.h>    /* err */
#include <fcntl.h>  /* open */
#include <string.h>
#include <unistd.h>

#include "pf.h"
#include "dir_list.h"

#define RESET  "\033[1;0m"
#define BLACK  "\033[1;30m"
#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE   "\033[1;34m"
#define MAGENTA    "\033[1;35m"
#define CYAN   "\033[1;36m"
#define WHITE  "\033[1;37m"

#define PRINT_D(x) print_d(x)
#define PRINT_L(x) print_l(x)
#define COL_PRINT(col, s, reset) {write(1, col, 7); write(1, s, ft_strlen(s)); write(1, reset, 7);}

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

void set_permission(t_dir_list **list);
struct winsize get_term_size();
int ft_strlen(const char *s);
int ft_strcmp(const char *s1, const char *s2);
void ft_strdup(char **dest, const char *src);
void print_d(int n);
void print_l(long int n);
char *get_ext_attr(char *path);
int ft_strchr(const char *s, int c);
int ft_comp_alph(char *s1, char *s2);

#endif