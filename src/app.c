#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main()
{
    printf("Image Processing!\n");

    Image *image = parse_image("./data/image.pgm");

    Coordinates coords = {18, 50};

    print_image(image, &coords, 20, 20);

    printf("pixel at 18:53 %d \n", image->image[18][53]);

    Hist *hist = make_hist(image, 0);

    printf("number of pixels at 193: %.2f \n", hist->hist[193]);

    // printf("number of pixels at 193: %d \n", hist->hist_coordinates[193].length);
    printf("coordinates of the first pixel of shade 193: x: %d, y: %d \n", hist->hist_coordinates[193].coordinates[0].x, hist->hist_coordinates[193].coordinates[0].y);

    printf("luminousity: %.2f \n", luminousity(image));

    printf("contrast: %.2f \n", contrast(image));

    // plot_hist(hist);

    print_hist(hist);

    return 0;
}