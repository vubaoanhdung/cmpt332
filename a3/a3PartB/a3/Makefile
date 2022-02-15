# Makefile
# Author: Damon Vu
# Student Number: 11261393
# NSID: bav965

# Partner: None (we decided to split apart after a2)

.PHONY:	clean

all: test_program

test_program: test_program.c petgroomsynch.c
	gcc -Wall -Wextra -o test_program test_program.c petgroomsynch.c -pthread

# app: petgroomsynch.c
# 	gcc -Wall -Wextra -o app petgroomsynch.c -pthread


clean:
	rm -f *.o *.a test_program app




