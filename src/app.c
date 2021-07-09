#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "filters.h"

int main()
{
    Coordinates coords = {0, 0};
    Image *feep = take_image("./data/lena.pgm");
    Image *lea2 = take_image("./data/lea2.pgm");
    Image *lea1 = take_image("./data/lea1.pgm");
    Image *mona_lisa = take_image("./data/mona_lisa.ascii.pgm");
    Image *mona = inverse_img(mona_lisa);
    Image *sum  = tansformation_saturer(feep,38,131);
    SpatialFilterList *spatial_filters = create_filters(10);
    //histogramme(feep);
    
    print_histogramme(feep);
    
    Image *feep_moyen = convulv(feep, spatial_filters->mean, PARTIAL_FILTERED_EDGES);
    
    save_image(feep_moyen,"./data/feep_msen_5.pgm");
    save_image(mona,"./data/monaverse.pgm");
    save_image(sum,"./data/ts.pgm");

    return 0;
}