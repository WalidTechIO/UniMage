/* ---------------------------------------------------
 * Entete et macro pour la gestion de pile
 * ---------------------------------------------------
 * 04/11/08	SR	creation 
 * 18/10/10	SR	changement STACK_ITERATOR (départ
 *			  du sommet de pile)
 * 19/10/19	SR	changement commentaire macro STACK_CAPACITY
 * 20/10/19	SR	version public sans macro
 * ---------------------------------------------------
 */
#ifndef _STACK2_H_
#define _STACK2_H_

struct _stack_t;
typedef struct _stack_t my_stack_t;

struct _stack_iterator_t;
typedef struct _stack_iterator_t stack_iterator_t;

 
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
 * Vide une pile de void *
 * ---------------------------------------------------
 * stack        : pile (my_stack_t *)
 * ---------------------------------------------------
 * ----------------------------------------------------
 */
void stack_init(my_stack_t * stack);

/* ---------------------------------------------------
 * empile une valeur 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * valeur	: valeur à mettre dans la pile
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


/* ---------------------------------------------------
 * depile une valeur de type void *
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * var		: objet affectable ou sera enregistree la
 *                   valeur depilee
 * type		: type de l'element a depiler
 * ---------------------------------------------------
 * retourne -1 si la pile est vide, 0 sinon
 * si la pile est vide, la valeur de "value" n'est pas changee
 * ----------------------------------------------------
 */
int stack_pop2( my_stack_t * stack, void ** value  );
	

/* ---------------------------------------------------
 * parcours iteratif du contenu de la pile
 * creation d'un iterateur (commence par le sommet de la pile)
 * ---------------------------------------------------
 * stack        : pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne un iterateur, NULL en cas d'echec
 * ----------------------------------------------------
 */
stack_iterator_t * stack_iterator_create(my_stack_t * stack);


/* ---------------------------------------------------
 * parcours iteratif du contenu de la pile
 * liberation d'un iterateur
 * ---------------------------------------------------
 * it   : iterateur de pile
 * ---------------------------------------------------
 * ----------------------------------------------------
 */
void stack_iterator_free(stack_iterator_t * it);


/* ---------------------------------------------------
 * parcours iteratif du contenu de la pile
 * acces aux elments
 * ---------------------------------------------------
 * it   : iterateur de pile (stack_iterator_t *)
 * ---------------------------------------------------
 * retourne une valeur enregistree dans la pile, et NULL si
 *  il n'y a plus de valeur disponible
 * ----------------------------------------------------
 */
void * stack_iterator_next(stack_iterator_t * it);


/* ---------------------------------------------------
 * parcours iteratif du contenu de la pile 
 * acces aux elments
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * value	: valeur lue (PS pointeur sur une variable 
 *			qui contiendra le résultat)
 * ---------------------------------------------------
 * retourne 0 si le parcours est termine sinon 1
 * ----------------------------------------------------
 */
int stack_iterator_next2(stack_iterator_t * it, void ** value);


/* ---------------------------------------------------
 * capacity max de la pile
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne la capacity max de la pile
 * ----------------------------------------------------
 */
int stack_capacity(const my_stack_t * stack);


/* ---------------------------------------------------
 * teste si la pile est vide 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne 1 si la pile est vide, 0 sinon
 * ----------------------------------------------------
 */
int stack_is_empty(const my_stack_t * stack);


/* ---------------------------------------------------
 * teste si le pile n'est pas vide 
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne 1 si la pile n'est pas vide, 0 sinon
 * ----------------------------------------------------
 */
int stack_has_value_available(const my_stack_t * stack);


/* ---------------------------------------------------
 * nombre de places libres
 * ---------------------------------------------------
 * stack 	: stack (my_stack_t *)
 * ---------------------------------------------------
 * retourne le nombre de places libres
 * ----------------------------------------------------
 */
int stack_mem_available(const my_stack_t * stack);


/* ---------------------------------------------------
 * nombre de places occupes
 * ---------------------------------------------------
 * stack 	: pile (my_stack_t *)
 * ---------------------------------------------------
 * retourne le nombre de places occupees
 * ----------------------------------------------------
 */
int stack_mem_used(const my_stack_t * stack);


#endif
