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
    return parent->size;
}

int degradationLuminosite(GNode *node, my_stack_t *pile){
    zpixel *parent = (zpixel *) node->data;
    GNode * children = node->children;
    int lumax = 0;
    //On recherche le zpixel le plus lumineux parmis les enfants
    while((children = stack_pop(pile))){
        zpixel *child = (zpixel *)children->data;
        if(luminosite(child) > lumax){
            lumax = luminosite(child);
        } 
    }
    return lumax;
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
    return dist / count;
}