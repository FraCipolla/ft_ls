#include "../include/print_form.h"
#include "../include/utils.h"

void  print_form(const char *form, ...)
{
    va_list args;
    va_start(args, form);
    while(*form) {
        if (*form && *form == '%') {
            char c = *(++form);
            if (c) {
                switch (c)
                {
                case 's':
                    PRINT_S(va_arg(args, char *));
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

// void buff_d(int n, char **buffer, int *index)
// {
//     char c;
//     if (n < 0) {
//         (*buffer)[(*index)++] = '-';
//         if (*index > 200) {
//             write_and_clear(buffer, index);
//         }
//         n *= -1;
//     }
//     if (n / 10) {
//         buff_d(n / 10, buffer, index);
//     }
//     c = n % 10 + '0';
//     (*buffer)[(*index)++] = c;
//     if (*index > 200) {
//         write_and_clear(buffer, index);
//     }   
// }

// void buff_s(char *s, char **buffer, int *index)
// {
//     if (*index > 200) {
//         write_and_clear(buffer, index);
//     }
//     while (*s) {
//         (*buffer)[(*index)++] = *s++;
//         if (*index > 200) {
//             write_and_clear(buffer, index);
//         }
//     }
// }

// void buff_ld(long int n, char **buffer, int *index)
// {
//     if (*index > 200) {
//         write_and_clear(buffer, index);
//     }
//     char c;
//     if (n < 0) {
//         (*buffer)[(*index)++] = '-';
//         n *= -1;
//         if (*index > 200) {
//             write_and_clear(buffer, index);
//         }
//     }
//     if (n / 10) {
//         buff_ld(n / 10, buffer, index);
//     }
//     c = n % 10 + '0';
//     (*buffer)[(*index)++] = c;
//     if (*index > 200) {
//         write_and_clear(buffer, index);
//     }
// }

// void print_form(char **buffer, int *index, const char *form,...)
// {
//     va_list args;
//     va_start(args, form);
//     while(*form) {
//         if (*form && *form == '%') {
//             char c = *(++form);
//             if (c) {
//                 switch (c)
//                 {
//                 case 's':
//                     buff_s(va_arg(args, char *), &(*buffer), index);
//                     break;
//                 case 'l':
//                     buff_ld(va_arg(args, long int), &(*buffer), index);
//                     break;
//                 case 'd':
//                     buff_d(va_arg(args, int), &(*buffer), index);
//                     break;
//                 default:
//                     break;
//                 }
//             }
//         } else {
//             (*buffer)[(*index)++] = *form;
//         }
//         if (*index > 200) {
//             write_and_clear(buffer, index);
//         }
//         ++form;
//     }
//     va_end(args);
// }