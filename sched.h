#define STACK_SIZE 4 //Car pas de variables locales

typedef void (*func_t) ( void);

struct ctx_s
{
	unsigned int sp;
	unsigned int lr;
};

struct ctx_s *current_ctx; 


void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);
void __attribute__ ((naked)) switch_to(struct ctx_s* ctx);


