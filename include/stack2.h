/* ---------------------------------------------------
 * Entete et macro pour la gestion de pile
 * ---------------------------------------------------
 * 04/11/08	SR	creation 
 * 18/10/10	SR	changement STACK_ITERATOR (d�part
 *			  du sommet de pile)
 * 19/10/19	SR	changement commentaire macro STACK_CAPACITY
 * 20/10/19	SR	version public sans macro
 * ---------------------------------------------------
 */
#ifndef _STACK2_H_
#define _STACK2_H_

#include <stdlib.h>

typedef struct my_stack_t {
    void** values;
    int nb;
    int max;
} my_stack_t;

/* ---------------------------------------------------
 * creation d'une pile de void *
 * ---------------------------------------------------
 * nbMaxElement	: taille maximum de la pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne un pointeur sur la nouvelle pile
 * ----------------------------------------------------
 */
my_stack_t * stack_create(int nbMaxElement);


/* ---------------------------------------------------
 * destruction d'une pile de void *
 *  Rq : les elements pointes ne sont desalloues
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * ----------------------------------------------------
 */
void stack_remove(my_stack_t * stack);

/* ---------------------------------------------------
 * empile une valeur 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * valeur	: valeur � mettre dans la pile
 * ---------------------------------------------------
 * retourne -1 si la pile est pleine, 0 sinon 
 * ----------------------------------------------------
 */
int stack_push(my_stack_t * stack , void * valeur );


/* ---------------------------------------------------
 * depile une valeur de type void *
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * type		: type de l'element a depiler
 * ---------------------------------------------------
 * retourne la valeur depilee ou NULL en absence de valeur
 * ----------------------------------------------------
 */
void * stack_pop( my_stack_t * stack );

#endif
