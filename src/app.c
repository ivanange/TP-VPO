#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "filters.h"

int main()
{   
    char path[100]="",pathsave[100]="";
    int choix;
    int taille;
    SpatialFilterList *filters;
    int Smin,Smax;
    float g;
    int ratio;
    printf("############################################################################");
    printf("\t\tBienvenu sur le tp de vision  par ordinateu\n\n\n");
    printf("############################################################################\n\n");
    printf("Entrez l'image(chemin): -> ");
    scanf("%s",&path);
    Image *image = take_image(path),*transform,*image2;
    
debut:
    printf("\t\tQuelles operation voulez vous faire?\n");
    printf("############################################################################\n");
    printf("\t1-Dessiner l'histogramme\n");
    printf("\t2-afficher l'histogramme\n");
    printf("\t3-Inverser l'image\n");
    printf("\t4-Changer l'image\n");
    printf("\t5-luminance de l'image et contraste\n");
    printf("\t6-binarisation de l'image\n");
    printf("\t7-somme de 2 images\n");
    printf("\t8-soustraction de 2 img\n");
    printf("\t9-multiplication de l'image\n");
    printf("\t10-filtre moyenne\n");
    printf("\t11-filtre median\n");
    printf("\t12-filtre gaussien\n");
    printf("\t13-egalistion de l'histogramme\n");
    printf("\t14-transformation lineaire\n");
    printf("\t15-transformation lineaire avec saturation\n");
    printf("\t16-transformation gamma\n");
    printf("votre choix\n->");
    scanf("%d",&choix);

    switch (choix)
    {
        case 1:
            histogramme(image);
            goto debut;

        case 2:
            print_histogramme(image);
            goto debut;
        case 3:
            transform = inverse_img(image);
            goto save;
        case 4:
            printf("Entrez l'image(chemin): -> ");
            scanf("%s",&path);
            image = take_image(path);
            goto debut;
        case 5:
            printf(GRN"\t la luminance est :%f\n\t le contraste est :%f \n"RESET,luminance(image),contraste(image));
            goto debut;
        case 6:
            transform =binarise_img(image);
            goto debut;

        case 7:
            printf("Entrez image2(chemin): -> ");
            scanf("%s",&path);
            image2 = take_image(path);
            transform = addition_img(image,image2);
            goto save;
        case 8 : 
            printf("Entrez image2(chemin): -> ");
            scanf("%s",&path);
            image2 = take_image(path);
            transform = soustraction_img(image,image2);
            goto save;

        case 9 :
           
            printf("entrer le ratio: -> ");
            scanf("%d",&ratio);
            transform = multiplication_img(image,ratio);
            goto save;
        case 10 :
            
            printf("entrer la taille du filtre: -> ");
            scanf("%d",&taille);
            filters = create_filters(taille);
            transform = convulv(image, filters->mean, PARTIAL_FILTERED_EDGES);
            goto save;
        case 11 :
            
            printf("entrer la taille du filtre: -> ");
            scanf("%d",&taille);
            filters = create_filters(taille);
            transform = convulv(image, filters->median, PARTIAL_FILTERED_EDGES);
            goto save;
        case 12 :
            printf("entrer la taille du filtre: -> ");
            scanf("%d",&taille);
            filters = create_filters(taille);
            transform = convulv(image, filters->gaussian, PARTIAL_FILTERED_EDGES);
            goto save;
        case 13 :
            transform = egalisation_histogramme(image);
            goto save;

        case 14 :
            transform = tansformation_lineaire(image);
            goto save;
        
        case 15:
           
            printf("entrer Smin: -> ");
            scanf("%d",&Smin);
            printf("entrer Smax: -> ");
            scanf("%d",&Smax);
            transform = tansformation_saturer(image,Smin,Smax);
            goto save;

        case 16:
            
            printf("entrer gamm: -> ");
            scanf("%f",&g);
            transform = transformation_gamma(image,g);
            goto save;
        

        default:
        printf("Bye et merci") ;
         exit(1);
        break;

save : 
    printf("Entrez le chemin de sauvegarde: -> ");
    scanf("%s",&pathsave);
    save_image(transform,pathsave);
    goto debut;
}

    


    
   /* Coordinates coords = {0, 0};
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
    save_image(sum,"./data/ts.pgm");*/

    return 0;
}