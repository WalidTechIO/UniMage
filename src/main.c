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
    printf("Output : traitee.bmp\n");

    
    
    image * img2 = creerImage(17, 34);
    createBitmapFile("original2.bmp", img2);
    GNode * root2 = construire_arbre_zpixel(0, 0, 34, img2);
    if (root2 == NULL)
    {
        return -1;
    }
    printf("Arbre 2 construit avec succes !\n");
    switch (affiche_arbre(root2, 7, img2))
    {
    case -1:
            return -2;
            break;

    case -2:
            return -3;
            break;

    case 0:
            printf("Parcours arbre 2 OK ! Projection OK!\n");
            break;
    }
    createBitmapFile("traitee2.bmp", img2);
    printf("Output : traitee2.bmp\n");
    return 0;
}