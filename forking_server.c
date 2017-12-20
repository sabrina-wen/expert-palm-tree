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
  
  //when server is terminated with ctrl+c
  signal(SIGINT, sighandler);
  
  while (1) {
    //set up WKP
    int server_fd = server_setup();
    int f = fork();
    
    // if parent process (main server); close connection
    if (f != 0) { 
      close(server_fd);
    }
    // subserver process; keep connection
    else { 
      subserver(server_fd);
      return 0;
    }
  }
  return 0;
}


void subserver(int from_client) {

  int to_client;
  int process_id = getpid();
  char buffer[BUFFER_SIZE];

  //3 way handshake 
  to_client = server_connect(from_client);

  //while client continues to send something, change the received, send back
  while (read(from_client, buffer, BUFFER_SIZE)) {
    printf("[subserver %d] recieved info from client: %s\n",
	   process_id, buffer);
    process(buffer);
    write(to_client, buffer, BUFFER_SIZE);
  }
}


//make all letters uppercase
void process(char * s) {
  while (*s) {
    *s = toupper(*s);
    s++;
  }
}
