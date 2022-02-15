Author: Bao (Damon) Vu
NSID: bav965
Student #: 11261393

Documentation for Assignment 2 Part A
- To run the program
    + Change working directory to "xv6-riscv"
    + Run "make clean" command to clean up the directory
    + Run "make qemu" command to start the program
    + Inside qemu ternimal:
        ++ Run "test <k> <l> <m> <n> with k,l,m,n as integers greater than 0

- File Created:
    + user/test.c: This is the userspace program that used to test the new system call

- File Modified:
    + kernel/proc.c
        ++ Line 152 - 155
        ++ Line 359 - 363
        ++ Line 617 - 620
        ++ Line 751 - 842 (main part)

    + kernel/proc.h
    + kernel/defs.h
    + kernel/syscall.c
    + kernel/syscall.h
    + kernel/sysproc.c
    + kernel/trap.c
        ++ Line 167 (main part)
    + user/user.h
    + user/usys.pl

    + Makefile
