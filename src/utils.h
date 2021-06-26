#ifndef DEF_UTILS
#define DEF_UTILS

// Constants
#define PLOT_SYMBOL '*';

typedef struct Coordinates Coordinates;
struct Coordinates
{
    int x;
    int y;
};

typedef struct Image Image;
struct Image
{
    int height;
    int width;
    int spatial_resolution;
    int tonal_resolution; // number of shades of gray
    int **image;
};

typedef struct SpectralImage SpectralImage;
struct SpectralImage
{
};

typedef struct Hist Hist;
struct Hist
{
    float *hist;                    // array i -> H(i)
    Coordinates **coordinates_hist; // array i -> array Coordinates that have color i
    Image *image;                   // reference to corresponding image image
};

typedef struct FilterMatrix FilterMatrix;
struct FilterMatrix
{
    float **matrix;
    int rows; // #rows
    int cols; // #cols
};

typedef struct SpatialFilter SpatialFilter;
struct SpatialFilter
{
    FilterMatrix *filters;
};

typedef struct SpectralFilter SpectralFilter;
struct SpectralFilter
{
};

Image *parse_image(const char *path);

/*
* print part of image from point to { point.x + radius, point.y + radius }
*/
void print_image(Image *image, Coordinates *point, int radius);

float luminousity(Image *image);

float contrast(Image *image);

Hist *make_hist(Image *image);

void plot_hist(Hist *hist);

Hist *enhance_by_repartition(Hist *hist);

Hist *enhance_by_partioned_repartition(Hist *hist);

Hist *enhance_by_histogram_equalization(Hist *hist);

/**
* Applies filter to image
* @param image: the image to convulv
* @param filter filter to apply
* @param edges: the mechanism to use to fill edges
* @return Image*
*/
Image *convulv(Image *image, SpatialFilter *filter, const char *edges);

SpectralImage *FFT(Image *image);

SpectralImage *transform(SpectralFilter *filter);

Image *FFT_inverse(SpectralImage *image);

//for ad hoc operations

Image *image_mul(Image *image1, Image *image2);

Image *image_div(Image *image1, Image *image2);

Image *image_sub(Image *image1, Image *image2);

Image *image_add(Image *image1, Image *image2);

Image *image_and(Image *image1, Image *image2);

Image *image_or(Image *image1, Image *image2);

Image *image_not(Image *image);

Image *interpolate(Image *image, float factor);

#endif