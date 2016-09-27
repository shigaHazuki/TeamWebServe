#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "Main.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#define BSIZE 1024

int main ( /*int argc , char ** argv*/ ){
  int s=creer_serveur(8080);
  int n=0;
  char b[BSIZE];
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
      
    
    initialiser_signaux();

  
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
void initialiser_signaux ( void ){
  if (signal(SIGPIPE, SIG_IGN) ==SIG_ERR){
    perror("signal");
  }
}
