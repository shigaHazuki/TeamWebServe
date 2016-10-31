#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "Main.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#define BSIZE 1024
#include <dirent.h>
#ifndef WIN32
    #include <sys/types.h>
#endif

//Exécution du programme
int main (int argc , char ** argv){

  //vérifions d'abord si l'utilisateur donne un chemin d'accés valide
  if(verifRepertoireValide(argv[1])==-1||argc!=2){
    exit(EXIT_FAILURE);
  }
  int s=creer_serveur(8080);
  char b[BSIZE];
  initialiser_signaux();
  while(1){
    int a=accept(s,NULL,NULL);
    FILE* f = fdopen(a, "w+");
  
    if(f==NULL){
      fprintf(stderr,"%s","erreur ouverture f");
      exit(EXIT_FAILURE);
    }
    pid_t pid;
    pid = fork();
    fprintf(stdout,"%d",pid);
    if((pid == -1)){
      close(a);
    }
    else if(pid>0){
      close(a);
    }
    else if(pid==0){
      char * s = fgets_or_exit(b,BSIZE,f);
      // http_request *req=NULL;

      
      int bool=parse_http_request(s/*,req*/);
      skip_headers(s,b,f);

      char * status=NULL;

      /* if (bad_request){
	 send_response ( client , 400 , " Bad Request " , " Bad request \ r \ n " );
	 }
	 else if ( request . method == HTTP_UNSUPPORTED ){
	 send_response ( client , 405 , " Method Not Allowed " , " Method Not Allowed \ r \ n " );
	 }
	 else if ( strcmp ( request . target , " / " ) == 0){
	 send_response ( client , 200 , " OK " , motd );
	 }
	 else{
	 send_response ( client , 404 , " Not Found " , " Not Found \ r \ n " );
	 }*/

      int fd=check_and_open ( "/Test.txt",argv[1]);
      int dest=check_and_open ("/webserver",argv[1]);
      if(fd==-1){
	send_status(f,404,status);
	close(a);
	exit(0);
      }else{
       	send_status(f,200,status);
	copy(fd,dest);
	close(a);
	exit(0);
      }
      
      if(bool==1){
	send_status(f,200,status);
	close(a);
	exit(0);
      }else if(bool==-2){
	send_status(f,404,status);
	close(a);
	exit(0);
	
      }else if(bool==-1){
      	send_status(f,400,status);
	close(a);
	exit(0);
      }
    }
  }
  return 0;
}
//envoyer la reponse au client 
/*void send_response ( FILE * client , int code , const char * reason_phrase ,
  const char * message_body ){
  }*/

//detecte le status de la requete 
void send_status ( FILE * client , int code , const char * reason_phrase ){
  if(code==200){
    reason_phrase="HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length:15 \r\n\r\n200 Bienvenue\r\n";
  }else if(code==404){
    reason_phrase="HTTP/1.1 404 not found\r\nConnection: close\r\nContent-Length:15 \r\n\r\n404 not found\r\n";
  }else if(code==400){
    reason_phrase="HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length:17 \r\n\r\n400 Bad request\r\n";
  }
  fprintf(client,"%s",reason_phrase);
  fflush(client);
}
//Analyse des entetes
void skip_headers (char * s,char buffer [], FILE * client ){

  while (strncmp(s,"\r\n",2)!=0){
    fprintf(stdout, "<TeamWebServ>%s", buffer);
    s=fgets_or_exit(buffer,BSIZE,client);
  }
}
//détecte lorsqu'un client quitte le serveur -> permet de supprimer les fils zombie
void traitement_signal ( int sig ){
  printf ( " Signal %d reçu\n " , sig );
  while (waitpid(-1,NULL,WNOHANG) >0){}
}

//initialisation de l'ensemble des signaux: celui qui nous interesse detecter lorsqu'un client quitte le serveur
void initialiser_signaux ( void ){

  struct sigaction sa ;
  
  sa.sa_handler = traitement_signal ;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART ;
  
  if ( sigaction ( SIGCHLD , & sa , NULL ) == -1){
    perror ( " sigaction ( SIGCHLD ) " );
  }
}

//lecture de la requete et arret si erreur de lecture
char * fgets_or_exit ( char * buffer , int size , FILE * stream ){
  char *s=fgets(buffer,size,stream);
  if(ferror(stream)){
    perror("erreur de lecture");
    exit(EXIT_FAILURE);
  }
  return s;
}
//vérification de la validité de la requete 
int parse_http_request (char * chaine/*, http_request * request*/){

  //gestion de la fin de la premier phrase
  int size = strlen(chaine);
  if(chaine[size - 2] != '\r' || chaine[size - 1] != '\n'){
    // request -> method = HTTP_UNSUPPORTED;
    return -1;
  }

  char *token;
  token=strtok(chaine," ");
  int i=0;

  while(token!=NULL){

    //verification de la methode
    if(i==0){
      if(strcmp(token,"GET")!=0){
	//	request -> method = HTTP_UNSUPPORTED;
	return -1;
	/*	}else{
		request -> method =HTTP_GET;*/
	}
    }
    //verification du contenu
    if(i==1){
      if(strcmp(token,"/")!=0){
	//	request -> method = HTTP_UNSUPPORTED;
	return -2;
	/*	}else{
		request -> target=token;*/
       }
    }
    //Verification du protocole
    if(i==2){
      if((strncmp(token,"HTTP/1.1",8)!=0) && (strncmp(token,"HTTP/1.0",8)!=0)){
	//	request -> method= HTTP_UNSUPPORTED;
	return -1;
	/* }else{ 
	 if(strncmp(token,"HTTP/1.1",8)==0){
	   request -> minor_version=1;
	   request -> minor_version=1;
	 }
	 else if(strncmp(token,"HTTP/1.0",8)==0){
	   request -> major_version=1;
	   request -> minor_version=0;
	   }*/
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
//capture d'une chaine jusqu'au ?
char *rewrite_target (char *chaine){
   char *new_chaine = NULL;
   int fin;
   int size=strlen(chaine);
   int deb=0;
   int i;
   for(i=0;i<size;i++){
    if(chaine[i]=='?'){
      fin=i-1;
    }
   }
   if (chaine != NULL && deb < fin){

      new_chaine = malloc (sizeof (*new_chaine) * (fin - deb + 2));
      if (new_chaine != NULL){

         int i;
         for (i = deb; i <= fin; i++){
            new_chaine[i-deb] = chaine[i];
         }
         new_chaine[i-deb] = '\0';
      }
      else
      {
         fprintf (stderr, "Memoire insuffisante\n");
         exit (EXIT_FAILURE);
      }
   }
   return new_chaine;
}
//verifie si le répertoire est valide ou non (OUi=1, NOn=-1)
int verifRepertoireValide(char* chaine){
  DIR *rep=NULL;
   rep=opendir(chaine);
  if(rep!=NULL){
      // S'il y a eu un souci avec la fermeture
    if (closedir(rep) == -1){
      exit(EXIT_FAILURE);
    }
    return 1;
  }else{
    perror("");
    return -1;
  }
}
//ouverture du fichier retourne -1 s'il y a une erreur, retourne le descripteur de fichier dans le cas contraire
int check_and_open ( char * target , char * document_root ){
  char* dir=strcat(document_root,target);

  int fichier  = open(dir,O_RDWR);
  if (fichier != -1){
    return fichier;
  }else{
    perror("");
    
    return -1;
  }
}
//retourne la taille du fichier en octet
int get_file_size(int fd){
  struct stat *buff=NULL;
  fstat(fd,buff);
  return buff -> st_size;
}

//copie un fichier depuis un descripteur in vers un descripteur out 
int copy(int in, int out){
  char buf [BSIZE];
  int n=read(in,buf,BSIZE);
  int w=0;
  if(n==-1){
    perror("");
    return -1;
  }
  while ((n=read(in,buf,BSIZE))>0){
    w=write(out,buf,BSIZE);
  }
  if(w==-1){
    perror("");
    return -1;
  }
  return 1;
}
