#include <stdio.h>
#include <signal.h>
#include "signals.h"


int WAITING_LIST[SIGNALS];
int PRIORITY[SIGNALS];
int CURRENT_PRIORITY;

void disable_interrupts (void) {
   for(int i=0; i < SIGNALS; i++)
      sighold (SIGNAL_TYPES[i]);
   }

void enable_interrupts (void) {
   for(int i=0; i < SIGNALS; i++)
      sigrelse (SIGNAL_TYPES[i]);
   }

void simmulate_interrupt_routine(int interrupt){

  for(int job_stage = 0; job_stage <= 5; job_stage++) print_job_stage(job_stage);
      for(int iter = 0; iter < interrupt; iter++) printf ("- ");
      printf("%d ", stage);
      for(int iter = SIGNALS; iter > interrupt; iter--) printf ("- ");
      sleep (1);
      
  }

void interrupt_handler(int sig){
  int n,x,k;
  disable_interrupts();
  print_recieved_interrupt(sig);

  WAITING_LIST[n]++;
  do{
    x=0;
    for(k=1;k<6;k++)
       if((WAITING_LIST[k]!=0) && (k>CURRENT_PRIORITY))
            x=k;
    if (x>0){
      WAITING_LIST[x]--;
      PRIORITY[x] = CURRENT_PRIORITY;
      CURRENT_PRIORITY = x;
      enable_interrupts();
      simmulate_interrupt_routine(x);
      disable_interrupts();
    }
  } while ( x > 0);
  
  CURRENT_PRIORITY = PRIORITY[x];
  enable_interrupts();
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
void print_recieved_interrupt(int interrupt_type) {

  int index = find_array_index (interrupt_type, SIGNAL_TYPES, SIGNALS);
  if (index != -1) {
    for(int i = 0; i <= index; i++) printf("- ");
    printf("X ");
    for(int i = SIGNALS; i > index; i--) printf("- ");
  }

}

int find_array_index (int value, int a[], int num_elements) {
  for (int i=0; i<num_elements; i++) {
    if (a[i] == value) {
      return(value);
    }
  }
  
  return(-1);
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
