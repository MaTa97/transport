#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stdio.h>
#include <netinet/in.h>
#include "utils.h"

Initial_Arguments prepare_data(char* argv[]);
void init_recv_packets(Received_Packet* recv_pack, int max);
struct sockaddr_in prepare_server_address(Initial_Arguments* init_args);
int count_length(int bytes, int i);
void reserve_memory_for_data(Received_Packet* tab, int begin_idx, int how_many, int max_packets_amount);
void write_data(Received_Packet* tab, FILE* f, int *start, int *end, int max_packets_amount);

#endif
