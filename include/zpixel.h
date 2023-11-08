#ifndef ZPIXEL_H
#define ZPIXEL_H

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CARRE(x) ((x) * (x))

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <utils.h>


/**
* Défini un point 2D
*/
typedef struct point {
    int x, y;
} point;

/**
* Défini une couleur rgb
*/
typedef struct rgbcolor {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} rgbcolor;

/**
* Défini un zpixel
*/
typedef struct zpixel {
    unsigned int size;
    point pos;
    rgbcolor color;
    int degradation;
} zpixel;

/**
* Cree un Zpixel de taille size de position pos et de couleur color
*/
zpixel * creerZpixel(unsigned int size, point pos, rgbcolor color);

/**
* Cree un Zpixel de taille size de position pos et de couleur noire
*/
zpixel * creerZpixelNoir(int size, point pos);

/**
* Cree un Zpixel de taille size de position pos et de couleur blanche
*/
zpixel * creerZpixelBlanc(int size, point pos);

/**
* Calcul le min/max d'une couleur et les renvoie par les pointeurs en parametres
*/
void minmaxrgb(rgbcolor color, unsigned char *min, unsigned char *max);

/**
* Calcul de la luminosité d'un zpixel
*/
unsigned char luminosite(zpixel * ptr);

/**
* Calcul de la saturation d'un zpixel
*/
unsigned char saturation(zpixel * ptr);

/**
* Calcul de la distance cartésienne entre les couleurs de 2 zpixel
*/
unsigned int distance(zpixel * ptr1, zpixel * ptr2);

/**
* Projette un zpixel a sa position sur une image
*/
int projeter(zpixel * ptr, image * img);

/**
 * Affiche le zpixel a l'adresse ptr
 */
int afficher_zpixel(zpixel *);

#endif