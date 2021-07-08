#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "filters.h"

int main()
{
    Coordinates origin = {0, 0};
    Image *apollo = parse_image("./data/apollonian_gasket.ascii.pgm");
    Image *feep = parse_image("./data/feep.ascii.pgm");
    Image *mona_lisa = parse_image("./data/mona_lisa.ascii.pgm");
    Image *lena = parse_image("./data/lena.pgm");
    Image *lea_2 = parse_image("/home/mleandra/Images/Webcam/lea2.pgm");
    Image *lea_1 = parse_image("/home/mleandra/Images/Webcam/lea1.pgm");
    Image *dif= parse_image("/home/mleandra/Images/Webcam/dif.pgm");


    // play with fleep
    //print_image(lena, &origin, lena->height - 1, lena->width - 1);
    Hist *feep_hist = make_hist(lena, 0);
    //plot_hist(feep_hist);
    //print_hist(feep_hist);
    SpatialFilterList *spatial_filters = create_filters(15);
    // Image *feep_moyen = convulv(feep, moyenneur, ZERO_EDGES);
    //Image *feep_moyen = convulv(lena, spatial_filters->mean, PARTIAL_FILTERED_EDGES);
    //save(feep_moyen, "./data/lena_2.pgm");

    Image *sum = image_add(lea_1, lea_1);
    save(sum,"/home/mleandra/Images/Webcam/sum2.pgm");
    Image *difference = image_sub(lea_2, dif);
    save(difference,"/home/mleandra/Images/Webcam/dif2.pgm");
    // Image *product = image_mul(feep, 3);
}
