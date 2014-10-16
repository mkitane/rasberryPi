#include "sched.h"
static struct pcb_s* first_process = (struct pcb_s *)0;  
static struct pcb_s* last_process = (struct pcb_s *)0;  
static struct pcb_s* current_process = (struct pcb_s *)0;  


void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size){

	(*ctx).lr = f; 
	(*ctx).sp = ((int)phyAlloc_alloc(stack_size)) + stack_size;
    //on caste en int car sinon on a le void et il peut faire une adition en multipliant le stack_size par 4.
}



void init_pcb(struct pcb_s* pcb, func_t f, void* args, unsigned int stack_size){
	static int compteur = 0; 
	(*pcb).id_process = compteur++; 
	(*pcb).state_process = NEW; 


	(*pcb).function = f; 
	(*pcb).args_f = args;


	(*pcb).adress = (int*)&start_current_process;   //Car start_current_process() est appelée pour
                                                    //lancer un processus (pour la première fois)
	(*pcb).sp = ((int)phyAlloc_alloc(stack_size)) + stack_size;

}
int create_process(func_t f, void* args, unsigned int stack_size)
{
	struct pcb_s *pcb = (struct pcb_s*)phyAlloc_alloc(sizeof(struct pcb_s));

	if(!first_process)
	{
		first_process = pcb; 
		last_process = pcb;
        
        first_process->next = last_process;
        last_process->next = first_process;
	}else{
		last_process->next_process = pcb; 
		pcb->next_process = first_process;
		last_process = pcb; 
	}
	init_pcb(pcb,f,args, stack_size);

	return pcb->id_process;
}

void start_current_process()
{
	current_process->state_process = READY ; 
	current_process->function(current_process->args_f);
	current_process->state_process = TERMINATED;
	ctx_switch();
}

void elect ()
{
    struct pcb_s* pcb = current_process;

    //On trouve le suivant qui n'est pas TERMINATED
    while(pcb->next->state_process != TERMINATED){
        if (pcb->next->id_process == current_process->id_process) { break; }
        pcb = pcb ->next;
    }
    
    //On supprime ceux qui sont terminated
    if(pcb->next->state_process == TERMINATED){  //Q4.9
        struct pcb_s* temp = pcb-> next;
        //throw it out of the list
        pcb->next = pcb->next->next;
        phyAlloc_free(temp,sizeof(struct pcb_s));
    }
    
    
    current_process = pcb->next;
    
}


void start_sched()
{
    current_process->next = first_process;
}



void __attribute__ ((naked)) ctx_switch() //Current process non initialisé?
{
    
    //Sauvegarder contexte courant
    
    //Enregistrer les registres
    __asm("push {r0-r12}");
    __asm("mov %0, sp" : "=r"(current_process->sp));
    __asm("mov %0, lr" : "=r"(current_process->adress));
    
    //elect a new process
    elect();
    
    //Restaurer ce nouveau contexte
    __asm("mov sp, %0" : : "r"(current_process->sp));
    __asm("mov lr, %0" : : "r"(current_process->adress));
    __asm("pop {r0-r12}");
    //Sauter a ladresse de retour du contexte restaure
    __asm("bx lr");
    
}

