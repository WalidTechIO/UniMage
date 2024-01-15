#include <stack2.h>

my_stack_t * stack_create(int nbMaxElement){
    my_stack_t* stack = (my_stack_t*) malloc(sizeof(my_stack_t));
    stack->max = nbMaxElement;
    stack->nb = 0;
    stack->values = (void**) malloc(sizeof(void*));
    return stack;
}

void stack_remove(my_stack_t * stack) {
    free(stack->values);
    free(stack);
}

int stack_push(my_stack_t * stack, void *valeur) {
    if(stack->nb+1<=stack->max && valeur != NULL){
        stack->nb++;
        stack->values = (void**) realloc(stack->values, stack->nb*sizeof(void*));
        stack->values[stack->nb-1] = valeur;
        return 0;
    } else {
        return -1;
    }
}

void* stack_pop(my_stack_t * stack) {
    if(stack->nb==0){
        return NULL;
    } else {
        stack->nb--;
        void * res = stack->values[stack->nb];
        stack->values = (void**) realloc(stack->values, stack->nb*sizeof(void*));
        return res;
    }
}