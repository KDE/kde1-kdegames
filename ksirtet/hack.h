#include <sys/types.h>
#include <sys/socket.h>

/* if someone could explain me why this hack seems necessary to compile */

int Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
int Accept(int sock, struct sockaddr *addr, int *addrlen);
                
