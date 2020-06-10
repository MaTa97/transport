#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "error_wrapper.h"

int Socket(int domain, int type, int protocol){
    int res = socket(domain, type, protocol);
    if (res < 0){
        fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, 
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    ssize_t res = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (res < 0){
        fprintf(stderr, "sendto error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

ssize_t Recvfrom(int sockfd, void *restrict buffer, size_t len, int flags,
                struct sockaddr *restrict addr, socklen_t *restrict addr_len)
{
    ssize_t res = recvfrom(sockfd, buffer, len, flags, addr, addr_len);
    if(res < 0 && (errno != EAGAIN || errno != EWOULDBLOCK)){
        fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

int Select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, 
            fd_set *restrict errorfds, struct timeval *restrict timeout)
{
    int res = select(nfds, readfds, writefds, errorfds, timeout);
    if (res < 0){
        fprintf(stderr, "select error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return res;
}

FILE* Fopen(const char* filename, const char* mode){
    FILE *f = fopen(filename, mode);
    if (f == NULL){
        fprintf(stderr, "fopen error: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return f;
}

int Fclose(FILE *f){
    int res = fclose(f);
    if (res == EOF){
        fprintf(stderr, "fclose error: %s", strerror(res));
        exit(EXIT_FAILURE);
    }
    return res;
}

int Close(int fd){
    int res = close(fd);
    if (res < 0){
        fprintf(stderr, "close error: %s", strerror(res));
        exit(EXIT_FAILURE);
    }
    return res;
}
