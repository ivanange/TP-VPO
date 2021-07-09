#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

Image *take_image(const char *path)
{
    Image *image = malloc(sizeof(Image));
    FILE *file;
    char ch, readChars[256];
    int row, col;
    int ch_int;

    file = fopen(path, "r+");
    if (file == NULL)
    {
        fprintf(stderr,RED "Error: Impossible d'ouvrir le fichier %s\n\n"RESET, path);
        exit(8);
    }

    printf(GRN"\n lecture du fichier: %s\n"RESET, path);

    /*determine pgm image type (only type three can be used)*/
    ch = getc(file);
    if (ch != 'P')
    {
        printf(RED"ERROR(1):format Non valide  (pgm/ppm/pbm attendu)\n"RESET);
        exit(1);
    }
    ch = getc(file);
    /*convert the one digit integer currently represented as a character to
    an integer(48 == '0')*/
   
     int type = ch - 48;
     if (type!=2)
    {
        printf(RED"ERROR(1):format Non valide \n"RESET);
        exit(1);
    }

    while (getc(file) != '\n'); /* skip to end of line*/

    fscanf(file, "%s", readChars);
    while (readChars[0] == '#')
    {
        //if a comment, get the rest of the line and a new word
        //si c'est un commentaire recueper le reste de ligne
        fgets(readChars, 255, file);
        fscanf(file, "%s", readChars);
    }

    
    sscanf(readChars, "%d", &(image->width));
    fscanf(file, "%d", &(image->height));
    fscanf(file, "%d", &(image->ton_max));
    image->tonal_resolution = image->ton_max + 1;
    image->spatial_resolution = image->height * image->width;
    printf("\n Image P%d",type);
    printf("\n Largeur: %d", image->width);
    printf("\n hauteur: %d", image->height);
    printf("\n Nombre de nuances de gris: %d", image->tonal_resolution);
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
    printf(GRN"\nLecture terminer avec success.\n\n"RESET);

    return image;
}
Image *take_imagebin(const char *path)
{
    Image *image = malloc(sizeof(Image));
    FILE *file;
    char ch, readChars[256];
    int row, col;
    int ch_int;

    file = fopen(path, "r+");
    if (file == NULL)
    {
        fprintf(stderr,RED "Error: Impossible d'ouvrir le fichier %s\n\n"RESET, path);
        exit(8);
    }

    printf(GRN"\n lecture du fichier: %s\n"RESET, path);

    /*determine pgm image type (only type three can be used)*/
    ch = getc(file);
    if (ch != 'P')
    {
        printf(RED"ERROR(1):format Non valide  (pgm/ppm attendu)\n"RESET);
        exit(1);
    }
    ch = getc(file);
    /*convert the one digit integer currently represented as a character to
    an integer(48 == '0')*/
     int type = ch - 48;
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
    //fscanf(file, "%d", &(image->ton_max));
    image->tonal_resolution = 0;
    image->spatial_resolution = image->height * image->width;
    printf("\n Image P%d",type);
    printf("\n Largeur: %d", image->width);
    printf("\n hauteur: %d", image->height);
    //printf("\n Nombre de nuances de gris: %d", image->tonal_resolution);
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
    printf(GRN"\nLecture terminer avec success.\n\n"RESET);
    image->ton_max =1 ;
    return image;
}

void save_image(Image *img, char *path)
{

    FILE *file;
    int row, col;
    int nr = img->height;
    int nc = img->width;

    file = fopen(path, "w");
    fprintf(file, "P2\n");
    fprintf(file, "# file mofidified by Lea\n");
    fprintf(file, "%d %d\n", nc, nr);
    fprintf(file, "%d\n", img->ton_max);

    for (row = 0; row < nr; row++)
    {
        for (col = 0; col < nc; col++)
        {
            fprintf(file, "%d ", img->image[row][col]);
        }
    }
    fprintf(file, "\n");
    fclose(file);
    printf(GRN"Image enregistrer sous %s \n\n"RESET, path);
}

void save_imageBin(Image *img, char *path)
{

    FILE *file;
    int row, col;
    int nr = img->height;
    int nc = img->width;

    file = fopen(path, "w");
    fprintf(file, "P1\n");
    fprintf(file, "# file mofidified by Lea\n");
    fprintf(file, "%d %d\n", nc, nr);
    

    for (row = 0; row < nr; row++)
    {
        for (col = 0; col < nc; col++)
        {
            fprintf(file, "%d ", img->image[row][col]);
        }
    }
    fprintf(file, "\n");
    fclose(file);
    printf(GRN"Image enregistrer sous %s \n\n"RESET, path);
}

void print_image(Image *image, Coordinates *start_point, int radius_x, int radius_y)
{
    printf("Image afficher du point { x: %d, y: %d } au point { x: %d, y: %d } \n\n", start_point->x, start_point->y, start_point->x + radius_x, start_point->y + radius_y);

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

float luminance(Image *image)
{
/* Cette fonction permet de calculer la luminausité de l'image*/
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

float contraste(Image *image)
{
/*calcul du contrat*/
    float mean = luminance(image);
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

int maxi(int *tab, int taille)
{
    int max=tab[0];  
    
    for (int i =1; i<taille; i++)
     {
        if (max < tab[i])
        {
         max = tab[i];
        }
         
    }  
    return max;
};


void histogramme(Image* image){

    printf("\thistogramme\n");
    int tab[image->tonal_resolution]; 
    int taille = image->tonal_resolution;
    int  row = image->height;
    int col = image->width;
    for (int i = 0; i < taille; i++)
    {
        tab[i] =0;
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j< col; j++)
        {
            int val = image->image[i][j];
           
            tab[val]++;

        }
        
    }
        
    int max =maxi(tab,  taille);
    for (int i = max; i > 0; i--)
    {
        printf("%d ", i);
        for (int j = 0; j < taille; j++)
        {
            
            if (tab[j]>= i)
            {
                printf("\u25A0\t");
            }
            else
            {
                printf(" \t");
            }
                
        }
        printf("\n");
            
    }
    printf("  ");
    for (int i = 0; i < taille; i++)
    {
        printf("%d\t", i) ;
    }
    printf("\n");
     
        
    }

void print_histogramme(Image *image)
{
    printf("\thistogramme\n");
    int tab[image->tonal_resolution]; 
    int taille = image->tonal_resolution;
    int  row = image->height;
    int col = image->width;
    for (int i = 0; i < taille; i++)
    {
        tab[i] =0;
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j< col; j++)
        {
            int val = image->image[i][j];
           
            tab[val]++;

        }
        
    }
    for (int i = 0; i < image->tonal_resolution; i++)
    {
        
        printf("%3d | %d \n", i, tab[i]);
    }
    printf("\n");
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
       
        row = filter->filters[k].rows;
        col = filter->filters[k].cols;
        radius_x = floor(row / 2);
        radius_y = floor(col / 2);

        for (i = 0; i < height; i++)
        {
            
            for (j = 0; j < width; j++)
            {
                
                sum = 0;
                coff = 0;
                scale = 1;
                x_start = 0;
                x_end = filter->filters[k].rows;
                y_start = 0;
                y_end = filter->filters[k].cols;

                if (i - radius_x < 0 || j - radius_y < 0 || i + radius_x >= height || j + radius_y >= width)
                {
                    
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



Image *addition_img(Image *image1, Image *image2)
{
    Image *image = malloc(sizeof(Image));
    int  pixel;
    if (image1->width != image2->width || image1->height != image2->height)
    {
        printf("ERROR(1): image1 and image2 should have the same dimension\n");
        exit(1);
    }
    image->width = image1->width;
    image->height = image1->height;
    image->spatial_resolution = image1->spatial_resolution;
    image->tonal_resolution = MAX(image1->tonal_resolution,image2->tonal_resolution);
    image->ton_max = MAX(image1->ton_max,image2->ton_max);
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
            pixel = MIN((image1->image[row][col] + image2->image[row][col]), image->ton_max);
            image->image[row][col] = pixel;
            //pixMax = MAX(acpix, pixMax);
        }
    }
    //image->tonal_resolution = pixMax+1;
    //image->ton_max = pixMax;

    return image;
}


Image *soustraction_img(Image *image1, Image *image2)
{
    // image1 - image2
    Image *image = malloc(sizeof(Image));
    int pixMax = 0, pixel;
    if (image1->width != image2->width || image1->height != image2->height)
    {
        printf(RED"ERROR(1): image1 et image2 n'ont pas la même taille\n"RESET);
        exit(1);
    }
    image->width = image1->width;
    image->height = image1->height;
    image->spatial_resolution = image1->spatial_resolution;
    image->tonal_resolution = MAX(image1->tonal_resolution,image2->tonal_resolution);
    image->ton_max = MAX(image1->ton_max,image2->ton_max);
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
            pixel = MAX((image1->image[row][col] - image2->image[row][col]), 0);
            image->image[row][col] = pixel;
            pixMax = MAX(pixel, pixMax);
        }
    }

    

    return image;
}



Image *multiplication_img(Image *image1, int ratio)
{
    // image1 * ratio
    Image *image = malloc(sizeof(Image));
    int pixMax = 0, pixel;
    image->width = image1->width;
    image->height = image1->height;
    image->tonal_resolution = image1->tonal_resolution;
    image->ton_max = image1->ton_max;
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
            pixel = MIN((image1->image[row][col] * ratio), image1->ton_max);
            image->image[row][col] = pixel;
            //pixMax = MAX(pixel, pixMax);
        }
    }
    

    return image;
}
Image *tansformation_lineaire(Image *image)
{
    int pixmax = image->ton_max;
    int LUT[image->tonal_resolution];
    int minI, maxI;
    Image *new = malloc(sizeof(Image));
    new->width = image->width;
    new->height = image->height;
    new->spatial_resolution = image->spatial_resolution;
    new->ton_max = image->ton_max;
    new->image = allocate_dynamic_matrix(image->height, image->width);
    maxI = image->image[0][0];
    minI = image->image[0][0];

    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            if (minI >= image->image[row][col])
            {
                minI = image->image[row][col];
            }
            if (maxI <= image->image[row][col])
            {
                maxI = image->image[row][col];
            }
        }
    }

    for (int i = 0; i <= pixmax; i++)
    {
        LUT[i] = pixmax *(i - minI) / (maxI - minI);
    }

    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            new->image[row][col] = LUT[image->image[row][col]];
        }
    }

    return new;
}

Image *tansformation_saturer(Image *image,int Smin ,int Smax)
{
    int pixmax = image->ton_max;
    int LUT[image->tonal_resolution];
    int minI, maxI;
    Image *new = malloc(sizeof(Image));
    new->width = image->width;
    new->height = image->height;
    new->spatial_resolution = image->spatial_resolution;
    new->ton_max = image->ton_max;
    new->image = allocate_dynamic_matrix(image->height, image->width);
    maxI = image->image[0][0];
    minI = image->image[0][0];

    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            if (minI >= image->image[row][col])
            {
                minI = image->image[row][col];
            }
            if (maxI <= image->image[row][col])
            {
                maxI = image->image[row][col];
            }
        }
    }
    if (Smin>=minI && Smax<=maxI)
    {
            for (int i = 0; i <= pixmax; i++)
        {   
            int m = MAX(i- Smin,0);
            LUT[i] = pixmax * m / (Smax - Smin);
        }

        for (int row = 0; row < image->height; row++)
        {
            for (int col = 0; col < image->width; col++)
            {
                new->image[row][col] = LUT[image->image[row][col]];
            }
        }
        
    }
    else{
        printf(RED"Mauvais Smin ou Smax"RESET);
        
    }

    return new;
}


Image *inverse_img(Image *image)
{
    int pixMax = image->ton_max;
    Image *new = malloc(sizeof(Image));
    new->width = image->width;
    new->height = image->height;
    new->spatial_resolution = image->spatial_resolution;
    new->tonal_resolution =image->tonal_resolution;
    new->ton_max = image->ton_max;
    new->image = allocate_dynamic_matrix(image->height, image->width);
   for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            new->image[row][col] = MAX(pixMax -image->image[row][col],0);
        }
    }
    return new;
}

Image *binarise_img(Image *image, int seuil)
{
    Image *new = malloc(sizeof(Image));
    new->width = image->width;
    new->height = image->height;
    new->spatial_resolution = image->spatial_resolution;
    new->tonal_resolution =image->tonal_resolution;
    new->ton_max = image->ton_max;
    new->image = allocate_dynamic_matrix(image->height, image->width);
    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {   
            if (image->image[row][col]> seuil)
            {
                 new->image[row][col] = 255;
            }
            else
            {
                new->image[row][col] = 0;
            }
            
        }
    }

    return new;
}



Image *egalisation_histogramme(Image *image1)
{

    Hist *hist = make_hist(image1, 1);
    int pixmax = image1->ton_max;
    // print_hist(hist);
    float *C = malloc((pixmax+1) * sizeof(int));
    for (int i = 0; i <= pixmax; i++)
    {
        float somme = 0.00;
        for (int j = 0; j <= i; j++)
        {

            somme = somme + hist->hist[j];
        }
        C[i] = somme;
        
    }

    Image *image = malloc(sizeof(Image));
    image->width = image1->width;
    image->height = image1->height;
    image->spatial_resolution = image1->spatial_resolution;
    image->tonal_resolution =image1->tonal_resolution;
    image->ton_max = image1->ton_max;
    image->image = allocate_dynamic_matrix(image->height, image->width);
    for (int row = 0; row < image->height; row++)
    {
        for (int col = 0; col < image->width; col++)
        {
            image->image[row][col] = C[image1->image[row][col]] * pixmax;
        }
    }

    return image;
}




/*fontion utiles*/


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






