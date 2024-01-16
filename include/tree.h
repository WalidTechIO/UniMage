#ifndef ZPIXELTREE_H
#define ZPIXELTREE_H

#define BUILD_MOYENNE 0

#include <zpixel.h>
#include <glib.h>
#include <degradation.h>

/**
 * Fonction qui construit un arbre
*/
GNode* construire_arbre_zpixel(int, int, int, image *, DegradationFunction);

/**
 * Affiche arbre
*/
int affiche_arbre(GNode *, const unsigned int, image *);

#endif