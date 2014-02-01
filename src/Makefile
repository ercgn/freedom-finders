CC = gcc
CFLAGS = -g -Wall -Werror -std=gnu99

all: ffind

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

ffind.o: ffind.c ffind.h
	$(CC) $(CFLAGS) -c ffind.c

ffind: ffind.o csapp.o 

clean:
	rm -f *~ *.o ffind free_times.ics

