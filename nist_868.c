#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <time.h>

//Read n bytes from socket
static int readn(int sock, char * buf, const int n){
  int i = 0, b;
  while((b = read(sock, &buf[i], n - i)) > 0){
    i+= b;
  }
  return i;
}

int main(int argc, char * argv[]){
  int num, sock;
  struct addrinfo hints, *result, *rp;

  /* Obtain address(es) matching host/port. */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if(getaddrinfo("time.nist.gov", "37", &hints, &result) != 0) {
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


  if(readn(sock, (void*) &num, 4) != 4){ //binary time is 4 bytes
    perror("read");
  }else{
    time_t t = ntohl(num);  //convert from network to host byte order
    t -= 2208988800;        //adjust to Unix epoch

    struct tm * tm = gmtime(&t);
    if(tm == NULL){
      perror("gmtime");
    }else{
      printf("Month: %02i\nDay: %02i\nYear: %i\nHour: %02i\nMin: %02i\nSec: %02i\n",
        tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);
    }
  }
  close(sock);

  return EXIT_SUCCESS;
}
