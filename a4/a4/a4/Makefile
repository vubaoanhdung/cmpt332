# Author: Damon(Bao) Vu
# NSID: bav965
# Student Number: 11261393

# Partner: Eyan Cunningham
# NSID: esc568

.PHONY: clean

all: sender_client receiver_client  server

server: server.c
	gcc -o server server.c -lpthread

sender_client: sender_client.c
	gcc -o sender_client sender_client.c

receiver_client: receiver_client.c
	gcc -o receiver_client receiver_client.c

clean:
	rm -f *.o *.a sender_client receiver_client server