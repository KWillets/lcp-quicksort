CC=gcc
CFLAGS=-I.  -Ofast  -g -funroll-loops
CPPFLAGS=$(CFLAGS)

DEPS = lcp-quicksort.h
OBJ = lcp-quicksort.o test.o 

%.o: %.c $(DEPS)
	$(CC)  -c -o $@ $< $(CFLAGS)

lcpsort: lcp-quicksort.o lcpsort.o
	g++ -o $@ $^ $(CFLAGS)

lcpsort-sse: lcp-quicksort-sse.o lcpsort.o
	g++ -o $@ $^ $(CFLAGS)

lcpsort-cache: lcp-quicksort-cache.o lcpsort.o
	g++ -o $@ $^ $(CFLAGS)

clean:  
	rm *.o