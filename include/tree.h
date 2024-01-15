#ifndef ZPIXELTREE_H
#define ZPIXELTREE_H

#define BUILD_MOYENNE 0

#include <zpixel.h>
#include <glib.h>
#include <degradation.h>

/**
 * Structure de données ayant pour but de faciliter le foreach des nodes
*/
typedef struct treedata {
    const unsigned int seuil;
    image * img;
} treedata;


/**
 * Fonction qui construit un arbre
*/
GNode* construire_arbre_zpixel(int x, int y, int taille, image *img, int (*f)(GNode *, my_stack_t *));

/**
 * Fonction récurvise qui projete sur une image un noeud et tout ses enfants selon un seuil de degradation
 */
void parcourir_arbre(GNode *root, gpointer data);

/**
 * Affiche arbre
*/
int affiche_arbre(GNode *root, const unsigned int seuil, image *img);

#endif