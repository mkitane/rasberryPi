#include "sched.h"
#include "hw.h"

void funcA() {
	int cptA = 0;
	while ( 1 ) {
		cptA ++;
		ctx_switch();
	}
}
void funcB() {
	int cptB = 1;
	while ( 1 ) {
		cptB += 2 ;
		ctx_switch();
	}
}

void funcC()
{
	int cptC = 2;
	int i = 0;
	for(i=0; i<5 ; i++)
	{
		cptC += 3 ;
		ctx_switch();
	}

}
//------------------------------------------------------------------------
int kmain ( void){
	init_hw();
	create_process(funcB, NULL, STACK_SIZE);
	create_process(funcA, NULL, STACK_SIZE);
	start_sched();
	//ctx_switch();
	while(1){}
/* Pas atteignable vues nos 2 fonctions */
	return 0;
}
