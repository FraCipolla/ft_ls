#ifndef PF_H
#define PF_H

# include <unistd.h>
# include <stdlib.h>
# include <strings.h>
# include <stddef.h>
# include <stdarg.h>
# include <limits.h>

void pf(const char *form, ...);
// void pf(char **buffer, int *index, const char *form, ...);
void buff_d(int n, char **buffer, int *index);
void buff_s(char *s, char **buffer, int *index);
void buff_ld(long int n, char **buffer, int *index);

#endif