#include <tree.h>

/**
 * Structure de données ayant pour but de faciliter le foreach des nodes
 */
typedef struct treedata
{
    const unsigned int seuil;
    image *img;
} treedata;

GNode* construire_arbre_zpixel(int x, int y, int taille, image * img, DegradationFunction f){
    if(img == NULL || img->contenu == NULL || x >= img->largeur || y >= img->hauteur || taille <= 0){
        return NULL;
    }
    my_stack_t *stack = stack_create(4);
    zpixel *pix = creerZpixelNoir(taille, (point) {x,y});
    zpixel *f1 = NULL, *f2 = NULL, *f3 = NULL, *f4 = NULL;
    int childrenCount = 0;
    GNode *node = g_node_new(pix);
    if(taille == 1){
        int index = y * img->rowstride + x * 3;
        rgbcolor color = {img->contenu[index], img->contenu[index+1], img->contenu[index+2]};
        pix->color = color;
    } else {
        int red=0, green=0, blue=0;
        GNode *fgh = construire_arbre_zpixel(x, y, taille/2 + (taille % 2), img, f);
        if(fgh != NULL){
            f1 = (zpixel *)fgh->data;
            g_node_insert(node, -1, fgh);
        }
        GNode *fdh = construire_arbre_zpixel(x + taille / 2, y, taille / 2 + (taille % 2), img, f);
        if(fdh != NULL) {
            f2 = (zpixel *) fdh->data;
            g_node_insert(node, -1, fdh);
        }
        GNode *fgb = construire_arbre_zpixel(x, y + taille / 2, taille / 2 + (taille % 2), img, f);
        if(fgb != NULL) {
            f3 = (zpixel *)fgb->data;
            g_node_insert(node, -1, fgb);
        }
        GNode *fdb = construire_arbre_zpixel(x + taille / 2, y + taille / 2, taille / 2 + (taille % 2), img, f);
        if(fdb != NULL) {
            f4 = (zpixel *)fdb->data;
            g_node_insert(node, -1, fdb);
        }
        if(f1 != NULL) {
            red += f1->color.red;
            green += f1->color.green;
            blue += f1->color.blue;
            childrenCount++;
        }
        if (f2 != NULL) {
            red += f2->color.red;
            green += f2->color.green;
            blue += f2->color.blue;
            childrenCount++;
        }
        if (f3 != NULL) {
            red += f3->color.red;
            green += f3->color.green;
            blue += f3->color.blue;
            childrenCount++;
        }
        if (f4 != NULL) {
            red += f4->color.red;
            green += f4->color.green;
            blue += f4->color.blue;
            childrenCount++;
        }
        red /= childrenCount;
        green /= childrenCount;
        blue /= childrenCount;
        pix->color = (rgbcolor) {red, green, blue};
        pix->degradation = degradation(node, stack, f);
        if(pix->degradation<0){
            stack_remove(stack);
            destroyNodeTree(node, NULL);
            return NULL;
        }
    }
    stack_remove(stack);
    return node;
}

void parcourir_arbre(GNode * root, gpointer data){
    treedata *dt = (treedata *)data;
    zpixel *pix = (zpixel *) root->data;
    image *img = (image *) dt->img;
    g_node_children_foreach(root, G_TRAVERSE_ALL, parcourir_arbre, data);
    if(pix->degradation < dt->seuil){
        if(projeter(pix, img) == -1) fprintf(stderr, "Erreur de projection du zpixel\n"); 
    }
}

int affiche_arbre(GNode *root, const unsigned int seuil, image *img){
    if(root == NULL || seuil <= 0 || img == NULL || img->contenu == NULL){
        return -1;
    }
    treedata data = {seuil, img};
    parcourir_arbre(root, &data);
    return 0;
}