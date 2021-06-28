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
    image->tonal_resolution = image->tonal_resolution + 1;
    image->spatial_resolution = image->height * image->width;

    printf("\n width: %d", image->width);
    printf("\n height: %d", image->height);
    printf("\n shades: %d", image->tonal_resolution);
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
    printf("\nDone reading file.\n\n");

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

void print_image(Image *image, Coordinates *start_point, int radius_x, int radius_y)
{
    printf("printing from { x: %d, y: %d } to { x: %d, y: %d } \n\n", start_point->x, start_point->y, start_point->x + radius_x, start_point->y + radius_y);

    for (int i = start_point->x; i <= start_point->x + radius_x; i++)
    {
        for (int j = start_point->y; j <= start_point->y + radius_y; j++)
        {
            printf(" %d ", image->image[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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

Hist *make_hist(Image *image, int normalize)
{
    int shade;
    Hist *histogram = malloc(sizeof(Hist));
    histogram->image = image;

    histogram->hist = calloc(image->tonal_resolution, sizeof(float));

    // set all shades to 0
    for (int i = 0; i < image->tonal_resolution; i++)
    {
        histogram->hist[i] = 0;
    }

    histogram->hist_coordinates = calloc(image->tonal_resolution, sizeof(CoordinatesList));

    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            shade = image->image[i][j];
            histogram->hist[shade] = histogram->hist[shade] + 1;

            Coordinates *coords = malloc(sizeof(Coordinates));
            coords->x = i;
            coords->y = j;

            if (histogram->hist_coordinates[shade].coordinates == NULL)
            {
                // allocate memory for array
                histogram->hist_coordinates[shade].coordinates = coords;
                histogram->hist_coordinates[shade].length = 1;
            }
            else
            {
                // reallocate memory to append to array
                Coordinates *new = realloc(histogram->hist_coordinates[shade].coordinates, (histogram->hist_coordinates[shade].length + 1) * sizeof(Coordinates));

                if (new == NULL)
                {
                    /* something went wrong getting more memory, abort */
                    perror("Couldn't get memrory, aborting and exitting");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    // got more memory, append to array
                    histogram->hist_coordinates[shade].coordinates = new;
                    histogram->hist_coordinates[shade].coordinates[histogram->hist_coordinates[shade].length] = *coords;
                    histogram->hist_coordinates[shade].length = histogram->hist_coordinates[shade].length + 1;
                }
            }
        }
    }

    if (normalize == 1)
    {
        for (int i = 0; i < image->tonal_resolution; i++)
        {
            histogram->hist[i] = histogram->hist[i] / image->spatial_resolution;
        }

        histogram->normalized = 1;
    }

    return histogram;
}

void print_hist(Hist *hist)
{
    // print hist
    for (int i = 0; i < hist->image->tonal_resolution; i++)
    {
        // printf("%.3d | %f", i, print_hist[i]);
        printf("%3d | %f \n", i, hist->hist[i]);
    }
    printf("\n");
}

void plot_hist(Hist *hist)
{
    float *print_hist = calloc(hist->image->tonal_resolution, sizeof(float));

    /* normalize if not normalized and scale */
    for (int i = 0; i < hist->image->tonal_resolution; i++)
    {
        print_hist[i] = floor((hist->normalized == 1 ? hist->hist[i] : hist->hist[i] / hist->image->spatial_resolution) * PLOT_SCALE);
    }

    // print hist
    printf("\nHistogram (scaled to %d to fit screen, may not be accurate enough) \n", PLOT_SCALE);
    for (int i = 0; i < hist->image->tonal_resolution; i++)
    {
        // printf("%.3d | %f", i, print_hist[i]);
        printf("%3d |", i);

        for (int j = 0; j < print_hist[i]; j++)
        {
            printf("%c", PLOT_SYMBOL);
        }

        printf("\n");
    }
    printf("\n");
}

Image *convulv(Image *image, SpatialFilter *filter, const char *edges)
{
}

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

char **allocate_dynamic_char_matrix(int row, int col)
{
    char **ret_val;
    int i;

    ret_val = (char **)malloc(sizeof(char *) * row);
    if (ret_val == NULL)
    {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i)
    {
        ret_val[i] = (char *)malloc(sizeof(char) * col);
        if (ret_val[i] == NULL)
        {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }

    return ret_val;
}

void deallocate_dynamic_char_matrix(char **matrix, int row)
{
    int i;

    for (i = 0; i < row; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);

    Image *image_add(Image * image1, Image * image2)
    {
        Image *image = malloc(sizeof(Image));
        int pixMax = 0, acpix;
        if (image1->width != image2->width || image1->height != image2->height)
        {
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
