#include "sched.h"



void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size){

	(*ctx).lr = f; 
	(*ctx).sp = phyAlloc_alloc(stack_size);

}


void __attribute__ ((naked)) switch_to(struct ctx_s* ctx)
{
	//Sauvegarder contexte courant
	__asm("mov %0, sp" : "=r"((*current_ctx).sp) );
	__asm("mov %0, lr" : "=r"((*current_ctx).lr) );

	//Changer de contexte courant
	current_ctx = ctx; 
	
	//Restaurer ce nouveau contexte
	__asm("mov sp, %0" : : "r"((*current_ctx).sp));
	__asm("mov lr, %0" : : "r"((*current_ctx).lr));

	//Sauter a ladresse de retour du contexte restaure
	__asm("bx lr"); 	
}
