#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void print_image(Image *image, Coordinates *point, int radius)
{
    for (int i = point->x; i <= point->x + radius; i++)
    {
        for (int j = point->y; j <= point->y; j++)
        {
            printf(' %d ', image->image[i][j]);
        }
        printf('\n');
    }
}

float luminousity(Image *image)
{
    for (int i = 0; i < image.; i++)
    {
        for (int j = point->y; j <= point->y; j++)
        {
            printf(' %d ', image->image[i][j]);
        }
        printf('\n');
    }
}
