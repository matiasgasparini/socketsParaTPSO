#ifndef CLIENTE_SOCKET
#define CLIENTE_SOCKET

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


int main(int argc, char **argv){

  if(argc<2)
  {
    printf("<host> <puerto>\n");
    return 1;
  }

  struct sockaddr_in cliente; //información del cliente

  struct hostent *servidor; //información del servidor

  servidor = gethostbyname(argv[1]); //asigna la ip del servidor según el parámetro

  if(servidor == NULL)
  { //Comprobación
    printf("Host erróneo\n");
    return 1;
  }

  int puerto, conexion;

  char * buffer = malloc(sizeof(char)*1000);

  conexion = socket(AF_INET, SOCK_STREAM, 0); //Asignación del socket
  puerto=(atoi(argv[2])); //conversion del argumento

  bzero(&cliente, sizeof(cliente)); //Rellena toda la estructura de 0

  cliente.sin_family = AF_INET; //asignacion del protocolo
  cliente.sin_port = htons(puerto); //asignacion del puerto
  bcopy((char *)servidor->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(servidor->h_length)); // copia la dirección del servidor en la estructura del cliente.


  if(connect(conexion,(struct sockaddr *)&cliente, sizeof(cliente)) < 0)
  { //conectando con el host
    printf("Error conectando con el host\n");
    close(conexion);
    return 1;
  }
  printf("Conectado con %s:%d\n",inet_ntoa(cliente.sin_addr),htons(cliente.sin_port)); // imprimo la dirección del servidor. inet convierte la ip en string

  printf("Escribe un mensaje: ");
  fgets(buffer, sizeof(buffer), stdin);
  send(conexion, buffer, sizeof(buffer), 0); //envio con el buffer vacío
  bzero(buffer, sizeof(*buffer)); // vacío el buffer
  recv(conexion, buffer, sizeof(buffer), 0); // recibo la información nueva en el buffer
  printf("%s", buffer);

  free(buffer);
return 0;
}

#endif
