#include <tree.h>

GNode *construire_arbre_zpixel(int x, int y, int taille, image * img){
    if(img == NULL || img->contenu == NULL || x > img->largeur || y > img->largeur || taille <= 0){
        return NULL;
    }
    my_stack_t *stack = stack_create(4);
    zpixel *pix = creerZpixelNoir(taille, (point) {x,y});
    zpixel *f1, *f2, *f3, *f4;
    int childrenCount = 0;
    GNode *node = g_node_new(pix);
    if(taille == 1){
        int index = y *img->rowstride + x * 3;
        rgbcolor color = {img->contenu[index], img->contenu[index+1], img->contenu[index+2]};
        pix->color = color;
    } else {
        int red=0, green=0, blue=0;
        GNode *fgh = construire_arbre_zpixel(x, y, taille/2, img);
        if(fgh != NULL){
            f1 = (zpixel *)fgh->data;
            g_node_insert(node, 0, fgh);
        } 
        GNode *fdh = construire_arbre_zpixel(x+taille/2, y, taille/2, img);
        if(fdh != NULL) {
            f2 = (zpixel *) fdh->data;
            g_node_insert(node, 1, fdh);
        }
        GNode *fgb = construire_arbre_zpixel(x, y + taille/2, taille/2, img);
        if(fgb != NULL) {
            f3 = (zpixel *)fgb->data;
            g_node_insert(node, 2, fgb);
        }
        GNode *fdb = construire_arbre_zpixel(x + taille / 2, y + taille / 2, taille / 2, img);
        if(fdb != NULL) {
            f4 = (zpixel *)fdb->data;
            g_node_insert(node, 3, fdb);
        }
        if(f1) {
            red += f1->color.red;
            green += f1->color.green;
            blue += f1->color.blue;
            childrenCount++;
        }
        if (f2) {
            red += f2->color.red;
            green += f2->color.green;
            blue += f2->color.blue;
            childrenCount++;
        }
        if (f3) {
            red += f3->color.red;
            green += f3->color.green;
            blue += f3->color.blue;
            childrenCount++;
        }
        if (f4) {
            red += f4->color.red;
            green += f4->color.green;
            blue += f4->color.blue;
            childrenCount++;
        }
        red /= childrenCount;
        green /= childrenCount;
        blue /= childrenCount;
        pix->color = (rgbcolor) {red, green, blue};
        pix->degradation = degradation(node, stack, degradationLuminosite);
    }
    return node;
}

void parcourir_arbre(GNode * root, gpointer data){
    treedata *dt = (treedata *)data;
    zpixel * pix = (zpixel *) root->data;
    image * img = (image *) dt->img;
    g_node_children_foreach(root, G_TRAVERSE_ALL, parcourir_arbre, data);
    if(pix->degradation < dt->seuil){
        if(projeter(pix, img) == -1) fprintf(stderr, "Erreur de projection du zpixel\n"); 
    }
}

int affiche_arbre(GNode *root, const unsigned int seuil, image *img){
    if(root == NULL || seuil <= 0 || img == NULL || img->contenu == NULL){
        return -1;
    }
    //Le proto de la function prise en parametre dans g_node_children_foreach prends en argument seulement un pointer vers le noeud parent et un pointer user_data 
    //Pour pouvoir transmettre a cette fonction a la fois un seuil et a la fois une image on utilise une struct
    treedata data = {seuil, img};
    parcourir_arbre(root, &data);
    //img->contenu 0 et img->contenu[3*img->rowstide+9] sont censé etre de la mm couleur (Premier zpixel de la diagonale)
    //img->contenu[15*img->rowstride] et img->contenu[15*img->rowstride+3] sont des pixel cote a cote mais ne sont pas censée etre groupé (degradation trop eleve au dessus de taille 2)
    return 0;
}