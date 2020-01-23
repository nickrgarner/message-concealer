CC = gcc
CFLAGS = -Wall -std=c99 -g

all: conceal extract

conceal: bits.o image.o

extract: bits.o image.o

conceal.o: bits.h image.h
extract.o: bits.h image.h
image.o: image.h bits.h
bits.o: bits.h

clean:
	rm -f *.o
	rm -f *.exe
	rm -f conceal
	rm -f extract
	rm -f output.ppm
	rm -f output.txt
	rm -f stderr.txt
	rm -f *.exe.stackdump