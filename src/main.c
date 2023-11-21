#include <utils.h>
#include <zpixel.h>
#include <tree.h>
#include <degradation.h>

int testModuleDegradation(image *img);
void parcourir_arbre_degradation(GNode *root, gpointer data);

int main(int argc, char *argv[]){
    printf("Début des tests !\n\n");

    image * monImage = creerImage(16, 16);
    if(monImage == NULL || monImage->contenu == NULL){
        fprintf(stderr, "Impossible de generer une image de test");
        return 0;
    }

    printf("Image de test:\noriginal.bmp");
    if(createBitmapFile("original.bmp", monImage) == -1)
        fprintf(stderr, "Erreur creation fichier bmp");

    printf("\n\n\n===============================\nModule Degradation de zPixels\n===============================\n\n\n");
    switch(testModuleDegradation(monImage)){
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

int testModuleDegradation(image * img){

    printf("Construction de 3 arbres de zpixel ayant chacun un mode de degradation différent\n");

    GNode * root = construire_arbre_zpixel(0, 0, 16, img, degradationLuminosite);
    if(root == NULL){
        return -1;
    }
    GNode *root1 = construire_arbre_zpixel(0, 0, 16, img, degradationTaille);
    if (root == NULL){
        return -1;
    }
    GNode * root2 = construire_arbre_zpixel(0, 0, 16, img, degradationDistance);
    if(root == NULL){
        return -1;
    }

    printf("Test arbre degradation lumineuse\n");
    parcourir_arbre_degradation(root, NULL);
    printf("Test arbre degradation taille\n");
    parcourir_arbre_degradation(root, NULL);
    printf("Test arbre degradation distance\n");
    parcourir_arbre_degradation(root, NULL);

    printf("Projection des images et enregistrement\n");
    image *img1 = creerImage(16, 16), *img2 = creerImage(16, 16), *img3 = creerImage(16, 16);
    affiche_arbre(root, 120, img1);
    createBitmapFile("img1.bmp", img1);
    affiche_arbre(root1, 4, img2);
    createBitmapFile("img2.bmp", img2);
    affiche_arbre(root2, 15, img3);
    createBitmapFile("img3.bmp", img3);

    return 0;
}

void parcourir_arbre_degradation(GNode *root, gpointer data){
    if(root != NULL){
        g_node_children_foreach(root, G_TRAVERSE_ALL, parcourir_arbre_degradation, data);
        zpixel * pix = (zpixel *) root->data;
        if(pix->degradation == -1){
            fprintf(stderr, "Erreur de calcul de la degradation");
        }
    }
}