#include "pipe_networking.h"

int server_setup() {
  //use getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, "9001", hints, &results);  //Server sets node to NULL

  //create socket
  printf("Server creating socket\n");
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  int bresult = bind(sd, results->ai_addr, results->ai_addrlen);
  if (bresult) {
    printf("Server didn't bind\n");
    return -1;
  }

  //DO STUFF
  printf("Server is listening\n");
  int val  = listen(sd, 10);
  if (val) {
    printf("Server didn't listen\n");
    return -1;
  }

  return sd;
  free(hints);
  freeaddrinfo(results);
}

int server_connect(int from_client) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  client_socket = accept(from_client,(struct sockaddr *)&client_address, &sock_size);
  return client_socket;
}

int client_connect() {
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo(NULL, "9001", hints, &results);  //Server sets node to NULL

  //create socket
  printf("Server creating socket\n");
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  int c = connect(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);

  return sd;
}
