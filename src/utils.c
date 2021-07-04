#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

Image *parse_image(const char *path)
{
    Image *image = malloc(sizeof(Image));
    FILE *file;
    char ch, readChars[256];
    int row, col;
    int ch_int;

    file = fopen(path, "r+");
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

    fscanf(file, "%s", readChars);
    while (readChars[0] == '#')
    {
        //if a comment, get the rest of the line and a new word
        fgets(readChars, 255, file);
        fscanf(file, "%s", readChars);
    }

    //ok, comments are gone
    //get width, height, color depth
    sscanf(readChars, "%d", &(image->width));
    fscanf(file, "%d", &(image->height));
    fscanf(file, "%d", &(image->tonal_resolution));
    image->pixMax = image->tonal_resolution;
    image->tonal_resolution++;
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
    fprintf(file, "# by code source author\n");
    fprintf(file, "%d %d\n", nc, nr);
    fprintf(file, "%d\n", img->pixMax);

    for (row = 0; row < nr; row++)
    {
        for (col = 0; col < nc; col++)
        {
            fprintf(file, "%d ", img->image[row][col]);
        }
    }
    fprintf(file, "\n");
    fclose(file);
    printf("Saved image to %s \n\n", path);
}

void print_image(Image *image, Coordinates *start_point, int radius_x, int radius_y)
{
    printf("printing from { x: %d, y: %d } to { x: %d, y: %d } \n\n", start_point->x, start_point->y, start_point->x + radius_x, start_point->y + radius_y);

    for (int i = start_point->x; i <= start_point->x + radius_x; i++)
    {
        for (int j = start_point->y; j <= start_point->y + radius_y; j++)
        {
            printf(" %3d ", image->image[i][j]);
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
            histogram->hist[shade]++;

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
                    histogram->hist_coordinates[shade].length++;
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

Image *convulv(Image *image, SpatialFilter *filter, int edges_fill)
{
    int i, j, x, y, x_start, y_start, x_end, y_end, row, col, k, scale = 1, sum = 0, coff = 0, radius_x, radius_y, height = image->height, width = image->width;
    Image *new = malloc(sizeof(Image));
    (*new) = (*image);
    new->image = allocate_dynamic_matrix(height, width);

    // initialize new image pixels to 0
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            new->image[i][j] = 0;
        }
    }

    for (k = 0; k < filter->length; k++)
    {
        /* apply each filter */

        // check filter is odd
        // printf("length: %d, k: %d, cols: %d, rows: %d\n", filter->length, k, filter->filters[k].cols, filter->filters[k].rows);

        // set radius
        row = filter->filters[k].rows;
        col = filter->filters[k].cols;
        radius_x = floor(row / 2);
        radius_y = floor(col / 2);

        for (i = 0; i < height; i++)
        {
            // printf("%d\n", i);
            for (j = 0; j < width; j++)
            {
                // printf("%d,%d\n", i, j);
                sum = 0;
                coff = 0;
                scale = 1;
                x_start = 0;
                x_end = filter->filters[k].rows;
                y_start = 0;
                y_end = filter->filters[k].cols;

                if (i - radius_x < 0 || j - radius_y < 0 || i + radius_x >= height || j + radius_y >= width)
                {
                    /* handle edges_fill */
                    switch (edges_fill)
                    {
                    case ZERO_EDGES:
                        scale = 0;
                        x_end = 0;
                        break;

                    case PARTIAL_FILTERED_EDGES:
                        x_start = i - radius_x < 0 ? radius_x - i : 0;
                        x_end = i + radius_x >= height ? MIN(radius_x + filter->filters[k].rows - i, filter->filters[k].rows) : filter->filters[k].rows;
                        y_start = j - radius_y < 0 ? radius_y - j : 0;
                        y_end = j + radius_y >= width ? MIN(radius_y + filter->filters[k].cols - j, filter->filters[k].cols) : filter->filters[k].cols;
                        break;

                    default:
                        break;
                    }
                }

                //calculate filter
                sum = filter->convulv_function(image->image, height, width, i, j, filter->filters + k, radius_x, radius_y, x_start, y_start, x_end, y_end);

                new->image[i][j] = MIN(MAX(0, new->image[i][j] + (sum * scale)), 255);
            }
        }
    }

    return new;
}

int **allocate_dynamic_matrix(int row, int col)
{
    int **ret_val;
    int i;

    ret_val = malloc(sizeof(int *) * row);
    if (ret_val == NULL)
    {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i)
    {
        ret_val[i] = malloc(sizeof(int) * col);
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

float **allocate_dynamic_float_matrix(int row, int col)
{
    float **ret_val;
    int i;

    ret_val = malloc(sizeof(float *) * row);
    if (ret_val == NULL)
    {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i)
    {
        ret_val[i] = malloc(sizeof(float) * col);
        if (ret_val[i] == NULL)
        {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }

    return ret_val;
}

void deallocate_dynamic_float_matrix(float **matrix, int row)
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

    ret_val = malloc(sizeof(char *) * row);
    if (ret_val == NULL)
    {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < row; ++i)
    {
        ret_val[i] = malloc(sizeof(char) * col);
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
}

Image *image_add(Image *image1, Image *image2)
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
            acpix = MAX((image1->image[row][col] - image2->image[row][col]), 0);
            image->image[row][col] = acpix;
            pixMax = MAX(acpix, pixMax);
        }
    }
    image->tonal_resolution = pixMax;

    return image;
}

Image *image_sub(Image *image1, Image *image2)
{
    // image1 - image2
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
            acpix = MAX((image1->image[row][col] - image2->image[row][col]), 0);
            image->image[row][col] = acpix;
            pixMax = MAX(acpix, pixMax);
        }
    }

    image->tonal_resolution = pixMax;

    return image;
}

Image *image_mul(Image *image1, int ratio)
{
    // image1 * ratio
    Image *image = malloc(sizeof(Image));
    int pixMax = 0, acpix;
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
            acpix = MIN((image1->image[row][col] * ratio), 255);
            image->image[row][col] = acpix;
            pixMax = MAX(acpix, pixMax);
        }
    }
    image->tonal_resolution = pixMax;

    return image;
}

Image *enhance_by_linear_trans(Image *image1)
{
    int LUT[256];
    int minI, maxI;
    Image *image = malloc(sizeof(Image));
    image->width = image1->width;
    image->height = image1->height;
    image->spatial_resolution = image1->spatial_resolution;
    image->pixMax = image1->pixMax;
    image->image = allocate_dynamic_matrix(image->height, image->width);
    maxI = image1->pixMax;
    minI = image1->image[0][0];

    for (int row = 0; row < image1->height; row++)
    {
        for (int col = 0; col < image1->width; col++)
        {
            if (minI >= image1->image[row][col])
            {
                minI = image1->image[row][col];
            }
        }
    }

    for (int i = 0; i <= 255; i++)
    {
        LUT[i] = 255 * (i - minI) / (maxI - minI);
    }

    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            image->image[row][col] = LUT[image1->image[row][col]];
        }
    }

    return image;
}

Image *enhance_by_histogram_equalization(Image *image1)
{

    Hist *hist = make_hist(image1, 1);
    // print_hist(hist);
    float *C = malloc(256 * sizeof(int));
    for (int i = 0; i <= 255; i++)
    {
        float somme = 0.00;
        for (int j = 0; j <= i; j++)
        {

            somme = somme + hist->hist[j];
        }
        C[i] = somme;
        printf("%f\n", C[i]);
    }

    Image *image = malloc(sizeof(Image));
    image->width = image1->width;
    image->height = image1->height;
    image->spatial_resolution = image1->spatial_resolution;
    image->pixMax = image1->pixMax;
    image->image = allocate_dynamic_matrix(image->height, image->width);
    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            image->image[row][col] = C[image1->image[row][col]] * 255;
        }
    }

    return image;
}
