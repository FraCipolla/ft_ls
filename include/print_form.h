#ifndef PRINT_FORM_H
#define PRINT_FORM_H

# include <unistd.h>
# include <stdlib.h>
# include <strings.h>
# include <stddef.h>
# include <stdarg.h>
# include <limits.h>

void print_form(const char *form, ...);
// void print_form(char **buffer, int *index, const char *form, ...);
void buff_d(int n, char **buffer, int *index);
void buff_s(char *s, char **buffer, int *index);
void buff_ld(long int n, char **buffer, int *index);

#endif