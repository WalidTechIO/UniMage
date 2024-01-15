#include <zpixel.h>
#include <tree.h>
#include <degradation.h>
#include <utils.h>

int degradationTest(GNode *parent, my_stack_t *stack)
{
    zpixel* pixel = (zpixel *) parent->data;
    if(parent->children != NULL) pixel = (zpixel *) parent->children->data;
    return pixel->size;
}

int main(void){

    printf("TEST\n\n");

    image * img = creerImage(11, 12);

    point p = {1, 11};
    point p2 = {8, 7};

    rgbcolor color = {1, 200, 143}; 

    zpixel *ptrMonZPixel = creerZpixel(2, p, color); // Creation zpixel taille 2 x 1 y 11 rgb(1,200,143)

    zpixel *ptrMonZPixel2 = creerZpixelBlanc(3, p2); // Creation zpixel taille 3 x 8 y 7 blanc

    printf("Module zpixel:\n\n");

    if (ptrMonZPixel == NULL || ptrMonZPixel2 == NULL || ptrMonZPixel->color.red != 1 || ptrMonZPixel->size != 2) fprintf(stderr, "\n       Creer zpixel: KO\n");
    else printf("       Creer zpixel: OK\n");

    if (luminosite(ptrMonZPixel) != 100) fprintf(stderr, "       Luminosite zpixel: KO\n");
    else printf("       Luminosite zpixel: OK\n");

    if (saturation(ptrMonZPixel) != 252) fprintf(stderr, "       Saturation zpixel: KO\n");
    else printf("       Saturation zpixel: OK\n");

    if (distance(ptrMonZPixel, ptrMonZPixel2) != 282) fprintf(stderr, "       Distance zpixel: KO\n");
    else printf("       Distance zpixel: OK\n");

    if (projeter(ptrMonZPixel2, img) == -1 || img->contenu[img->rowstride * ptrMonZPixel2->pos.y + 3 * ptrMonZPixel2->pos.x] != 255 || img->contenu[0] != 0)
        fprintf(stderr, "       Projeter zpixel: KO\n");
    else printf("       Projeter zpixel: OK\n");

    if (projeter(ptrMonZPixel, img) == -1 || img->contenu[img->rowstride * ptrMonZPixel->pos.y + 3 * ptrMonZPixel->pos.x] != 1 || img->contenu[0] != 0) fprintf(stderr, "       Projeter zpixel en bordure : KO\n");
    else printf("       Projeter zpixel en bordure : OK\n");

    //afficher_image(img);
    //afficher_zpixel(ptrMonZPixel2);
    //afficher_zpixel(ptrMonZPixel);
    
    free(ptrMonZPixel);

    printf("\n\nModule arbre de zpixel:\n\n");

    //Construit arbre verifie resultat non null
    GNode *root = construire_arbre_zpixel(0, 0, MAX(img->hauteur, img->largeur), img, degradationTaille);
    if(root == NULL) fprintf(stderr, "       Construction arbre: KO\n");
    else printf("       Construction arbre: OK\n");

    //Affiche arbre verifie composante verte du pixel (0,11) != 0 car moyenne arbre appliquée
    // sur l'image et pixel colorée présent a proximité
    affiche_arbre(root, 3, img);
    if(img->contenu[img->rowstride*11+1] == 0) fprintf(stderr, "       Affichage arbre: KO\n");
    else printf("       Affichage arbre: OK\n");

    //afficher_image(img);
    destroyNodeTree(root, NULL);
    free(img->contenu);
    free(img);

    printf("\n\nModule degradation:\n\n");

    my_stack_t* stack = stack_create(4);
    root = g_node_new(ptrMonZPixel2);
    if (degradation(root, stack, degradationTest) != 3) fprintf(stderr, "       Calcul degradation sans enfants: KO\n");
    else printf("       Calcul degradation Node sans enfants: OK\n");

    GNode* anotherNode = g_node_new(creerZpixelNoir(1, p2));
    g_node_insert(root, -1, anotherNode);

    if (degradation(root, stack, degradationTest) != 1) fprintf(stderr, "       Calcul degradation avec enfants: KO\n");
    else printf("       Calcul degradation Node avec enfants: OK\n");

    destroyNodeTree(root, NULL);
    stack_remove(stack);

    return 0;
}