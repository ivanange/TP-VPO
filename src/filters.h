#ifndef DEF_FILTERS
#define DEF_FILTERS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

typedef struct SpatialFilterList SpatialFilterList;
struct SpatialFilterList
{
    SpatialFilter *mean_3;
    SpatialFilter *gaussian_3;
};

int linear_convulv(int **image, int height, int width, int i, int j, FilterMatrix *filter, int radius_x, int radius_y, int x_start, int y_start, int x_end, int y_end)
{
    int x, y, row, col, sum = 0, coff = 0;
    for (x = x_start; x < x_end; x++)
    {
        for (y = y_start; y < y_end; y++)
        {
            row = (i - radius_x + x + height) % (height);
            col = (j - radius_y + y + width) % (width);
            sum = sum + floor(image[row][col] * filter->matrix[x][y]);
            coff = coff + filter->matrix[x][y];
        }
    }
    // only for coff != 0
    sum = coff != 0 ? floor(sum / coff) : sum;

    return sum;
}

SpatialFilter *make_mean_filter(int size)
{
    float **mat = allocate_dynamic_float_matrix(size, size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mat[i][j] = 1;
        }
    }

    SpatialFilter *mean = malloc(sizeof(SpatialFilter));
    mean->filters = malloc(sizeof(FilterMatrix));
    mean->filters[0].matrix = mat;
    mean->convulv_function = &linear_convulv;
    mean->filters[0].cols = size;
    mean->filters[0].rows = size;
    mean->length = 1;

    return mean;
}

SpatialFilter *make_gaussian_filter(int size, float mean, float std, float scale)
{
    int displacement, x, y;

    displacement = mean - floor(size / 2); // mean - radius

    float **mat = allocate_dynamic_float_matrix(size, size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            x = i + displacement;
            y = j + displacement;

            // printf(" %d,%d ", x, y);
            mat[i][j] = floor((exp(-((pow(x, 2) + pow(y, 2)) / (2 * pow(std, 2)))) / (2 * M_PI * pow(std, 2))) * scale);
        }
        printf("\n");
    }

    SpatialFilter *gaussian = malloc(sizeof(SpatialFilter));
    gaussian->filters = malloc(sizeof(FilterMatrix));
    gaussian->filters[0].matrix = mat;
    gaussian->convulv_function = &linear_convulv;
    gaussian->filters[0].cols = size;
    gaussian->filters[0].rows = size;
    gaussian->length = 1;

    return gaussian;
}

SpatialFilterList *create_filters()
{
    SpatialFilterList *spatial_filters = malloc(sizeof(SpatialFilterList));
    spatial_filters->mean_3 = make_mean_filter(3);
    spatial_filters->gaussian_3 = make_gaussian_filter(3, 0, 1, 1000);

    return spatial_filters;
}

#endif
