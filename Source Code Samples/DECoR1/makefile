CC = gcc
CFLAGS = -Wall -g 

all: target

global.o: global.c imports.h 
	$(CC) $(CFLAGS) -c global.c

helpersDef.o: helpersDef.c imports.h helpers.h
	$(CC) $(CFLAGS) -c helpersDef.c

decor_impl.o: helpersDef.c imports.h helpers.h decor.h

testBench.o: testBench.c imports.h helpers.h decor.h
	$(CC) $(CFLAGS) -c testBench.c

target: global.o helpersDef.o decor_impl.o testBench.o 
	$(CC) $(CFLAGS) -o runDECoR global.o helpersDef.o decor_impl.o testBench.o 

clean:
	rm -f *.o target rm -f
