CC = gcc
CFLAGS = -Wall -g

all: target

global.o: global.c imports.h 
	$(CC) $(CFLAGS) -c global.c

helpersDef.o: helpersDef.c imports.h helpers.h
	$(CC) $(CFLAGS) -c helpersDef.c

pso_impl.o: helpersDef.c imports.h helpers.h pso.h

testBenchPSO.o: testBenchPSO.c imports.h helpers.h pso.h
	$(CC) $(CFLAGS) -c testBenchPSO.c

target: global.o helpersDef.o pso_impl.o testBenchPSO.o 
	$(CC) $(CFLAGS) -o runPSO global.o helpersDef.o pso_impl.o testBenchPSO.o 

clean:
	rm -f *.o target rm -f
