#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(10086);

  socklen_t len = sizeof(addr);
  int ret = bind(fd, (struct sockaddr*)(&addr), len);

  printf("ret = %d\n", ret);

  while (1) {
      char buf[4096] = {0};
      int len = 4096;

      struct sockaddr_in addr1;
      socklen_t len1 = sizeof(addr1);
      int ret = recvfrom(fd, buf, len, 0, (struct sockaddr*)&addr1, &len1);
      // printf("[recv:%d:%s]\n", ret, buf);

      ret = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)(&addr1), len1);
      // printf("[snd:%d]\n", ret);
  }
}
