#include <stdlib.h>
#include <math.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include "transport.h"
#include "error_wrapper.h"
#include "receive.h"
#include "send.h"

Initial_Arguments prepare_data(char* argv[]){
	Initial_Arguments i_a;
	i_a.ip_address = argv[1];
	i_a.port = atoi(argv[2]);
	i_a.filename = argv[3];
	i_a.bytes = atoi(argv[4]);
	double x = (double) i_a.bytes / MAX_PACKET_DATA_LEN;
	i_a.max_packets_amount =   ceil(x);
	return i_a;
}

void init_recv_packets(Received_Packet* recv_pack, int max){
	for(int i = 0; i < max; i++){
		recv_pack[i].received = 0;
		recv_pack[i].timeout = 0;
	}
}

struct sockaddr_in prepare_server_address(Initial_Arguments* init_args){
	struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family      = AF_INET;
	server_address.sin_port        = htons(init_args->port);
	inet_pton(AF_INET, init_args->ip_address, &server_address.sin_addr);
	return server_address;
}

void reserve_memory_for_data(Received_Packet* tab, int begin_idx, int how_many, int max_packets_amount){
	if (begin_idx < max_packets_amount){
		for (int i = 0; i < how_many; i++){
			tab[i + begin_idx].data = malloc((size_t) MAX_PACKET_DATA_LEN);
		}
	}
}

void write_data(Received_Packet* tab, FILE* f, int *start, int *end, int max_packets_amount){
	int start_idx = *start;
	int old_end_idx = *end;
	int i = 0;
	while(i <= old_end_idx - start_idx){
		if(tab[i + start_idx].received == true){
			fwrite(tab[i + start_idx].data, 1, (size_t) tab[i + start_idx].length, f);
			free(tab[i + start_idx].data);
			i++;
		}
		else
			break;
	}
	if(start_idx + i < max_packets_amount)
		*start += i;

	// check if old end index is not more than total amount of packets that I have to receive
	if (max_packets_amount - i > old_end_idx){
		*end += i;
	}
	// check if old end index is not at the last possible position
	else if (old_end_idx < max_packets_amount - 1)
	{
		*end = max_packets_amount - 1;	// updating end index
		i = max_packets_amount - old_end_idx - 1; // amount describing distance between  old end index and the last position
	}
	reserve_memory_for_data(tab, old_end_idx + 1, i, max_packets_amount);
}

int main(int argc, char* argv[])
{	
    if (argc != 5) {print_usage();}

//  init data
	Initial_Arguments init_args = prepare_data(argv);
	struct sockaddr_in server_address = prepare_server_address(&init_args);
	FILE *f = Fopen(init_args.filename, "w");
	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	Received_Packet all_packets[init_args.max_packets_amount];
	init_recv_packets(all_packets ,init_args.max_packets_amount);

	int already_received = 0;
	int begin_idx = 0;
	int end_idx = init_args.max_packets_amount < WINDOW_SIZE ? init_args.max_packets_amount - 1  : WINDOW_SIZE - 1;
	reserve_memory_for_data(all_packets, begin_idx, end_idx + 1, init_args.max_packets_amount);

	while(already_received < init_args.max_packets_amount){
		int to_receive = send_window(sockfd, &server_address, init_args.bytes, all_packets, begin_idx, end_idx);
		int received = receive(sockfd, all_packets, &init_args, begin_idx, to_receive);
		already_received += received;
		print_progress(already_received, init_args.max_packets_amount);
		write_data(all_packets, f, &begin_idx, &end_idx, init_args.max_packets_amount);
	}

	Fclose(f);
	Close(sockfd);
	return EXIT_SUCCESS;
}
