Author: Bao (Damon) Vu
NSID: bav965
Student #: 11261393

Documentation for Assignment 2 Part B
- To run the program
    + Change working directory to "xv6-riscv"
    + Run "make clean" command to clean up the directory
    + Run "make qemu" command to start the program
    + Inside qemu ternimal:
        ++ Run "test <k> <l> <m> <n> with k,l,m,n as integers greater than 0

- To change parameters "mtimes" and "moveup", please go to kernel/param.h. Make 
sure to run "make qemu" again to reflect the changes.

- Where to look at (main changes):
    + kernel/proc.c
        ++ Line 506-552

    + kernel/param.h
        ++ Line 15-16

- NOTE:
    + Since I use int to store the sum, it would not be able to hold a
    super large value (for example, the sum with k=30000 and n=3000). It still
    gives you a result but not the right one as the result bigger than what 
    "int" can hold in xv6. To see this behavior, please uncomment line 33
    in user/test.c


-  Turnaround times and estimated running times almost about the same when
moveup = 50 and mtimes = 3
    First 15 Child Processes (K, N)=(30000, 30000)
    Sum of the turnaround times is: 331
    Sum of the estimated running times is: 316

    Next 15 Child Processes (K, M)=(30000, 20000)
    Sum of the turnaround times is: 220
    Sum of the estimated running times is: 216

    Last 15 Child Processes (K, L)=(30000, 10000)
    Sum of the turnaround times is: 108
    Sum of the estimated running times is: 105

- Advantages
    ++ Aging (moveup) -> no starvation
    ++ Low scheduling overhead

- Disadvantages:
    ++ More complex since we need to choose mtimes and moveup -> need to test
    with many different values to get the best scheduler (we may not be able to 
    find ones)
    ++ More CPU overhead

- What I could do to improve the scheduler?
    Test the scheduler with different values of "mtimes" and "moveup". Record
    the results (e.g. the estimated running times) and make plots or graphs
    based on those result. Use the graphs to find a "sweet spot" that will
    generate the best result.



