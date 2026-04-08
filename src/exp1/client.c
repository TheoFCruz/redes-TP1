#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>   
#include <unistd.h>   
#include <string.h>   

#define SERVER_PORT 5431
#define MAX_LINE 256

int main(int argc, char * argv[])
{
  FILE *fp;
  struct sockaddr_in sin;
  char *host;
  int host_addr;
  char buf[MAX_LINE];
  int s;
  int len;

  if (argc==2) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "No host address provided. Usage: exp*_client xxx.xxx.xxx.xxx\n");
    exit(1);
  }

  // get host from dotted notation
  host_addr = inet_addr(host);

  /* build address data structure */
  bzero((char *)&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  bcopy(&host_addr, (char *)&sin.sin_addr, sizeof(host_addr));
  sin.sin_port = htons(SERVER_PORT);

  /* active open */
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("simplex-talk: socket");
    exit(1);
  }

  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
  {
    perror("simplex-talk: connect");
    close(s);
    exit(1);
  }

  /* main loop: get and send lines of text */
  while (fgets(buf, sizeof(buf), stdin)) {
    buf[MAX_LINE-1] = '\0';
    len = strlen(buf) + 1;
    send(s, buf, len, 0);
  }
}
