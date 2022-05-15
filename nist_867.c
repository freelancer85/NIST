#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

//Read n bytes from socket
static int readn(int sock, char * buf, const int n){
  int i = 0, b;
  while((b = read(sock, &buf[i], n - i)) > 0){
    i+= b;
  }
  return i;
}

int main(){
  char buf[49]; //48 for string and 1 for \0
  int n, sock, d, m, y, H, M, S;
  struct addrinfo hints, *result, *rp;

  /* Obtain address(es) matching host/port. */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if(getaddrinfo("time.nist.gov", "13", &hints, &result) != 0) {
    fprintf(stderr, "Error: Resolve failed\n");
    return EXIT_FAILURE;
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
      sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (sock == -1)
        continue;

      if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
        break;

      close(sock);
  }
  freeaddrinfo(result);

  if (rp == NULL) {
    fprintf(stderr, "Error: Could not connect\n");
    return EXIT_FAILURE;
  }

  //example string '59673 22-04-04 18:20:54 50 0 0   4.6 UTC(NIST) *'
  if(readn(sock, buf, 48) != 48){ //string is exactly 48 bytes
    perror("read");
  }
  close(sock);

  buf[49] = '\0';


  if(sscanf(buf, "%d %d-%d-%d %d:%d:%d", &n, &y, &m, &d, &H, &M, &S) != 7){
    fprintf(stderr, "Error: Invalid syntax\n");
  }else{
    printf("Month: %02i\nDay: %02i\nYear: %i\nHour: %02i\nMin: %02i\nSec: %02i\n",
      m, d, 2000+y, H, M, S);
  }

  return EXIT_SUCCESS;
}
