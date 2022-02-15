.PHONY: clean

app: memory_allocator.o
	gcc -Wall -o app memory_allocator.o

memory_allocator.o: memory_allocator.c common.h memory_allocator.h
	gcc -c memory_allocator.c

clean:
	rm -f *.o app