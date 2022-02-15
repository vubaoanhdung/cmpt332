#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Please provide a positive integer!\n");
		exit(1);
	} else {
		int i,j;
		int num = atoi(argv[1]); // getting the number of times it should fork itself
		for (i = 0; i < num; i++) {
			int rc = fork();
			if (rc < 0) {
				printf("fork failed!\n");
				break;
			} else if (rc == 0) {
				// child
				exit(0);
			}
		}

		// invoke new system call and print out the result
		printf("Number of processes that currently exists in the system whose name begins with 'cmpt': %d\n", howmanycmpt());

		// invoke the helper system call for visualization
		// cps lists all processes exist in the system
		// cps();

		// wait for all child processes to finish
		int status;
		for (j = 0; j < num; j++) {
			wait(&status);
		}

		// after all processes finished, invoke helper system call cps to confirm
		// printf("All child processes exited\n");
		// cps();
		
	}

	exit(0);
}
