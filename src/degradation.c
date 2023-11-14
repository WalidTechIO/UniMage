#include <degradation.h>

int degradation(GNode * node, int (*f)(my_stack_t *), my_stack_t * stack){
    g_node_children_foreach(node, G_TRAVERSE_ALL, stack_creation, stack);
    switch(mode) {
        case MODE_TAILLE:
            return degradationTaille(stack);

        case MODE_LUMINOSITE:
            return degradationLuminosite(stack);

        case MODE_DISTANCE:
            return degradationDistance(stack);

        default:
            return -1;
    }
}

int degradationTaille(my_stack_t *pile){
    zpixel * pixel = (zpixel *) stack_pop(pile);
    return pixel->size*2-1;
}

int degradationLuminosite(my_stack_t *pile){
    return 0;
}

int degradationDistance(my_stack_t *pile){
    return 0;
}

void stack_creation(GNode * node, gpointer data){
    stack_push(data, node);   
}