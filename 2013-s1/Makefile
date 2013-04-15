CC = gcc

FLAGS = -ansi -Wall -pedantic -g -lm -std=c99

UTIL = util.h util.c

all: wHeap sHeap wHash sHash

wHeap: wHeap.c $(UTIL)
	$(CC) $(FLAGS) wHeap.c $(UTIL) -o wHeap

sHeap: sHeap.c $(UTIL)
	$(CC) $(FLAGS) sHeap.c $(UTIL) -o sHeap

wHash: wHash.c $(UTIL)
	$(CC) $(FLAGS) wHash.c $(UTIL) -o wHash

sHash: sHash.c $(UTIL)
	$(CC) $(FLAGS) sHash.c $(UTIL) -o sHash

clean:
	rm -rf *.o wHeap sHeap wHash sHash hashed heap 
