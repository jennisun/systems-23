#include "pipe_networking.h"

int server_setup() {
  //use getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo("localhost", "9001", hints, &results);  //Server sets node to NULL

  //create socket
  printf("Server creating socket\n");
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  int bresult = bind(sd, results->ai_addr, results->ai_addrlen);
  if (bresult) {
    printf("Server didn't bind\n");
    exit(EXIT_FAILURE);
  }

  //DO STUFF
  printf("Server is listening\n");
  listen(sd, 10);

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

/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("Server creates WKP..\n");
  int val = mkfifo(WKP, 0644);
  if (val < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }

  printf("Server opening and reading secret pipe..\n");
  int server = open(WKP, O_RDONLY);
  if (server < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }
  char client[HANDSHAKE_BUFFER_SIZE];
  val = read(server, client, HANDSHAKE_BUFFER_SIZE);
  if (val < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }

  printf("Server removing WKP..\n");
  remove(WKP);

  printf("Server sending message to client..\n");
  *to_client = open(client, O_WRONLY);
  if (*to_client < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }
  val = write(*to_client, ACK, sizeof(ACK));
  if (val < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }

  printf("Server receives verification message from client..\n");
  val = read(server, client, HANDSHAKE_BUFFER_SIZE);
  if (val < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }
  if (strcmp(client, ACK) != 0) {
    printf("Error occured\n");
    return 0;
  }
  else {
    printf("Server success\n");
    return server;
  }
}

/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  getaddrinfo("localhost", "9001", hints, &results);  //Server sets node to NULL

  //create socket
  printf("Server creating socket\n");
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  int c = connect(sd, results->ai_addr, results->ai_addrlen);

  return c;
  free(hints);
  freeaddrinfo(results);
}
