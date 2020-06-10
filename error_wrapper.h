#ifndef ERROR_WRAPPER_H
#define ERROR_WRAPPER_H

#include <sys/socket.h>
#include <stdio.h>
#include <sys/time.h>

int Socket(int domain, int type, int protocol);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, 
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t Recvfrom(int sockfd, void *restrict buffer, size_t len, int flags,
                struct sockaddr *restrict addr, socklen_t *restrict addr_len);
int Select(int nfds, fd_set *restrict readfds, fd_set *restrict writefds, 
            fd_set *restrict errorfds, struct timeval *restrict timeout); 
FILE* Fopen(const char* filename, const char* mode);
int Fclose(FILE *f);
int Close(int fd);

#endif
