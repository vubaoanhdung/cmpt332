/*
    Author: Damon(Bao) Vu
    NSID: bav965
    Student #: 11261393
*/

README

- To Run The Program
	+ Extract
	+ Run "make clean"
	+ Run "make"
	+ Run "./server" in a terminal
	+ Run "./sender_client hostname 33333" in another terminal
	+ Run "./receiver_client hostname 34444" in another terminal
	
	NOTE: 
	The order of sender_client and receiver_client is not matter
	Users can fire up receiver_client before the sender client
	Users can use localhost as hostname or a specific hostname (e.g. tux8)
	Users can fire up as many sender and receiver clients as they want.
	
	+ Start to send messages in the sender_clients terminals

e.g.
	Terminal 1:
		make clean
		make
		./server
	
	Terminal 2:
		./receiver_client localhost 34444
		
	Terminal 3:
		./sender_client localhost 33333
		hello
		=> this would print "hello" inside Terminal 2
		
	Terminal 4:
		./sender_client localhost 33333
		world
		=> this wourld print "world" inside Terminal 2
	
	Terminal 5:
		./receiver_client localhost 34444
	
	Inside Terminal 3 (again)
		hello world again
		=> this would print "hello world again" inside Terminal 2 and Terminal 5
	
	
Please note that:
	+ The program behaves as expected. However, it does not handle
	the cases where server or clients disconnected/left. I have not
	tackled this issue so there is still room for improvement. The
	most severe case is when you terminate a sender_client before 
	terminating the server. It will give a core dump error to the server
	and send infinite messages to all the receiver clients. On the other hand,
	nothing will happen if you terminate the server or receiver clients(still,
	the program will not function as expected).
	
	+ If you did not terminate any connected terminals, the program will 
	work well.
	
Some key points of the program:
	+ There are two lists in the program, one for messages and one for
	receiver clients. When a receiver client thread is created, it will
	be added to the clients lists. Similarly, when a new message arrives,
	it will be add to the message list. 
	
	+ Messages will be sent to all receiver clients in the receiver client list
	
	+ Resources:
		++ Beej's Guide: Section 5.6 6.2 6.3 and 9.9
		++ Youtube: Chatroom in C using Threads | Socket Programming in C
		https://www.youtube.com/watch?v=fNerEo6Lstw
