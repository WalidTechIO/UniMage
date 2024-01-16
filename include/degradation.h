#ifndef DEGRADATION_H
#define DEGRADATION_H

#include <glib.h>
#include <stack2.h>
#include <zpixel.h>

typedef int (*DegradationFunction)(GNode *, my_stack_t *);

int degradation(GNode *, my_stack_t *, DegradationFunction);

int degradationTaille(GNode *, my_stack_t *);

int degradationLuminosite(GNode *, my_stack_t *);

int degradationDistance(GNode *, my_stack_t *);

#endif