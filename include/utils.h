#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

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

void print_permission(mode_t stat);

#endif