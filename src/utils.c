#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

Image *parse_image(const char *path)
{
    Image *image = malloc(sizeof(Image));
    FILE *file;
    char ch;
    int row, col;
    int ch_int;

    file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n\n", path);
        exit(8);
    }

    printf("\nReading image file: %s\n", path);

    /*determine pgm image type (only type three can be used)*/
    ch = getc(file);
    if (ch != 'P')
    {
        printf("ERROR(1): Not valid pgm/ppm file type\n");
        exit(1);
    }
    ch = getc(file);
    /*convert the one digit integer currently represented as a character to
    an integer(48 == '0')*/

    while (getc(file) != '\n')
        ; /* skip to end of line*/

    while (getc(file) == '#') /* skip comment lines */
    {
        while (getc(file) != '\n')
            ; /* skip to end of comment line */
    }

    fseek(file, -1, SEEK_CUR); /* backup one character*/

    fscanf(file, "%d", &(image->width));
    fscanf(file, "%d", &(image->height));
    fscanf(file, "%d", &(image->tonal_resolution));

    printf("\n width  = %d", image->width);
    printf("\n height = %d", image->height);
    printf("\n shades = %d", image->tonal_resolution);
    printf("\n");

    image->image = allocate_dynamic_matrix(image->height, image->width);

    for (row = 0; row < image->height; row++)
    {
        for (col = 0; col < image->width; col++)
        {
            fscanf(file, "%d", &ch_int);
            //printf("%d", ch_int);
            if (ch_int < 0)
                printf("%d %d %d", row, col, ch_int);
            image->image[row][col] = ch_int;
        }
    }

    fclose(file);
    printf("\nDone reading file.\n");

    return image;
}

void save(Image *img, char *path)
{

    FILE *file;
    int row, col;
    int nr = img->height;
    int nc = img->width;

    file = fopen(path, "w");
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", nc, nr);
    fprintf(file, "%d\n", img->tonal_resolution);

    for (row = 0; row < nr; row++)
    {
        for (col = 0; col < nc; col++)
        {
            fprintf(file, "%d ", img->image[row][col]);
        }
    }
    fprintf(file, "\n");
    fclose(file);
    printf("Save image done \n");
}

void print_image(Image *image, Coordinates *start_point, int radius)
{
    for (int i = start_point->x; i <= start_point->x + radius; i++)
    {
        for (int j = start_point->y; j <= start_point->y + radius; j++)
        {
            printf(" %d ", image->image[i][j]);
        }
        printf("\n");
    }
}

float luminousity(Image *image)
{
    float sum = 0;

    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            sum = sum + image->image[i][j];
        }
    }

    return sum / image->spatial_resolution;
}

float contrast(Image *image)
{
    float mean = luminousity(image);
    float sum = 0;

    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            sum = sum + pow(image->image[i][j] - mean, 2);
        }
    }

    return pow(sum / image->spatial_resolution, 0.5);
}

// Hist *make_hist(Image *image)
// {
//     Hist *histogram = malloc(sizeof(Hist));
//     histogram->image = image;

//     histogram->hist = calloc(image->tonal_resolution, sizeof(float));
//     histogram->hist = (float[]){0};
//     histogram->hist_coordinates = calloc(image->tonal_resolution, sizeof(CoordinatesList));
// }

int **allocate_dynamic_matrix(int row, int col)
{
    int **ret_val;
    int i;

    ret_val = (int **)malloc(sizeof(int *) * row);
    if (ret_val == NULL)
    {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i)
    {
        ret_val[i] = (int *)malloc(sizeof(int) * col);
        if (ret_val[i] == NULL)
        {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }

    return ret_val;
}

void deallocate_dynamic_matrix(int **matrix, int row)
{
    int i;

    for (i = 0; i < row; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);
}


Image *image_add(Image *image1, Image *image2)
{
    Image *image = malloc(sizeof(Image));
    int pixMax=0, acpix;
    if (image1->width != image2->width || image1->height != image2->height) {
        printf("ERROR(1): image1 and image2 should have the same dimension\n");
        exit(1);
    }
    image->width = image1->width;
    image->height = image1->height;
    image->spatial_resolution = image1->spatial_resolution;
    image->image = allocate_dynamic_matrix(image->height, image->width);
    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            image->image[row][col] = 0;

        }
    }
    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            acpix = MIN((image1->image[row][col] + image2->image[row][col]), 255);
            image->image[row][col] = acpix;
            pixMax = MAX(acpix, pixMax);

        }
    }
    image->tonal_resolution = pixMax;

    return image;
}
