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
    scanf("%d", &tt);
    printf("number is : %d", tt);
    return 0;
}