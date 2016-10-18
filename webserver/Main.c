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
  char b[BSIZE];
  initialiser_signaux();
  while(1){
    int a=accept(s,NULL,NULL);
    FILE* f = fdopen(a, "w+");
  
    pid_t pid;
    pid = fork();

    if((pid == -1)){
      close(a);
    }
    else if(pid>0){
      close(a);
    }
    else if(pid==0){
      char * s = fgets(b,BSIZE,f);
      int bool=verifGet(s);
      while (strncmp(s,"\r\n",2)!=0){
	fprintf(stdout, "<TeamWebServ>%s", b);
	s=fgets(b,BSIZE,f);
      }
      
      if(bool==1){
       	fprintf(stdout, "<TeamWebServ> La requete est bonne (%s)\n", b);


	fprintf(f,"%s","HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length:15 \r\n\r\n200 Bienvenue\r\n");
	fflush(f);
	
	close(a);
	exit(0);
      }else if(bool==-2){
       	fprintf(f,"%s","HTTP/1.1 404 not found\r\nConnection: close\r\nContent-Length:15 \r\n\r\n404 not found\r\n");
	fflush(f);
	fprintf(stderr,"fergdfggsbgd");
	
      }else if(bool==-1){
   
	fprintf(f,"%s","HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length:17 \r\n\r\n400 Bad request\r\n");
	fflush(f);
	close(a);
	exit(0);
      }
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
/*int verifGet (char * chaine){ 
   int i=0; 
   if(chaine[0]!='G'||chaine[1]!='E'||chaine[2]!='T'){ 
     return -1; 
   } 
   int compteur=0; 
   while(chaine[i]!='\n'){ 
     if(chaine[i]=='/'){ 
       compteur++; 
     } 
     i++; 
   } 
   if(compteur!=2){ 
     return -1; 
   } 
   return 1; 
 }*/

int verifGet (char * chaine){
  char *token;
  token=strtok(chaine," ");
  int i=0;
  while(token!=NULL){
    if(i==0){
      if(strcmp(token,"GET")!=0){
	return -1;
      }
    }
    if(i==1){
      if(strcmp(token,"/")!=0){
	return -2;
      }
    }
    if(i==2){
      if((strncmp(token,"HTTP/1.1",8)!=0) && (strncmp(token,"HTTP/1.0",8)!=0)){
	return -1;
      }
    }
    i++;
    token = strtok(NULL," ");
  }
  if(i!=3){
    return -1;
  }
  return 1;
  }
