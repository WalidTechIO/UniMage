#include <degradation.h>

int degradation(GNode *node, my_stack_t *stack, int (*f)(GNode *, my_stack_t *))
{
    if(node == NULL || stack == NULL || f == NULL){
        return -1;
    }
    GNode * children = node->children;
    if(children != NULL){
        stack_push(stack, children);
    }
    while((children = children->next)){
        if(children != NULL){
            stack_push(stack, children);
        }
    }
    if(f == NULL){
        return -1;
    }
    return f(node, stack); //Appel fonction de degradation avec le noeud et ses enfants dans la pile
}

int degradationTaille(GNode * node, my_stack_t *pile){
    zpixel *parent = (zpixel *) node->data;
    return parent->size-1;
}

int degradationLuminosite(GNode *node, my_stack_t *pile){
    zpixel *parent = (zpixel *) node->data;
    return 255-(luminosite(parent));
}

int degradationDistance(GNode *node, my_stack_t *pile){
    zpixel * parent = (zpixel *) node->data;
    unsigned int dist = 0, count = 0;
    GNode * children;
    while((children = stack_pop(pile))){
        zpixel * child = (zpixel *) children->data;
        dist += distance(parent, child);
        count++;
    }
    dist /= count;
    return dist;
}