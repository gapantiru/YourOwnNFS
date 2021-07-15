
#include "glu_responses.h"


extern int errno;

int parse_request(int client, glu_header * header){

if((send_response(client, header)) == -1)
    return -1;

return 0;

}

int main (){
struct sockaddr_in server;  // structura folosita de server
struct sockaddr_in from;
int sd;     //descriptorul de socket

/* crearea unui socket */
if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
{
  perror ("[server]Eroare la socket().\n");
  return errno;
}

/* pregatirea structurilor de date */
bzero (&server, sizeof (server));
bzero (&from, sizeof (from));

/* umplem structura folosita de server */
/* stabilirea familiei de socket-uri */
server.sin_family = AF_INET;
/* acceptam orice adresa */
server.sin_addr.s_addr = htonl (INADDR_ANY);
/* utilizam un port utilizator */
server.sin_port = htons (GLU_PORT);

if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
perror("setsockopt(SO_REUSEADDR) failed");

/* atasam socketul */
if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
{
  perror ("[server]Eroare la bind().\n");
  return errno;
}

/* punem serverul sa asculte daca vin clienti sa se conecteze */
if (listen (sd, 5) == -1)
{
  perror ("[server]Eroare la listen().\n");
  return errno;
}

while (1){

  int client;
  int length = sizeof (from);

  glu_log ("[server]Asteptam la portul %d...\n",GLU_PORT);
  /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
  client = accept (sd, (struct sockaddr *) &from, &length);

  /* eroare la acceptarea conexiunii de la un client */

  if (client < 0){

      perror ("[server]Eroare la accept().\n");
      //continue;
      break;

    }
  int cid;
  if((cid = fork()) == 0) {
         close(sd);
      do {
          /*se astepta mesajul */
          glu_header header;
          glu_log ("[server]Asteptam request-ul...\n");

          /* citirea mesajului */
          if (read(client, &header, sizeof(glu_header)) != sizeof(glu_header)) {

              perror("[server]Eroare la read() de la client.\n");
              goto finish_session;

          }

          if (parse_request(client, &header) == -1)
              goto finish_session;

      } while (1);

  }
  else if(cid == -1){

      perror("Eroare la fork()!\n");
      return 1;

  }

finish_session:
close (client);
}       /* while */
}       /* main */
