#ifndef RECEIVE_H
#define RECEIVE_H

#include <stdbool.h>
#include <netinet/in.h>
#include "utils.h"

bool check_recv_addr(struct sockaddr_in *rcv_addr, int port, char* ip_address);
bool wait_for_packets(int sockfd);
char* read_number(int* result, char* buf, char separator);
char* get_start_length(int *start, int *length, char* buf);
void save_data(int start, int length, char* data, int* received, Received_Packet* tab);
bool process(char* buffer, int* received, Received_Packet* tab);
int receive(int sockfd, Received_Packet *tab, Initial_Arguments* init_args, int start_idx, int to_receive);

#endif
