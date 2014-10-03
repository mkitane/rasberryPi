#include "sched.h"



void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size){

	(*ctx).lr = f; 
	(*ctx).sp = ((int)phyAlloc_alloc(stack_size)) + stack_size;
    //on caste en int car sinon on a le void et il peut faire une adition en multipliant le stack_size par 4.
}


void __attribute__ ((naked)) switch_to(struct ctx_s* ctx)
{
	//Sauvegarder contexte courant

    //Enregistrer les registres
    __asm("push {r0-r12}"); 
	__asm("mov %0, sp" : "=r"((*current_ctx).sp) );
	__asm("mov %0, lr" : "=r"((*current_ctx).lr) );

	//Changer de contexte courant
	current_ctx = ctx; 
	
	//Restaurer ce nouveau contexte
c
	__asm("mov sp, %0" : : "r"((*current_ctx).sp));
	__asm("mov lr, %0" : : "r"((*current_ctx).lr));
    __asm("pop {r0-r12}"); 

	//Sauter a ladresse de retour du contexte restaure
	__asm("bx lr"); 	
}
