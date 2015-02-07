#include <stdio.h>
#include <signal.h>
#define N 6

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;
int sig[]={SIGUSR1, SIGUSR2, SIGTERM, SIGQUIT, SIGINT};

void zabrani_prekidanje(){
   int i;
   for(i=0; i<5; i++)
      sighold(sig[i]);
   }

void dozvoli_prekidanje(){
   int i;
   for(i=0; i<5; i++)
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
void register_interrupts(){
   sigset (SIGUSR1, interrupt_handler);
   sigset (SIGUSR2, interrupt_handler);
   sigset (SIGTERM, interrupt_handler);
   sigset (SIGQUIT, interrupt_handler);
   sigset (SIGINT, interrupt_handler);
}

int main ( void ){
  int iter;
  register_interrupts();

  printf(">>PID=%d\n", getpid());
  printf("GP S1 S2 S3 S4 S5\n");
  printf("-----------------\n");
  
  for(j=0; j < 40; j++){
     printf("%d - - - - -\n", j % 10 );
     sleep(1);
  }

  printf (">>Process ended.\n");
return 0;
}