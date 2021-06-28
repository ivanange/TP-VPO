#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


int main()
{

    printf("Hello world!\n");

    Image *image1 = parse_image("./data/image.pgm");
    Image *image2 = parse_image("./data/image.pgm");
    Image *image = image_add(image1, image2);
    Coordinates coords = {0, 0};
    print_image(image, &coords, 6);
    save(image, "./data/image2.pgm");


    // Hist test;
    // int tt;
    // test.hist = calloc(5, sizeof(float));
    // test.hist = (float[]){0};
    // printf("Hello number : %f\n", test.hist[3]);
    // scanf("%d", &tt);
    // printf("number is : %d", tt);
    // return 0;
}
