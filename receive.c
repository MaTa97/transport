#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "receive.h"
#include "error_wrapper.h"

bool check_recv_addr(struct sockaddr_in *rcv_addr, int port, char* ip_address){
	char r_addr_IP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &rcv_addr->sin_addr, r_addr_IP, INET_ADDRSTRLEN);
	if (rcv_addr->sin_port == htons(port) && strcmp(r_addr_IP, ip_address) == 0)
		return true;
	return false;
}

bool wait_for_packets(int sockfd){
	struct timeval timeout = {.tv_sec = 0, .tv_usec = 50000};
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);
    int ready = Select(sockfd + 1, &descriptors, NULL, NULL, &timeout);
    return ready == 1;
}

char* read_number(int* result, char* buf, char separator){
	int i = 0;
	while(buf[i] != separator) i++;
    char num[i+1];
    strncpy(num, buf, i);
    num[i] = '\0';
	*result = atoi(num);
	return buf += i + 1;
}

char* get_start_length(int *start, int *length, char* buf){
    buf += IGNORE_STRING_DATA;
    char* tmp_buf = read_number(start, buf, ' ');
	char* res_buf = read_number(length, tmp_buf, '\n');
    return res_buf;
}

void save_data(int idx, int length, char* data, int* received, Received_Packet* tab){
	tab[idx].length = length;
	memcpy(tab[idx].data, data, length);
	(*received)++; 
}

bool process(char* buffer, int* received, Received_Packet* tab){
	int start, length;
	char* shifted_buf = get_start_length(&start, &length, buffer);
	if (tab[start / MAX_PACKET_DATA_LEN].received == false){
		save_data(start / MAX_PACKET_DATA_LEN, length, shifted_buf, received, tab);
		tab[start / MAX_PACKET_DATA_LEN].received = true;
        return true;
	}
    return false;
}

int receive(int sockfd, Received_Packet *tab, Initial_Arguments* init_args, int start_idx, int to_receive){
	size_t size = count_length(init_args->bytes, start_idx) + PREFIX_LEN;
	char* buffer = malloc(size);
	struct sockaddr_in recv_address;
	socklen_t len = sizeof(recv_address);
	int received = 0;

	while(received != to_receive){
		if (!wait_for_packets(sockfd))
			break;

		Recvfrom(sockfd, (char *)buffer, size, 0, (struct sockaddr*) &recv_address, &len);
		if (check_recv_addr(&recv_address, init_args->port, init_args->ip_address)){
			process(buffer, &received, tab);
		}	
	}
	free(buffer);
	return received;
}
