#include <stdio.h>
#define _XOPEN_SOURCE 500
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int SIGNAL_TYPES[]={SIGUSR1, SIGUSR2, SIGTERM, SIGQUIT, SIGINT};

void interrupt_handler(int sig){
   kill(sig, SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]){
    if (argc != 2) { 
        fprintf(stderr, "USAGE: %s <PID> \n", argv[0]);
        exit(-1);
    }

    int pid = atoi(argv[1]);
    sigset(SIGINT, interrupt_handler);
    srand(time(NULL));
    int sleep_interval = rand() % 2 + 3;

    int signal_index;
    while(1){
      sleep(sleep_interval);
      signal_index = rand() % 4;
      printf(">> %d", signal_index);
      kill(pid, SIGNAL_TYPES[signal_index]);
    }

  return 0;
}

