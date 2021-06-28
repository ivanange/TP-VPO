#ifndef DEF_UTILS
#define DEF_UTILS

// Constants
#define PLOT_SYMBOL (char)254u
#define PLOT_SCALE 150

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
    int spatial_resolution; // height * width
    int tonal_resolution;   // number of shades of gray
    int **image;
};

typedef struct SpectralImage SpectralImage;
struct SpectralImage
{
};

typedef struct CoordinatesList CoordinatesList;
struct CoordinatesList
{
    Coordinates *coordinates;
    int length;
};

typedef struct Hist Hist;
struct Hist
{
    float *hist;                       // array i -> H(i)
    CoordinatesList *hist_coordinates; // array i -> array Coordinates that have color i
    Image *image;
    int normalized; // reference to corresponding image image
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

/**
* parse ASCII PGM (P2) image given at specified path
* @param path image path
* @return Image*
*/
Image *parse_image(const char *path); // ok

void save(Image *img, char *path); // ok

/**
* print part of image from point to { point.x + radius, point.y + radius }
* @param image
* @param start_point where to start printing
* @param radius size to print from start point
* @return void
*/

void print_image(Image *image, Coordinates *start_point, int radius_x, int radius_y);
// >>>>>>> 65aad935b165f04a20ee0272e5cd157b6f8d62d1

/**
* Calculate luminosuity
* @param image
* @return float
*/
float luminousity(Image *image); // ok

/**
* Calculate contrast
* @param image
* @return float
*/
float contrast(Image *image); // ok

/**
* Build histogram of
* @param image
* @return Hist
*/
Hist *make_hist(Image *image, int normalize);

void print_hist(Hist *hist);

void plot_hist(Hist *hist);

Image *enhance_by_linear_trans(image *Image); // en cour

Image *enhance_by_linear_trans_sat(Hist *hist); // à revoir

Image *enhance_by_linear_trans_frag(Hist *hist);  // à revoir

Hist *enhance_by_histogram_equalization(Hist *hist); // en cour

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

Image *image_mul(Image *image1, int ratio); // ok

// Image *image_div(Image *image1, Image *image2);

Image *image_sub(Image *image1, Image *image2); // ok

Image *image_add(Image *image1, Image *image2); // ok

Image *image_or(Image *image1, Image *image2);

Image *image_not(Image *image);

Image *interpolate(Image *image, float factor); // à revoir

// helpers

int **allocate_dynamic_matrix(int row, int col);

void deallocate_dynamic_matrix(int **matrix, int row);

char **allocate_dynamic_char_matrix(int row, int col);

void deallocate_dynamic_char_matrix(char **matrix, int row);

#endif
