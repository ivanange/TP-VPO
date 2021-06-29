#ifndef DEF_FILTERS
#define DEF_FILTERS
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

SpatialFilter *create_filters()
{
    float **mat = allocate_dynamic_float_matrix(3, 3);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat[i][j] = 1;
        }
    }

    SpatialFilter *moyenneur = malloc(sizeof(SpatialFilter));
    moyenneur->filters = malloc(sizeof(FilterMatrix));
    moyenneur->filters[0].matrix = mat;
    moyenneur->filters[0].cols = 3;
    moyenneur->filters[0].rows = 3;
    moyenneur->length = 1;

    return moyenneur;
}

#endif
