#include <stdlib.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include "send.h"
#include "error_wrapper.h"

char* prepare_message(int start, int len){
	// int -> string
	int l1 = snprintf(NULL, 0, "%d", start);
	char* str_st = malloc(l1 + 1);
	snprintf( str_st, l1 + 1, "%d", start);
	// int -> string
	int l2 = snprintf(NULL, 0, "%d", len);
	char* str_len = malloc(l2 + 1);
	snprintf(str_len, l2 + 1, "%d", len);
	// compose message
	int size = l1 + l2 + 6;
	char* message = malloc((size_t) size);
	strcat(message, "GET ");
	strcat(message, str_st);
	strcat(message, " ");
	strcat(message, str_len);
	strcat(message, "\n");

	free(str_st);
	free(str_len);

	return message;
}

void send_packet(int sockfd, struct sockaddr_in* addr, int start, int len){
	char* message = prepare_message(start, len);
	ssize_t message_len = strlen(message);
	Sendto(sockfd, message, message_len, 0, (struct sockaddr*) addr, sizeof(*addr));
	free(message);
}

int send_window(int sockfd, struct sockaddr_in* addr, int bytes, Received_Packet *tab, int begin_idx, int end_idx){
	int to_send = 0;
	for(int i = begin_idx; i <= end_idx; i++){
		if (tab[i].received == false)
			to_send++;
	}
	int j = to_send;
	while(j){
		for (int i = 0; i <= end_idx - begin_idx; i++){
			if (tab[i + begin_idx].received == false){
				if (tab[i + begin_idx].timeout == 0){
					int start = begin_idx * MAX_PACKET_DATA_LEN + i * MAX_PACKET_DATA_LEN;
					int len = bytes - start < MAX_PACKET_DATA_LEN ? bytes - start : MAX_PACKET_DATA_LEN;
					send_packet(sockfd, addr, start, len);
					tab[i + begin_idx].timeout = TIMEOUT;
					j--;
					if(j == 0)
						break;
				}
				else{
				tab[i + begin_idx].timeout--;
				}
			}
		}
	}
	return to_send;
}
