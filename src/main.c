#include <utils.h>
#include <zpixel.h>
#include <tree.h>

/**
 * Fonction qui execute des tests sur le module zpixel
 */
//int testModuleZPixel(image *img);
int testModuleArbre(image *img);

    int main(int argc, char *argv[])
{
    printf("Début des tests !\n\n");

    image * monImage = creerImage(16, 16);
    if(monImage == NULL || monImage->contenu == NULL){
        fprintf(stderr, "Impossible de generer une image de test");
        return 0;
    }

    printf("Image de test:\noriginal.bmp");
    if(createBitmapFile("original.bmp", monImage) == -1)
        fprintf(stderr, "Erreur creation fichier bmp");

    /*printf("\n\n\n===============================\nModule zPixels\n===============================\n\n\n");

    switch (testModuleZPixel(monImage)){
        case -4:
            fprintf(stderr, "Calcul de la distance des zpixel erronée");
            break;
        case -3:
            fprintf(stderr, "Erreur creation fichier bmp");
            break;

        case -2:
            fprintf(stderr, "Erreur de projection\n");
            break;

        case -1:
            fprintf(stderr, "Erreur malloc ou coords negatives d'un zpixel\n");
            break;

        default:
            break;
    }*/

    printf("\n\n\n===============================\nModule Arbre de zPixels\n===============================\n\n\n");
    switch(testModuleArbre(monImage)){
        case -3:
            fprintf(stderr, "Comparaison des pixels echoué PROJECTION / SEUIL KO !");

        case -2:
            fprintf(stderr, "Image ou arbre NULL - Afficher arbre KO");
            break;

        case -1:
            fprintf(stderr, "Erreur construction arbre");
            break;

        case 0: 
            break;
    }

    free(monImage->contenu);
    free(monImage);
    return 0;
}

/*int testModuleZPixel(image * img){
    point p = {1, 1};
    point p2 = {15, 15};

    rgbcolor color = {1, 200, 143}; // rgb(1, 200, 143) La composante rouge va devenir 1 a la compilation car une couleur a 3 composantes unsigned char (0 à 255 ou 0x00 a 0xFF); 257 = 0x101 alors la composante rouge sera 0x01 (overflow du digit le plus significatif

    zpixel *ptrMonZPixel = creerZpixel(2, p, color); // Creation zpixel taille 2 x 1 y 1 rgb(1,200,143)

    zpixel *ptrMonZPixel2 = creerZpixelBlanc(3, p); // Creation zpixel taille 2 x 1 y 1 blanc

    if(ptrMonZPixel == NULL || ptrMonZPixel2 == NULL || img->contenu == NULL){
        return -1;
    }

    afficher_zpixel(ptrMonZPixel);
    afficher_zpixel(ptrMonZPixel2);
    if(distance(ptrMonZPixel, ptrMonZPixel2) != 282) return -4;
    printf("Calcul distance couleur de zpixel: OK");
    printf("\nProjection "); afficher_zpixel(ptrMonZPixel2);
    if(projeter(ptrMonZPixel2, img) == -1){
        return -2;
    };
    if(createBitmapFile("proj1.bmp", img) == -1) return -3;
    printf("Projection OK : proj1.bmp\nProjection "); afficher_zpixel(ptrMonZPixel);
    if(projeter(ptrMonZPixel, img) == -1){
        return -2;
    }
    if(createBitmapFile("proj2.bmp", img) == -1) return -3;
    ptrMonZPixel2->pos = p2;
    printf("Projection OK : proj2.bmp\nChangement de la position du "); afficher_zpixel(ptrMonZPixel2);
    printf("Projection "); afficher_zpixel(ptrMonZPixel2);
    if(projeter(ptrMonZPixel2, img) == -1){
        return -2;
    }
    if(createBitmapFile("proj3.bmp", img) == -1) return -3;
    printf("Projection OK : proj3.bmp");
    //Free zpixel
    free(ptrMonZPixel);
    free(ptrMonZPixel2);
    return 0;
}*/

int testModuleArbre(image * img){
    GNode * root = construire_arbre_zpixel(0, 0, 16, img);
    if(root == NULL){
        return -1;
    }
    printf("Arbre construit avec succes !\n");
    switch(affiche_arbre(root, 7, img)){
        case -1:
            return -2;
            break;
        
        case -2:
            return -3;
            break;

        case 0:
            printf("Parcours arbre OK ! Projection OK!\n");
            break;
    }
    createBitmapFile("traitée.bmp", img);
    printf("Output : traitee.bmp");
    return 0;
}