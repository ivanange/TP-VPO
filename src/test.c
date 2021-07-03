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
    Image *circuit = parse_image("./data/circuit.pgm");

    // get default filters
    SpatialFilterList *spatial_filters = create_filters();

    // play with fleep
    // print_image(feep, &origin, feep->height - 1, feep->width - 1);
    // Hist *feep_hist = make_hist(feep, 0);
    // plot_hist(feep_hist);
    // Image *feep_moyen = convulv(feep, moyenneur, ZERO_EDGES);
    // Image *feep_moyen = convulv(feep, spatial_filters->gaussian_3, PARTIAL_FILTERED_EDGES);
    // save(feep_moyen, "./data/feep_moyen_3_partial_filtering.pgm");

    //mona lisa
    // Image *mona_lisa_gaussian_3 = convulv(mona_lisa, spatial_filters->gaussian_3, PARTIAL_FILTERED_EDGES);
    // save(mona_lisa_gaussian_3, "./data/mona_lisa_gaussian_3_partial_filtering.pgm");

    //circuit with salt & pepper noise
    Image *circuit_median = convulv(circuit, spatial_filters->median_3, PARTIAL_FILTERED_EDGES);
    save(circuit_median, "./data/circuit_median_3_partial_filtering.pgm");

    // Image *sum = image_add(feep, feep);
    // Image *difference = image_sub(feep, feep);
    // Image *product = image_mul(feep, 3);
}
