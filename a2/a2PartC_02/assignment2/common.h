#ifndef COMMON_H
#define COMMON_H

/*
    Round a number up to the nearest multiple of 16 (bytes)
    Input:
        -- number: integer number to be rounded up
    Output:
        -- the rounded number (which is a multiple of 16)
*/
int round_up_helper(int number);

/*
    Search the free list for the first appear that can be used
    to alloc total_size of memory
    Input:
    -- void *start: the starting point (ideally, the next pointer) in the free list
    -- total_size: the requested size
    Output:
    -- void *target: the pointer to the location satisfied the requirement
*/
void *search_helper(void *start, int total_size);

#endif