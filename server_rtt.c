#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>

#define DATA 32
#define LOOPS 1000

int main(int argc , char *argv[]) {
  if(argc != 2) {
    perror("USAGE: ./SERVER-EXE <PORT-NO>\n");
    return 1;
  }

  int port_num = atoi(argv[1]);
  char temp_msg[32];
  memset(temp_msg, '0', sizeof(temp_msg));

  int server_sock, client_sock, client_len;
  struct sockaddr_in server, client;
  client_len = sizeof(struct sockaddr_in);

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock == -1) {
    perror("ERROR: Socket create failed!\n");
    return 1;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port_num);

  if(bind(server_sock,(struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("ERROR: Socket bind failed!");
    return 1;
  }

  listen(server_sock , 3); 
  printf("Started Server...\n");
  

  while(1) {
    client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t*)&client_len);

    if (client_sock < 0) {
      perror("ERROR: Socket accept failed!");
      return -1;
    }

    for (int j = 0; j < LOOPS; j++) {
      recv(client_sock, &temp_msg, DATA, 0);
      send(client_sock, &temp_msg, DATA, 0);
    }
  }

  printf("Ending Server...\n");
  return 0;
}
