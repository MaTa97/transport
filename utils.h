#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define PREFIX_LEN 20
#define MAX_PACKET_DATA_LEN 1000
#define WINDOW_SIZE 1000
#define IGNORE_STRING_DATA 5
#define MAX_DIGIT_NUMBER 8
#define TIMEOUT 5

typedef struct Initial_Arguments {
	char* ip_address;
	int port;
	char* filename;
	int bytes;
	int max_packets_amount;
} Initial_Arguments;

typedef struct Received_Packet {
	char* data;
	int length;
	int timeout;
	bool received;
} Received_Packet;

int count_length(int bytes, int i);
void print_progress(int i, int all);
void print_usage();

#endif
