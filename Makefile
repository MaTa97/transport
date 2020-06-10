CC = gcc
FLAGS = -std=gnu99 -Wall -Wextra

transport: error_wrapper.o utils.o transport.o receive.o send.o
	$(CC) $(FLAGS) error_wrapper.o utils.o receive.o send.o transport.o -o transport -lm

error_wrapper.o: error_wrapper.c error_wrapper.h
	$(CC) $(FLAGS) error_wrapper.c -c -o error_wrapper.o

utils.o: utils.c utils.h
	$(CC) $(FLAGS) utils.c -c -o utils.o

receive.o: receive.c receive.h utils.h error_wrapper.h
	$(CC) $(FLAGS) receive.c -c -o receive.o 

send.o: send.c send.h utils.h error_wrapper.h
	$(CC) $(FLAGS) send.c -c -o send.o

transport.o: transport.c transport.h receive.h send.h utils.h error_wrapper.h
	$(CC) $(FLAGS) transport.c -c -o transport.o 

clean:
	rm -f *.o

distclean:
	rm -f *.o transport a.out 
