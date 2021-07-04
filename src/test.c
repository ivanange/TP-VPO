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

    // play with fleep
    print_image(feep, &origin, feep->height - 1, feep->width - 1);
    Hist *feep_hist = make_hist(feep, 0);
    plot_hist(feep_hist);
    SpatialFilterList *spatial_filters = create_filters();
    // Image *feep_moyen = convulv(feep, moyenneur, ZERO_EDGES);
    Image *feep_moyen = convulv(feep, spatial_filters->gaussian_3, PARTIAL_FILTERED_EDGES);
    save(feep_moyen, "./data/feep_moyen_3_partial_filtering.pgm");

    // Image *sum = image_add(feep, feep);
    // Image *difference = image_sub(feep, feep);
    // Image *product = image_mul(feep, 3);


    Image *fig3_brillant = parse_image("./data/sd/cacao_obscure.pgm");

    // printf("%d\n", obscure->spatial_resolution);
    // printf("%d\n", feep->tonal_resolution);
    Coordinates coords = {0, 0};
    Image *fig3_contrate = malloc(sizeof(Image));
    fig3_contrate = enhance_by_histogram_equalization(fig3_brillant);
    //
    print_image(fig3_contrate, &coords, 23, 23);
    // Hist *hist = make_hist(obscure, 0);

    // printf("number of pixels at 193: %.2f \n", hist->hist[193]);

    // print_hist(hist);

    save(fig3_contrate, "./data/cacao_constrate.pgm");

}
