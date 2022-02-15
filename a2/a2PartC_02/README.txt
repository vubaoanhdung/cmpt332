Author: Bao (Damon) Vu
NSID: bav965
Student #: 11261393

Documentation for Assignment 2 Part C
- To run the program
    + Change working directory to "assignment2"
    + Modify the main function inside memory_allocator.c as you want
    + Run "make clean"
    + Run "make"
    + Run "app"

- Note:
    + The current version of the memory_allocator works as expected.
    However, it is not a final version as it wasn't throughly tested.
    There is still a bug, expectially when user tries to allocate a chunk
    of memory after he/she has freed a chunk before (head got updated). It will
    go through a endless loop if the user do so and call M_Display(). To see
    this behavior/bug, please uncomment line 388-391 in memory_allocator.c
