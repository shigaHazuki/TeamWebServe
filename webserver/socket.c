#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/types.h>


//création du serveur qui écoute sur le port indiqué en parametre
int creer_serveur(int port){
	int socket_serveur;
	
	socket_serveur= socket(AF_INET , SOCK_STREAM,0);
	if (socket_serveur == -1){
		
		perror("socket_serveur");
	}
	
	
	saddr.sin_family = AF_INET ; /* Socket ipv4 */
	saddr.sin_port = htons (port); /* Port d 'ecoute */
	saddr.sin_addr.s_addr = INADDR_ANY ; /* ecoute sur toutes les interfaces */

	int optval = 1;
	if ( setsockopt ( socket_serveur , SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1){
	  perror ( " Can not set SO_REUSEADDR option " );
	}
	
	if ( bind ( socket_serveur , ( struct sockaddr *)& saddr , sizeof ( saddr )) == -1){
		perror ( " bind socker_serveur " ); /* traitement de l ' erreur */
		return socket_serveur;
	}
	
	if ( listen ( socket_serveur , 10) == -1){
		perror ( " listen socket_serveur " ); /* traitement d ' erreur */
		return socket_serveur;
	}
	
	return socket_serveur;
}
