#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  while (1) {
    int server_fd = server_setup();
    int f = fork();
    if (f != 0) { // if parent process (main server)
      close(server_fd);
    } else { // subserver process
      subserver(server_fd);
    }
  }
  return 0;
}

void subserver(int from_client) {
  int to_client;
  int process_id = getpid();
  char buffer[BUFFER_SIZE];
  to_client = server_connect(from_client);
  while (read(from_client, buffer, BUFFER_SIZE)) {
    printf("[subserver %d] recieved info from client: %s\n", process_id, buffer);
    process(buffer);
    write(to_client, buffer, BUFFER_SIZE);
  }
}

void process(char * s) {
  while (*s) {
    *s = toupper(*s);
    s++;
  }
}
