#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;
  int sd = server_setup();

  char input[BUFFER_SIZE];
  while (1) {
    from_client = server_connect(sd);
    int f = fork();
    if (!f) {
      // to_client = server_connect(from_client);
      while (1) {
        int val = read(from_client, input, BUFFER_SIZE);
        if (val < 0) {
          printf("Error: %s\n", strerror(errno));
          return 0;
        }

        int len = strlen(input);
        char line[len];
        for (int i = 0; i < len; i ++) {
          line[len - 1 - i] = input[i];
        }
        line[len] = '\0';
        write(from_client, line, sizeof(line));
      }
    }
    else {
      close(from_client);
    }
  }
}
