Author: Bao (Damon) Vu
NSID: bav965
Student #: 11261393

Documentation for Assignment 1 Part A

- To run the program
    + Change working directory to "xv6-riscv"
    + Run "make clean" command to clean up the directory
    + Run "make qemu" command to start the program
    + Inside qemu ternimal:
        ++ Run "cmpt332 <n>" with n is the number of times the program cmpt332 should fork itself
        For example: "cmpt 10"
        Result => Number of processes that currently exists in the system whose name begins with 'cmpt': 11
        Explain: 1 (parent) + 10 (children) = 11
        Note: I have create a helper system call named cps() to print out the table of processes for visualization.
        To enable it, please uncomment line 30, 39 and 40 inside user/cmpt332.c file

        ++ To exit Qemu: hit Ctrl + A then X

- Files created:
    + user/cmpt332.c: This is the userspace program. It has a name beginning with cmpt as required in the assignment description.
    It takes a command line argument the number of times it should fork itself to create child processes. The child processes should
    immediately exit. After forking all of the children, the parent process should invoke the new system call "howmanycmpt()", print the result,
    and wait for all children, and then exit.

- Files modified:
    + Makefile

    + kernel/syscall.c
        Line: 103-104 128-129

    + kernel/syscall.h
        Line: 23-24

    + kernel/sysproc.c
        Line: 101-111

    + kernel/proc.c
        Line: 682-734

    + kernel/defs.h
        Line: 107-108

    + user/user.h
        Line: 26-27

    + user/usys.pl
        Line: 39-40


    
