
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "filters.h"

int main()
{  
    
    Coordinates coords = {0, 0};
    Image *feep = parse_image("./data/feep.ascii.pgm");
    Image *mona_lisa = parse_image("./data/mona_lisa.ascii.pgm");
    Image *apollo = parse_image("./data/apollonian_gasket.ascii.pgm");
    Image *circuit = parse_image("./data/circuit.pgm");
    SpatialFilterList *spatial_filters = create_filters();
    Hist *feep_hist = make_hist(feep, 0);
    plot_hist(feep_hist);
    histogramme(feep);
    

    // play with fleep
    // Image *feep_moyen = convulv(feep, spatial_filters->mean_3, PARTIAL_FILTERED_EDGES);
    // // print_image(feep_moyen, &coords, feep->height - 1, feep->width - 1);
    // save(feep_moyen, "./data/feep_moyen_3.pgm");

    Image *circuit_median = convulv(circuit, spatial_filters->median_3, PARTIAL_FILTERED_EDGES);
    save(circuit_median, "./data/circuit_median_3_partial_filtering.pgm");
    printf(".....\u25A0");
    return 0;
}
