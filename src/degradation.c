#include <degradation.h>

void putInStack(GNode * node, gpointer data){
    my_stack_t * stack = (my_stack_t *) data;
    stack_push(stack, node);
}

int degradation(GNode *node, my_stack_t *stack, DegradationFunction f)
{
    if(node == NULL || stack == NULL || f == NULL){
        return -1;
    }
    g_node_children_foreach(node, G_TRAVERSE_ALL, putInStack, stack);
    return f(node, stack); //Appel fonction de degradation avec le noeud et ses enfants dans la pile
}

int degradationTaille(GNode * node, my_stack_t *pile){
    zpixel *parent = (zpixel *) node->data;
    return parent->size;
}

int degradationLuminosite(GNode *node, my_stack_t *pile){
    GNode * children;
    int lumax = 0;
    //On recherche le zpixel le plus lumineux parmis les enfants
    while((children = stack_pop(pile)) != NULL){
        int luminosite_zone  = luminosite((zpixel *)children->data);
        if(luminosite_zone > lumax){
            lumax = luminosite_zone;
        } 
    }
    return lumax;
}

int degradationDistance(GNode *node, my_stack_t *pile){
    zpixel * parent = (zpixel *) node->data;
    unsigned int dist = 0, count = 0;
    GNode * children;
    while((children = stack_pop(pile)) != NULL){
        zpixel * child = (zpixel *) children->data;
        dist += distance(parent, child);
        count++;
    }
    dist /= count;
    dist *= (parent->size-1);
    return dist;
}