#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main()
{

    printf("Hello world!\n");

    Image *image1 = parse_image("./data/image.pgm");
    Image *image2 = parse_image("./data/image.pgm");
    Image *apollo = parse_image("./data/apollonian_gasket.ascii.pgm");
    Image *feep = parse_image("./data/feep.ascii.pgm");
    Image *image = image_add(image1, image2);
    Coordinates coords = {0, 0};
    print_image(image, &coords, 6, 6);
    save(image, "./data/image2.pgm");

    printf("Here is feep: \n\n");
    print_image(feep, &coords, feep->height, feep->width);

    // Hist test;
    // int tt;
    // test.hist = calloc(5, sizeof(float));
    // test.hist = (float[]){0};
    // printf("Hello number : %f\n", test.hist[3]);

    // char *ptr;
    // ptr = (char *)malloc(10);
    // strcpy(ptr, "Programming");
    // printf(" %s,  Address = %u\n", ptr, ptr);

    // ptr = (char *)realloc(ptr, 20); //ptr is reallocated with new size
    // printf(" %s,  Address = %u\n", ptr, ptr);
    // strcat(ptr, " In 'C'");
    // printf(" %s,  Address = %u\n", ptr, ptr);
    // free(ptr);
}
