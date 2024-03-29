#define STACK_SIZE 1024 //Car pas de variables locales
#include "stdlib.h"
#include "hw.h"

typedef void (*func_t) ( void);

struct ctx_s
{
	unsigned int sp;
	unsigned int lr;
};

struct ctx_s *current_ctx; 


void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);


//Partie 4
enum process_state_s {NEW, READY, RUNNING, WAITING, TERMINATED};
struct pcb_s
{
    unsigned int id_process;
    enum process_state_s state_process;
    unsigned int sp;
    unsigned int adress;

    func_t function;
	void* args_f;

    struct pcb_s* next_process;
};



void init_pcb(struct pcb_s* pcb, func_t f, void* args, unsigned int stack_size);
int create_process(func_t f, void* args, unsigned int stack_size);
void start_current_process();
