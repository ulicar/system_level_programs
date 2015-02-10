#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include "signals.h"
#include <unistd.h>


int WAITING_LIST[SIGNALS + 1];
int PRIORITY[SIGNALS];
int CURRENT_PRIORITY;

int find_array_index (int value, int a[], int num_elements);
int get_top_priority_signal(void);


void disable_interrupts (void) {
   for(int i=0; i < SIGNALS; i++)
      sighold (SIGNAL_TYPES[i]);
   }

void enable_interrupts (void) {
   for(int i=0; i < SIGNALS; i++)
      sigrelse (SIGNAL_TYPES[i]);
   }

void print_recieved_interrupt(int sig) {
  int index = find_array_index(sig, SIGNAL_TYPES, SIGNALS);
  if (index != -1) {
    for(int i = 0; i <= index; i++) printf("-  ");
    printf("X  ");
    for(int i = SIGNALS - 1; i > index; i--) printf("-  ");
    printf("\n");
  }
  sleep(1);

}
void print_job_stage(int job_stage, int interrupt){
  for(int iter = 0; iter <= interrupt; iter++) printf ("-  ");
    printf("%d  ", job_stage);
    for(int iter = SIGNALS - 1; iter > interrupt; iter--) printf ("-  ");
    printf("\n");
    sleep (1);
}

void simmulate_interrupt_routine(int interrupt){

  for(int job_stage = 0; job_stage <= 5; job_stage++) 
    print_job_stage(job_stage, interrupt);
      
  }

void interrupt_handler(int sig){
  int curr_interrupt, priority_signal;
  disable_interrupts();
  print_recieved_interrupt(sig);

  curr_interrupt = find_array_index (sig, SIGNAL_TYPES, SIGNALS);
  if (curr_interrupt == -1) return;
  
  WAITING_LIST[curr_interrupt]++;
  do{
    priority_signal = get_top_priority_signal();
            
    if (priority_signal > 0){
      WAITING_LIST[priority_signal]--;
      PRIORITY[priority_signal] = CURRENT_PRIORITY;
      CURRENT_PRIORITY = priority_signal;
      enable_interrupts();
      simmulate_interrupt_routine(priority_signal);
      disable_interrupts();
    }
  } while (priority_signal > 0);
  
  CURRENT_PRIORITY = PRIORITY[priority_signal];
  enable_interrupts();
}
int get_top_priority_signal(){
  int top_priority = 0;
  for(int signal = 0; signal < SIGNALS; signal++)
    if((WAITING_LIST[signal] != 0) && (signal > CURRENT_PRIORITY))
      top_priority = signal;
  
  return top_priority;
}

void register_interrupts (void) { 
  for(int i = 0; i <= SIGNALS; i++) { 
    sigset (SIGNAL_TYPES[i], interrupt_handler);
  }
}

void simulate_main_program_execution (int iter) {
  printf("%d ", iter % 10 );
  for(int i = 0; i <= SIGNALS; i++) printf(" - ");
  printf("\n");
  
  sleep(1);
}



int find_array_index (int value, int a[], int num_elements) {
  for (int i=0; i<num_elements; i++) {
    if (a[i] == value) {
      return(i);
    }
  }
  
  return(-1);
}

void print_main_info (void) {
  printf(">>PID = %d\n", getpid());
  printf("GP ");
  for(int i = 0; i < SIGNALS; i++) printf("S%d ", i);
  printf("\n");
  for(int i = 0; i <= SIGNALS + 1; i++) printf("---");
  printf("\n");
}

int main (void) {
  register_interrupts();
  print_main_info();
  
  for(int iter = 0; iter < 40; iter++) {
     simulate_main_program_execution(iter);
  }

  printf ("\n>>Process ended.\n");
  return 0;
}
