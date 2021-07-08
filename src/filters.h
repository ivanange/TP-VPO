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
    SpatialFilter *median_3;
    SpatialFilter *min_3;
    SpatialFilter *max_3;
};

// calculate targeted column and row
void calc_indexes(int *row, int *col, int i, int j, int x, int y, int radius_x, int radius_y, int height, int width)
{
    *row = (i - radius_x + x);
    *row = (i - radius_x < 0 ? abs(*row + 1) : (i + radius_x >= height ? (2 * height) - (*row + 1) : *row)) % (height);

    *col = (j - radius_y + y);
    *col = (j - radius_y < 0 ? abs(*col + 1) : (j + radius_y >= width ? (2 * width) - (*col + 1) : *col)) % (width);
}

int linear_convulv(int **image, int height, int width, int i, int j, FilterMatrix *filter, int radius_x, int radius_y, int x_start, int y_start, int x_end, int y_end)
{
    int x, y, row, col, coff = 0;
    float sum = 0;
    for (x = x_start; x < x_end; x++)
    {
        for (y = y_start; y < y_end; y++)
        {
            calc_indexes(&row, &col, i, j, x, y, radius_x, radius_y, height, width);
            sum = sum + image[row][col] * filter->matrix[x][y];
            coff = coff + filter->matrix[x][y];
        }
    }
    // only for coff != 0
    sum = floor(coff != 0 ? sum / coff : sum);

    return (int)sum;
}

int median_convulv(int **image, int height, int width, int i, int j, FilterMatrix *filter, int radius_x, int radius_y, int x_start, int y_start, int x_end, int y_end)
{
    int x, y, row, col, k = 0, length = filter->rows * filter->cols, min_idx, temp;
    int *list = malloc(sizeof(int) * length);

    // take convolution elements into list
    for (x = x_start; x < x_end; x++)
    {
        for (y = y_start; y < y_end; y++)
        {
            calc_indexes(&row, &col, i, j, x, y, radius_x, radius_y, height, width);
            list[k] = image[row][col];
            k++;
        }
    }

    length = k;
    if (length == 0)
    {
        return 0;
    }

    // sort
    for (k = 0; k < length - 1; k++)
    {
        // Find the minimum element in unsorted array
        min_idx = k;
        for (j = k + 1; j < length; j++)
        {
            if (list[j] < list[min_idx])
            {
                min_idx = j;
            }
        }

        //swap
        temp = list[min_idx];
        list[min_idx] = list[k];
        list[k] = temp;
    }

    temp = list[(int)ceil(length / 2)];
    free(list);

    return temp;
}

int min_convulv(int **image, int height, int width, int i, int j, FilterMatrix *filter, int radius_x, int radius_y, int x_start, int y_start, int x_end, int y_end)
{
    int x, y, row, col, k = 0, length = filter->rows * filter->cols, min_idx, min;
    int *list = malloc(sizeof(int) * length);

    // take convolution elements into list
    for (x = x_start; x < x_end; x++)
    {
        for (y = y_start; y < y_end; y++)
        {
            calc_indexes(&row, &col, i, j, x, y, radius_x, radius_y, height, width);

            list[k] = image[row][col];
            k++;
        }
    }

    length = k;
    if (length == 0)
    {
        return 0;
    }

    // min
    min = list[0];
    for (k = 1; k < length; k++)
    {
        if (list[k] < min)
        {
            min = list[k];
        }
    }

    return min;
}

int max_convulv(int **image, int height, int width, int i, int j, FilterMatrix *filter, int radius_x, int radius_y, int x_start, int y_start, int x_end, int y_end)
{
    int x, y, row, col, k = 0, length = filter->rows * filter->cols, min_idx, max;
    int *list = malloc(sizeof(int) * length);

    // take convolution elements into list
    for (x = x_start; x < x_end; x++)
    {
        for (y = y_start; y < y_end; y++)
        {
            calc_indexes(&row, &col, i, j, x, y, radius_x, radius_y, height, width);

            list[k] = image[row][col];
            k++;
        }
    }

    length = k;
    if (length == 0)
    {
        return 0;
    }

    // max
    max = list[0];
    for (k = 1; k < length; k++)
    {
        if (list[k] > max)
        {
            max = list[k];
        }
    }

    return max;
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

SpatialFilter *make_median_filter(int size)
{
    SpatialFilter *median = malloc(sizeof(SpatialFilter));
    median->filters = malloc(sizeof(FilterMatrix));
    median->convulv_function = &median_convulv;
    median->filters[0].cols = size;
    median->filters[0].rows = size;
    median->length = 1;

    return median;
}

SpatialFilter *make_min_filter(int size)
{
    SpatialFilter *min = malloc(sizeof(SpatialFilter));
    min->filters = malloc(sizeof(FilterMatrix));
    min->convulv_function = &min_convulv;
    min->filters[0].cols = size;
    min->filters[0].rows = size;
    min->length = 1;

    return min;
}

SpatialFilter *make_max_filter(int size)
{
    SpatialFilter *max = malloc(sizeof(SpatialFilter));
    max->filters = malloc(sizeof(FilterMatrix));
    max->convulv_function = &max_convulv;
    max->filters[0].cols = size;
    max->filters[0].rows = size;
    max->length = 1;

    return max;
}

SpatialFilterList *create_filters()
{
    SpatialFilterList *spatial_filters = malloc(sizeof(SpatialFilterList));
    spatial_filters->mean = make_mean_filter(n);
    spatial_filters->gaussian = make_gaussian_filter(3, 0, 1, 1000);
    spatial_filters->median = make_median_filter(n);
    spatial_filters->min = make_min_filter(n);
    spatial_filters->max = make_max_filter(n);

    return spatial_filters;
}

#endif

