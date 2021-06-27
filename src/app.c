#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main()
{
    printf("Hello world!\n");

    Image *image = parse_image("./data/image.pgm");
    Coordinates coords = {0, 0};
    print_image(image, &coords, 6);

    return 0;
}