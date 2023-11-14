#ifndef DEGRADATION_H
#define DEGRADATION_H

#include <glib.h>
#include <stack2.h>
#include <zpixel.h>

#define MODE_TAILLE 0
#define MODE_LUMINOSITE 1
#define MODE_DISTANCE 2

int degradation(GNode *node, my_stack_t *stack, int (*f)(GNode *, my_stack_t *));

int degradationTaille(GNode *node, my_stack_t *pile);

int degradationLuminosite(GNode *node, my_stack_t *pile);

int degradationDistance(GNode *node, my_stack_t *pile);

#endif