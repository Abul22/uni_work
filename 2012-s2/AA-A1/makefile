FLAG= -W -Wall -D_XOPEN_SOURCE=500 -lm -g -ansi 
all: set_search

set_search: main.o libset.o libutil.o
	gcc -o set_search main.o libset.o libutil.o $(FLAG)

main.o: main.c
	gcc -c main.c $(FLAG)

libset.o: libset.c
	gcc -c libset.c $(FLAG)

libutil.o: libutil.c
	gcc -c libutil.c $(FLAG)

main-test.o: main-test.c
	gcc -c main-test.c $(FLAG)

test: main-test.o libset.o libutil.o
	gcc -o test main-test.o libset.o libutil.o $(FLAG) 

clean:
	-rm *.o
	-rm set_search
	-rm test
	-rm core

