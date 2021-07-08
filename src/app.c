#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "filters.h"

int main()
{
    Coordinates coords = {0, 0};
    Image *feep = take_image("./data/feep.ascii.pgm");
    //Image *mona_lisa = ("./data/mona_lisa.ascii.pgm");
   // Image *apollo = parse_image("./data/apollonian_gasket.ascii.pgm");
    SpatialFilterList *spatial_filters = create_filters(5);
    histogramme(feep);
    print_histogramme(feep);
    // play with fleep
    Image *feep_moyen = convulv(feep, spatial_filters->mean, PARTIAL_FILTERED_EDGES);
    // print_image(feep_moyen, &coords, feep->height - 1, feep->width - 1);
    save_image(feep_moyen, "./data/feep_moyen_5.pgm");

    return 0;
}