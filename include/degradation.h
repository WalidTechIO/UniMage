#ifndef DEGRADATION_H
#define DEGRADATION_H

#include <glib.h>
#include <stack2.h>
#include <zpixel.h>

#define MODE_TAILLE 0
#define MODE_LUMINOSITE 1
#define MODE_DISTANCE 2

int degradation(GNode * node, const int mode);

int degradationTaille(my_stack_t *pile);

int degradationLuminosite(my_stack_t *pile);

int degradationDistance(my_stack_t *pile);

void stack_creation(GNode *node, gpointer data);

#endif