# ifndef __MAIN_H__
# define __MAIN_H__



/*enum http_method {
HTTP_GET ,
HTTP_UNSUPPORTED ,
};

typedef struct {

enum http_method method ;
int major_version ;
int minor_version ;
char * target ;

} http_request ;*/

void send_response ( FILE * client , int code , const char * reason_phrase ,
const char * message_body );
void initialiser_signaux ( void );
void traitement_signal ( int sig );
int parse_http_request(char * chaine/*, http_request * request*/);
char * fgets_or_exit ( char * buffer , int size , FILE * stream );
void skip_headers (char * s,char buffer [], FILE * client );
void send_status ( FILE * client , int code , const char * reason_phrase );




# endif
