#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "Main.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#define BSIZE 1024

int main ( /*int argc , char ** argv*/ ){
  int s=creer_serveur(8080);
  int n=0;
  char b[BSIZE];
  initialiser_signaux();
  while(1){
    int a=accept(s,NULL,NULL);

    write(a,"Bienvenue sur notre serveur\n",28);
    
    pid_t pid;

    pid = fork();
    if((pid == -1)){
      close(a);
    }
    else if(pid>0){
      close(a);
    }
    else if(pid==0){
    
    while ((n=read(a,b,BSIZE))>0){
      
      write(1,b,n);
      write(a,b,n);
      sleep(1);
    }
    close(a);
    exit(0);
    }
  }
  return 0;
}
void traitement_signal ( int sig ){
  printf ( " Signal %d reçu\n " , sig );
  while (waitpid(-1,NULL,WNOHANG) >0){}
}
void initialiser_signaux ( void ){

  struct sigaction sa ;
  
  sa.sa_handler = traitement_signal ;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART ;
  
  if ( sigaction ( SIGCHLD , & sa , NULL ) == -1){
    perror ( " sigaction ( SIGCHLD ) " );
  }
}
