/*
    Author: Damon(Bao) Vu
    Student #: 11261393
    NSID: bav965

    Partner: Eyan Cunningham
    NSID: esc568
*/

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

typedef struct __header_t {
    int size;
    long long magic;
} header_t;

typedef struct __footer_t {
    int size;
    long long magic;
} footer_t;

typedef struct __header_node_t {
    int size;
    struct __header_node_t *next;
} header_node_t;

typedef struct __footer_node_t {
    int size;
    struct __header_node_t *prev;
} footer_node_t;


/*
    Initialize memory allocator
    Input: 
        -- size: integer number, the size of the momory space (in bytes) that
        the allocator should manage
    Output:
        -- 0: on Success
        -- -1: on Failure
*/
int M_Init(int size);
    

/*
    Allocate a chunk of memory
    Input:
        -- size: integer number represents the requested size in bytes
    Output:
        -- void pointer to the allocated address space (void *ptr)
*/
void *M_Alloc(int size);


/*
    Free the chunk with address p
    Input:
        -- void pointer p (void *p)
    Output:
        -- 0: on Success
        -- -1: on Failure
*/
int M_Free(void *p);


/*
    Print the addresses and sizes of the free chunks to stdout
    Input: None
    Output: None
*/
void M_Display();

#endif