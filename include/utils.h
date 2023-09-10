#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

void print_array(char** arr)
{
    int i = 0;
    while (arr[i] != NULL)
    {
        printf("%s  ", arr[i]);
        i++;
    }
    printf("\n");
};

#endif