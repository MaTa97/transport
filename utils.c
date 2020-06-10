#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int count_length(int bytes, int i){
	if ((bytes - i * MAX_PACKET_DATA_LEN) / MAX_PACKET_DATA_LEN >= 1)
			return MAX_PACKET_DATA_LEN;
		else
			return bytes % MAX_PACKET_DATA_LEN;
}

void print_progress(int i, int all){
	static int old_i = 0;
	if (old_i != i)
		printf("Done %f%%\n", (double)(i * 100) / all);
	old_i = i;
}

void print_usage(){
    fprintf(stderr, "Usage: ./transport IP-address port filename size\n");
    exit(EXIT_FAILURE);
}
