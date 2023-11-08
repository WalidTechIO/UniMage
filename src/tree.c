#include <tree.h>

GNode *construire_arbre_zpixel(int x, int y, int taille, image * img){
    if(img == NULL || img->contenu == NULL || x > img->largeur || y > img->largeur || taille <= 0){
        return NULL;
    }
    zpixel *pix = creerZpixelNoir(taille, (point) {x,y});
    GNode *node = g_node_new(pix);
    if(taille == 1){
        int index = y *img->rowstride + x * 3;
        rgbcolor color = {img->contenu[index], img->contenu[index+1], img->contenu[index+2]};
        pix->color = color;
    } else {
        int red=0, green=0, blue=0;
        GNode *fgh = construire_arbre_zpixel(x, y, taille/2, img);
        g_node_insert(node, 0, fgh);
        GNode *fdh = construire_arbre_zpixel(x+taille/2, y, taille/2, img);
        g_node_insert(node, 1, fdh);
        GNode *fgb = construire_arbre_zpixel(x, y + taille/2, taille/2, img);
        g_node_insert(node, 2, fgb);
        GNode *fdb = construire_arbre_zpixel(x + taille / 2, y + taille / 2, taille / 2, img);
        g_node_insert(node, 3, fdb);
        zpixel *f1 = (zpixel *) fgh->data;
        zpixel *f2 = (zpixel *) fdh->data;
        zpixel *f3 = (zpixel *) fgb->data;
        zpixel *f4 = (zpixel *) fdb->data;
        red += f1->color.red + f2->color.red + f3->color.red + f4->color.red;
        green += f1->color.green + f2->color.green + f3->color.green + f4->color.green;
        blue += f1->color.blue + f2->color.blue + f3->color.blue + f4->color.blue;
        red /= 4;
        green /= 4;
        blue /= 4;
        pix->color = (rgbcolor) {red, green, blue};
        pix->degradation = (taille - 1) * (1+((int) sqrt(CARRE(x-y))));
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
    if(img->contenu[0] == img->contenu[3*img->rowstride+9] && img->contenu[15*img->rowstride] != img->contenu[15*img->rowstride + 3]){
        return 0;
    } else {
        return -2;
    }
}