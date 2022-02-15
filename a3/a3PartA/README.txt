Author: Damon(Bao) Vu
Student Number: 11261393
NSID: bav965

Partner: None

**********

README - Part A

**********

- To Run The Program:
	+ run "make clean"
	+ run "make qemu"
	+ run "ipc_program"
	
**********

Files changed:
	
	Makefile

	kernel:
		defs.h 
			111-114	
		main.c
			23
		param.h
			18-19
		proc.c
			9-13
			932-1091
		proc.h
			85-90
		syscall.c
			142-145
		syscall.h
			26-29
		sysproc.c
			134-243
		types.h
			11
	
	user:
		ipc_program.c
		
	
	
**********
	
Notes:

valid tag (topic_t): QUIZ, MIDTERM, FINAL
MAXTWEETTOTAL  = 5

tput:
	There should be a failure return if the system is holding the 
	maximum number of tweets.

tget:
	There should be a failure return if the system contains no tweets or
	there is no tweet with the given tag exists in the system
	
tput was tested with 6 (i=0->4) tweets (1 tweet more than the MAXTWEETTOAL). The first 
five tweets went through and got saved. However, the message got replaced for
each iteration while the tag was expected. For example:

tweet1: QUIZ, "Quiz was easy"
tweet2: MIDTERM, "Midterm was ok"
tweet3: FINAL, "Final was hard"
tweet4: QUIZ, "Other Quizzes"
tweet5: QUIZ, "Other Quizzes"

when these tweets got saved into the system

first tweet got saved:
tweet1: QUIZ, "Quiz was easy"

first two tweets got saved:
tweet1: QUIZ, "Midterm was ok"
tweet2: MIDTERM, "Midterm was ok"

and so on
Therefore, at the end of the first 5 iteration
tweet1: QUIZ, "Other Quizzes"
tweet2: MIDTERM, "Other Quizzes"
tweet3: FINAL, "Other Quizzes"
tweet4: QUIZ, "Other Quizzes"
tweet5: QUIZ, "Other Quizzes"

This is because the message pointer for each system call was the same. It pointed
to a single location inside the kernel. I have printed out the message address
inside the kernel to show this bug in ipc_program. This is caused by not using
the copyin function correctly inside the proc.c. Many tries have been made but
I could not adjust this problem.

For the last iteration (i=5), tput return -1 as there are 5 tweets in the system
already. This is the behaviour as expected

Since tput replaced the messages being saved, tget gets the wrong message. 
If I was able to fix the problem in tput, then tget should get the right message
(with the use of copyout() in the right way)

**********

btput and btget was not fully tested, I have commented out the test code segment
inside the main of ipc_program. Please comment out the code above it and uncomment 
that segment to see the behaviours of btget and btput. 

My original plan was that when the number of tweets in the system already at
the MAXTWEETTOAL, the calling btput process will be put in sleep and waiting 
in channel "-1". When the number of tweets in the system < MAXTWEETTOTAL, then
that waiting process will be wakened up. Also, there were going to be 3 different
wait channels for btget (-2 for QUIZ, -3 for MIDTERM, -4 for FINAL). When a tweet
getting put into the system, depending on its tag, processes in different
waiting channels will be wakened up.

The above is the original plan, however, I went with a simple solution first with
intent to add more complexity incrementally. I use only 1 wait channel -2 if
the number of tweets in the system is 0, and that btget always gets the last 
tweet in the array (similar to pop() in array) and wakes up all process in 
channel -1. For example,

array: 0, 1, 2, 3, 4, 5

btget()

array: 0, 1, 2, 3, 4

Nevertheless, I could not make the two functions work together (the synchronization
problem). btput was waiting forerver if number of tweets == MAXTWEETTOAL => btget
does not have chance to run and wake btput up.
	
	

