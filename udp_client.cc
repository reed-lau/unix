#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h> 
#include <unistd.h>

#include <string>


int main(int argc, char *argv[]) {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
  addr.sin_port = htons(10086);

  socklen_t len = sizeof(addr);

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000;
  if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      fprintf(stderr, "setsockopt fail\n");
  }

  int i = 0;
  while (1) {
      char buf[4096] = {0};
      std::string bufs(1290, 'x');
      sprintf(buf, "hello udp %d %s", ++i, bufs.c_str());

      sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&addr, len);

      struct sockaddr_in addr1;
      socklen_t len1 = sizeof(addr);
      memset(buf, 0, 4096);
      int ret = recvfrom(fd, buf, 4096, 0, (struct sockaddr*)&addr, &len1);
      if (ret == -1) {
          fprintf(stderr, "-- message miss --\n");
      }

      size_t n = strlen(buf);
      if (n > 20) {
        printf("[recv:%d:%8.8s...%8.8s]\n", ret, buf, &buf[n - 8]);
      } else {

      }
      usleep(5000);
  }
}
