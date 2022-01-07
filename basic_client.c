#include "pipe_networking.h"


int main() {

  int sd = client_connect();

  char input[BUFFER_SIZE];
  while(1) {
    printf("Enter input: ");
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0';
    int val = write(sd, input, sizeof(input));
    if (val < 0) {
      printf("Error: %s\n", strerror(errno));
      return 0;
    }
    val = read(sd, input, sizeof(input));
    printf("Output: %s\n", input);
  }
}
