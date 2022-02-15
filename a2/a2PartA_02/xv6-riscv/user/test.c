#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
    helper function which will fork 15 times, calculate the total 
    turnaround time and total estimated running time. Then it will
    print the results 
*/
void exec_helper(int x, int y) {
        int totalTurnaroundTime = 0;
        int totalEstimatedRunningTime = 0;
        int status;
        uint turnaroundTime, estimatedRunningTime;
        
        int t = 15;
        while (t > 0) {
            t--;
            
            int rc = fork();
            if (rc < 0) {
                printf("fork failed!\n");
                break;
            } else if (rc == 0) {
                // child
                int i, j;
                int sum = 0;
                for (i = 1; i <= x; i++) {
                    for (j = 1; j <= y; j++) {
                        sum += (i - j);
                    }
                }
                exit(0);
            } else {
                waitstat(&status, &turnaroundTime, &estimatedRunningTime);
                totalTurnaroundTime +=  turnaroundTime;
                totalEstimatedRunningTime += estimatedRunningTime;
            }
            
        }
        printf("Sum of the turnaround times is: %d\n", totalTurnaroundTime);
        printf("Sum of the estimated running times is: %d\n", totalEstimatedRunningTime);
}

int main(int argc, char const *argv[])
{

    if (argc != 5) {
        printf("Please provide 4 positive integer\n");
        exit(1);
    } else {
        int k = atoi(argv[1]);
        int l = atoi(argv[2]);
        int m = atoi(argv[3]);
        int n = atoi(argv[4]);

        printf("\n----------\n\n");

        printf("First 15 Child Processes (K, N)=(%d, %d)\n", k, n);
        exec_helper(k, n);
        printf("\n");

        printf("Next 15 Child Processes (K, M)=(%d, %d)\n", k, m);
        exec_helper(k, m);
        printf("\n");

        printf("Last 15 Child Processes (K, L)=(%d, %d)\n", k, l);
        exec_helper(k, l);

        printf("\n----------\n\n");

    }

    exit(0);
}

