#include <stdio.h>
#include <stdlib.h>
#define TAILLE_MAX 100
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main()
{
printf("Hello world!\n");
int r;
r = MAX(5,6);
printf("%d\n",r );
return 0;
}
//
//
// int main(int argc, char *argv[])
// {
//     FILE* fichier = NULL;
//     char n[TAILLE_MAX] = "" , pixMax[5] = "";
//     int i,  j, b,  dimension[3] = {0};
//     char ligne, colonne ;
//     char caractereActuel;
//     fichier = fopen("image.PGM", "r");
//     if (fichier != NULL)
//     {
//         i = 0;
//         j = 0;
//
//         while (fgetc(fichier)!= EOF && i != 4) {
//
//             // fseek(fichier, -1, SEEK_CUR);
//
//             if (j == 0) {
//                 // printf("%d\n", j );
//                 while (j != 2) {
//                     caractereActuel = fgetc(fichier);
//                     if (caractereActuel == '\n') {
//                         j = j + 1;
//                     }
//                 }
//             }
//
//             if (i == 0) {
//                 fscanf(fichier, "%d %d\n", &dimension[0], &dimension[1]);
//
// 				i = i + 1;
//
//
//             }
//
// 			if (i == 1) {
// 				fgets(pixMax, 5, fichier);
// 			}
//
//
//             while (fgets(n, TAILLE_MAX, fichier) != NULL && i < 4) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
//             {
//
//                 printf("%s\n", n );
//                 i = i + 1;
//             }
//
//
//
//         }
//
//         printf("ligne = %d\n", dimension[0]);
//         printf("colonne = %d\n", dimension[1]);
//         printf("pixMax = %s\n", pixMax );
//
//         fclose(fichier);
//     }
//
//     return 0;
// }
