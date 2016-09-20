#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <unistd.h>
#define BSIZE 1024

int main ( /*int argc , char ** argv*/ ){
  int s=creer_serveur(8080);
  int n=0;
  char b[BSIZE];
  while(1){
    int a=accept(s,NULL,NULL);
  
    write(a,"Bienvenue sur notre serveur\n",28);
  
    while ((n=read(a,b,BSIZE))>0){
      write(1,b,n);
      write(a,b,n);
      sleep(1);
    }
  }
  return 0;
}
