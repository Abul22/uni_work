CC = gcc

FLAGES = -ansi -Wall -pedantic -lm -g -std=c99 -D _POSIX_C_SOURCE=200112L -lrt

UTTL = util.h util.c

HASH = hash_table.h hash_table.c

all: dbload dbquery

dbload: dbload.c $(UTIL)
	$(CC) $(FLAGES) dbload.c $(UTTL) -o dbload

dbquery: dbquery.c $(UTIL) $(HASH)
	$(CC) $(FLAGES) dbquery.c $(UTTL) $(HASH) -o dbquery

clean:
	rm -rf *.o dbload dbquery core dbload.dSYM dbquery.dSYM

