#ifndef SERVIDOR_SOCKET
#define SERVIDOR_SOCKET

// Por ahora dejo esto básico.
// Lo único que hace es conectar un cliente con un servidor, quien recibe un manda un string a el último.
// Si quieren probarlo, primero ejecutan el servidor pasándole como parámetro el puerto.
// Después ejecutan el cliente y pasan como parámetro la ip del servidor (127.0.0.1) y el puerto que le pasaron al servidor.

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>


int main (int argc, char ** argv){

  if(argc<2)
  { //Especifica los argumentos
    printf("%s <puerto>\n",argv[0]);
    return 1;
  }

  int conexion_servidor, conexion_cliente, puerto; //variables de direcciones

  socklen_t longc; //Debemos declarar una variable que contendrá la longitud de la estructura

  struct sockaddr_in servidor, cliente; //estructuras de direcciones de cliente y servidor

  char * buffer = malloc (sizeof(char)*1000); //para lo que se reciba

  puerto = atoi(argv[1]);

  conexion_servidor = socket(AF_INET, SOCK_STREAM, 0); //creamos el socket

  bzero((char *)&servidor, sizeof(servidor)); //llenamos la estructura de 0's

  servidor.sin_family = AF_INET; //asignamos a la estructura

  servidor.sin_port = htons(puerto);

  servidor.sin_addr.s_addr = INADDR_ANY; //esta macro especifica nuestra dirección

  int activado = 1;
  setsockopt(conexion_servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(int)); // para que no joda con el tema de esperar la conexión

  if(bind(conexion_servidor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
  { // asocia el servidor con la estructura dirección
    printf("Error al asociar el puerto a la conexion\n");
    close(conexion_servidor);
    return 1;
  }

  listen(conexion_servidor, 3); //Pongo de prueba 3 conexiones máximo para encolar

  printf("A la escucha en el puerto %d\n", ntohs(servidor.sin_port));

  longc = sizeof(cliente); //Asignamos el tamaño de la estructura a esta variable

  conexion_cliente = accept(conexion_servidor, (struct sockaddr *)&cliente, &longc); //Esperamos una conexion

  if(conexion_cliente<0)
  {
    printf("Error al aceptar trafico\n");
    close(conexion_servidor);
    return 1;
  }

  printf("Conectando con %s:%d\n", inet_ntoa(cliente.sin_addr),htons(cliente.sin_port));

  if(recv(conexion_cliente, buffer, sizeof(buffer), 0) < 0)
  { //Comenzamos a recibir datos del cliente
    //Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
    printf("Error al recibir los datos\n");
    close(conexion_servidor);
    return 1;
  }
  else
  {
    printf("%s\n", buffer);
    bzero((char *)&buffer, sizeof(buffer));
    send(conexion_cliente, "Recibido\n", 13, 0);
  }

  close(conexion_servidor);
  free(buffer);

  return 0;
}

#endif
