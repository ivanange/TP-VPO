
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
    SpatialFilter *moyenneur = create_filters();

    // play with fleep
    Image *feep_moyen = convulv(feep, moyenneur, MIRROR_EDGES);
    // print_image(feep_moyen, &coords, feep->height - 1, feep->width - 1);
    save(feep_moyen, "./data/feep_moyen_3.pgm");

    return 0;
}
