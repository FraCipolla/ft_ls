#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

enum flags {
    l = 1 << 0,
    R = 1 << 1,
    a = 1 << 2,
    r = 1 << 3,
    t = 1 << 4,
    u = 1 << 5,
    f = 1 << 6,
    g = 1 << 7,
    d = 1 << 8,
};

int check_flags(char *s)
{
    if (*s == '-') {
        s++;
    } else {
        return 0;
    }

    int flags = 0;
    while (*s) {
        switch (*s) {
            case 'l':
                flags |= l;
                break;
            case 'R':
                flags |= R;
                break;
            case 'a':
                flags |= a;
                break;
            case 'r':
                flags |= r;
                break;
            case 't':
                flags |= t;
                break;
            case 'u':
                flags |= u;
                break;
            case 'f':
                flags |= f;
                break;
            case 'g':
                flags |= g;
                break;
            case 'd':
                flags |= d;
                break;
            default:
                return 0;
        }
        s++;
    }
    return flags;
}

void no_flags()
{
    DIR *dir = opendir(".");
    while (1) {
        struct dirent *entry = readdir(dir);
        if (!entry) {
            break;
        }
        if (entry->d_name[0] != '.')
            printf("%s  ", entry->d_name);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    int flags = 0;
    if (argc > 2) {
        flags = check_flags(argv[1]);
        printf("flags: %d\n", flags);
    } else {
        no_flags();
    }

    return 0;
}