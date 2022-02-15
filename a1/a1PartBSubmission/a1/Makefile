# Author: Bao Vu (bav965 - 11261393)

CFLAGS = -Wall

.PHONY: clean

all: wrdsh

wrdsh: wrdsh.o mylib.o
	gcc wrdsh.o mylib.o -o wrdsh


wrdsh.o: wrdsh.c mylib.c
	gcc $(CFLAGS) -c wrdsh.c mylib.c

clean:
	rm -f wrdsh.o
	rm -f wrdsh
	rm -f mylib.o