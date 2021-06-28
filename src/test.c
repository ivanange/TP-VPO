#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main()
{
    Hist test;
    int tt;
    test.hist = calloc(5, sizeof(float));
    test.hist = (float[]){0};
    printf("Hello number : %f\n", test.hist[3]);

    char *ptr;
    ptr = (char *)malloc(10);
    strcpy(ptr, "Programming");
    printf(" %s,  Address = %u\n", ptr, ptr);

    ptr = (char *)realloc(ptr, 20); //ptr is reallocated with new size
    printf(" %s,  Address = %u\n", ptr, ptr);
    strcat(ptr, " In 'C'");
    printf(" %s,  Address = %u\n", ptr, ptr);
    free(ptr);

    // Coordinates *coords = malloc(sizeof(Coordinates));
    // coords = (Coordinates *){1, 2};
    // printf("x: %d, y: %d \n", coords->x, coords->y);

    return 0;
}