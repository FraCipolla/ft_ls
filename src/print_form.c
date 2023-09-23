#include "../include/print_form.h"
#include "../include/utils.h"

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

void print_s(char *s)
{
    write(1, s, strlen(s));
}

void print_form(const char *form, ...)
{
    va_list args;
    va_start(args, form);
    int color = WHITE;
    while(*form) {
        if (*form == '#') {
            color = va_arg(args, int);
            write(1, "\033[", 2);
            print_d(color);
            write(1, "m", 1);
            ++form;
        }
        if (*form && *form == '%') {
            char c = *(++form);
            if (c) {
                switch (c)
                {
                case 's':
                    print_s(va_arg(args, char *));
                    break;
                case 'l':
                    print_l(va_arg(args, long int));
                    break;
                case 'd':
                    print_d(va_arg(args, int));
                    break;
                default:
                    break;
                }
            }
        } else {
            write(1, form, 1);
        }
        ++form;
    }
    va_end(args);
}