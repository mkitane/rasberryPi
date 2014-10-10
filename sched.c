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

	__asm("mov sp, %0" : : "r"((*current_ctx).sp));
	__asm("mov lr, %0" : : "r"((*current_ctx).lr));
    __asm("pop {r0-r12}"); 

	//Sauter a ladresse de retour du contexte restaure
	__asm("bx lr"); 	
}


void init_pcb(struct pcb_s* pcb, func_t f, void* args, unsigned int stack_size){
	(*pcb).id_process = compteur; 
	compteur++; 
	(*pcb).state_process = PRET; 
	(*pcb).adress = f; 
	(*pcb).sp = ((int)phyAlloc_alloc(stack_size)) + stack_size;
	(*pcb).args_f = args;

}
int create_process(func_t f, void* args, unsigned int stack_size)
{
	struct pcb_s *pcb = (struct pcb_s*)phyAlloc_alloc(sizeof(struct pcb_s));

	last_process->next_process = pcb; 
	pcb->next_process = first_process;
	last_process = pcb; 

	init_pcb(pcb,f,args, stack_size);

	return pcb->id_process;
}

void start_current_process()
{
	current_process->state_process = EXECUTION ; 
	current_process->adress(current_process->args_f); 
}
void start_sched()
{
}
void ctx_switch()
{
}

