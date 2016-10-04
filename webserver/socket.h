# ifndef __SOCKET_H__
# define __SOCKET_H__
#include <netinet/in.h>

/* * Cr�e une socket serveur qui �coute sur toute les interfaces IPv4
de la machine sur le port pass� en param�tre. La socket retourn�e
doit pouvoir �tre utilis�e directement par un appel � accept .
La fonction retourne -1 en cas d erreur ou le descripteur de la
socket cr�e . */

int creer_serveur ( int port );
struct sockaddr_in saddr ;

# endif
