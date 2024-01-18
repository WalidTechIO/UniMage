#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/**
 * Défini une image
 */
typedef struct image{
    unsigned int largeur, hauteur, rowstride;
    unsigned char *contenu;
} image;

/**
 * Affiche une image au format textuelle avec les lignes/colonnes et le gap au format hexa chaque pixel est représenté dans le quadrillage par sa couleur RGB
 */
int afficher_image(image *);
/**
 * Genere un fichier bitmap de nom filename correspondant a l'image
 */
int createBitmapFile(const char *, image *);
/**
 * Genere une image de width*height avec un gap valide pour du BMP et des pixels de couleur noire
*/
image * creerImage(unsigned int width, unsigned int height);
/**
 * Enregistre un pixbuf dans notre format d'image
*/
image *creerImagePixbuf(GdkPixbuf *);
/**
 * Detruit un arbre de GNode en profondeur
*/
void detruire_arbre(GNode **);

#endif