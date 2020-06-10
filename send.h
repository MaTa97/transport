#ifndef SEND_H
#define SEND_H

#include <netinet/in.h>
#include "utils.h"

char* prepare_message(int start, int len);
void send_packet(int sockfd, struct sockaddr_in* addr, int start, int len);
int send_window(int sockfd, struct sockaddr_in* addr, int bytes, Received_Packet *tab, int begin_idx, int end_idx);

#endif
