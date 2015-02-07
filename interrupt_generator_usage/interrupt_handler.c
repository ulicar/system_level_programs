#include <stdio.h>
#include <signal.h>
#include "signals.h"

#define N SIGNALS+1

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

void disable_interrupts (void) {
   for(int i=0; i < SIGNALS; i++)
      sighold ( sig[i] );
   }

void enable_interrupts (void) {
   for(int i=0; i < SIGNALS; i++)
      sigrelse(sig[i]);
   }

void obrada_prekida(int i){
       int j;
      switch(i){
      case 1:for(j=1;j<6;j++){
             printf("- %d - - - -\n",j);
             sleep(1);}
           break;
      case 2:for(j=1;j<6;j++){
             printf("- - %d - - -\n",j);
             sleep(1);}
           break;
      case 3:for(j=1;j<6;j++){
             printf("- - - %d - -\n",j);
             sleep(1);}
           break;
      case 4:for(j=1;j<6;j++){
             printf("- - - - %d -\n",j);
             sleep(1);}
           break;
      case 5:for(j=1;j<6;j++){
             printf("- - - - - %d\n",j);
             sleep(1);}
           break;
      }
  }

void interrupt_handler(int sig){
  int n,x,k;
  zabrani_prekidanje();
   switch(sig){
      case SIGUSR1:
           n=1;
           printf("- X - - - -\n");
           break;
      case SIGUSR2:
           n=2;
           printf("- - X - - -\n");
           break;
      case SIGTERM:
           n=3;
           printf("- - - X - -\n");
           break;
      case SIGQUIT:
           n=4;
           printf("- - - - X -\n");
           break;
      case SIGINT:
           n=5;
           printf("- - - - - X\n");
           break;
      }

  OZNAKA_CEKANJA[n]++;
  do{
       x=0;
       for(k=1;k<6;k++)
           if((OZNAKA_CEKANJA[k]!=0) && (k>TEKUCI_PRIORITET))
                x=k;
  if (x>0){
           OZNAKA_CEKANJA[x]--;
           PRIORITET[x] = TEKUCI_PRIORITET;
           TEKUCI_PRIORITET= x;
           dozvoli_prekidanje();
           obrada_prekida(x);
           zabrani_prekidanje();
          
           }
  }while(x>0);
  TEKUCI_PRIORITET = PRIORITET[x];
           dozvoli_prekidanje();


}

void register_interrupts (void) { 
  for(int i = 0; i <= SIGNALS; i++) { 
    sigset (SIGNAL_TYPES[i], interrupt_handler);
  }
}

void simulate_main_program_execution (void) {
  printf("%d", iter % 10 );
  for(int i = 0; i <= SIGNALS; i++) printf(" - ");
  printf("\n");
  
  sleep(1);
}

void print_main_info (void) {
  printf(">>PID=%d\n", getpid());
  printf("GP");
  for(int i = 0; i <= SIGNALS; i++) printf("S%d", i);
  printf("\n");
  for(int i = 0; i <= SIGNALS; i++) printf("---");
  printf("\n");
}

int main (void) {
  register_interrupts();
  print_main_info();
  
  for(int iter = 0;; iter < 40; iter++) {
     simulate_main_program_execution();
  }

  printf (">>Process ended.\n");
  return 0;
}
