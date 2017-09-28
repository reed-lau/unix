#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

int esocket(int domain,int type,int protocol);
int ebind(int sockfd,const struct sockaddr_in *addr,int len);
int elisten(int sockfd,int backlog);
int eaccept(int sockfd,struct sockaddr_in *addr,int *addrlen);
int econnect(int sockfd,struct sockaddr_in *addr);
struct sockaddr_in mkaddr(char *ipbydot,int port);


#endif
