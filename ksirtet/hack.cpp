#include <sys/types.h>
#include <sys/socket.h>
#include "config.h"

int Connect(int sockfd, struct sockaddr *serv_addr, int addrlen)
{
         return  connect(sockfd, serv_addr, (ksize_t)addrlen);
}

int Accept(int sock, struct sockaddr *addr, int *addrlen)
{
         int rv;
         ksize_t tmplen = (ksize_t)addrlen;
         rv = accept(sock, addr, &tmplen);
         *addrlen = (int)tmplen;
         return rv;
}
