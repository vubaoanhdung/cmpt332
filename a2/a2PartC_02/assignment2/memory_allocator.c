/*
    Author: Damon(Bao) Vu
    Student #: 11261393
    NSID: bav965

    Partner: Eyan Cunningham
    NSID: esc568
*/

// vscode fix for MAP_ANONYMOUS
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "memory_allocator.h"
#include "common.h"


static header_node_t *head = NULL;
static header_node_t *next = NULL;
static void *original_addr = NULL;

// Size of the header and footer
const int METADATA_SIZE = sizeof(header_node_t) + sizeof(footer_node_t);
const long long MAGIC = 0x0123456789abcdf;

// ----------

int M_Init(int size) {

    // check if there exists head (e.g. if M_Init has been called before)
    if (head != NULL) {
        printf("Head already existed\n");
        return -1;
    }

    // header and footer are going to take 32 bytes
    // therefore, anything less than 32 is invalid
    if (size < 32) {
        printf("Init Failed. Size is too small");
        return -1;
    }

    
    void *addr = (header_node_t *) mmap(NULL,(size_t) size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    
    if (addr == MAP_FAILED) {
        printf("mmap() failed\n");
        return -1;
    }

    // calculate the actual free space (after taking into account the header and footer)
    int free_space = size - METADATA_SIZE;

    head = (header_node_t *) addr;
    original_addr = addr;
    head->size = free_space;
    head->next = NULL;
    printf("Initial Header Address: %p\n", head);

    next = (header_node_t *) head;

    footer_node_t *foot = (footer_node_t *) (addr + sizeof(header_node_t) + free_space);
    foot->size = free_space;
    foot->prev = NULL;
    printf("Initial Footer Address: %p\n", foot);

    return 0;

}

// ----------

void *M_Alloc(int size) {
    // TODO: M_Alloc now using first-fit.
    // Need to use next pointer to make it next-fit

    if (next == NULL) {
        next = head;
    }

    int total_size = round_up_helper(size) + METADATA_SIZE;

    // search for a first-fit (first free chunk that can be used to alloc total_size)
    // TODO: need to use search_helper(next, total_size) for next-fit
    header_node_t *temp = (header_node_t *) search_helper(next, total_size);
    if (temp == NULL) {
        return NULL;
    }
    
    // Doing the memory alloc
    header_t *h;
    footer_t *f;
    void *ptr;

    h = (header_t *) ((void *) temp);
    ptr = (void *) ((void *) h + sizeof(header_t));
    f = (footer_t *) ((void *) ptr + round_up_helper(size));
    
    int new_free_space = temp->size - total_size;

    if ((void *)temp == (void *)head) {
        // Update header
        header_node_t *old_temp_next = temp->next;
        head = (header_node_t *)((void *)h + total_size);
        head->size = new_free_space;
        head->next = old_temp_next;    

        next = head;  

        // Update footer
        footer_node_t *foot = (footer_node_t *)((void *) head + sizeof(header_node_t) + new_free_space);
        foot->size = new_free_space;
        foot->prev = NULL;
    } else {
        footer_node_t *temp_footer = (footer_node_t *)((void *)temp + sizeof(header_node_t) + temp->size);
        header_node_t *before_temp_header = (header_node_t *)((void *)temp_footer->prev);
        header_node_t *header = (header_node_t *)((void *)temp + total_size);
        header->next = temp->next;
        header->size = temp->size - total_size;
        footer_node_t *footer = (footer_node_t *)((void *)header + header->size);
        footer->size = temp->size - total_size;
        footer->prev = (header_node_t *)((void *) before_temp_header);
        before_temp_header->next = (header_node_t *)((void *)header);
        next = header;

    }

    // update h and f
    h->size = round_up_helper(size);
    h->magic = MAGIC;
    
    f->size = round_up_helper(size);
    f->magic = MAGIC;
    
    return ptr;
}

// ----------

int M_Free(void *p) {
    
    header_t *h = (header_t *)((void *) p - sizeof(header_t));
    if (h->magic != MAGIC) {
        printf("Invalid pointer!\n");
        return -1;
    }

    header_node_t *new_head;
    footer_node_t *new_foot;

    new_head = (header_node_t *)((void *) h);
    new_foot = (footer_node_t *)((void *) h + h->size);
    int new_size = h->size;

    new_head->size = new_size;
    new_foot->size = new_size;

    new_head->next = head;
    new_foot->prev = NULL;

    footer_node_t *foot = (footer_node_t *)((void *)head + sizeof(header_node_t) + head->size);
    foot->prev = (header_node_t *)((void *) new_head);
    
    head = new_head;

    // TODO: Coalesce
    footer_t *top_magic_check;
    header_t *bottom_magic_check;
    header_node_t *top_neighbour_head;
    footer_node_t *top_neighbour_foot;
    header_node_t *bottom_neighbour_head;
    footer_node_t *bottom_neighbour_foot;
    bool top_free;
    bool bottom_free;
    
    top_magic_check = (footer_t *)((void *) new_head - sizeof(footer_t));
    bottom_magic_check = (header_t*)((void *) new_foot + sizeof(header_t));
    
    bottom_free = (bottom_magic_check->magic != MAGIC);
    if((void*) new_head <= (void *) original_addr){
        top_free = false;
    }else{
        top_free = (top_magic_check->magic != MAGIC);
    }

    if(top_free){
        top_neighbour_foot = (footer_node_t *)((void *) new_head - sizeof(footer_node_t));
        top_neighbour_head = (header_node_t *)((void *) top_neighbour_foot - top_neighbour_foot->size - sizeof(header_node_t));
    }
    if(bottom_free){
        bottom_neighbour_head = (header_node_t *)((void *) new_foot + sizeof(footer_node_t));
        bottom_neighbour_foot = (footer_node_t *)((void *) bottom_neighbour_head + bottom_neighbour_head->size + sizeof(header_node_t));
    }



    if(top_free && bottom_free){
        int new_size = top_neighbour_head->size + new_head->size + bottom_neighbour_head->size + METADATA_SIZE;

        head = top_neighbour_head;
        head->size = new_size;

        if((void *) bottom_neighbour_foot->prev == (void *) head){
            head->next = NULL;
        }else{
            head->next = top_neighbour_head->next;
        }
        
        bottom_neighbour_foot->size = new_size;
        bottom_neighbour_foot->prev = NULL;

        memset((void *) head + sizeof(header_node_t), 0x00, new_size);

    }

    else if(top_free && !bottom_free){
    new_size = new_head->size + top_neighbour_foot->size + METADATA_SIZE;

    footer_node_t *top_neighbour_next_foot;

    if(top_neighbour_foot->prev != NULL){
        //check for only one item in free list
        if(top_neighbour_head->next == NULL){
            //check for end of free list
            top_neighbour_foot->prev->next = NULL;
        }else{
            top_neighbour_foot->prev->next = top_neighbour_head->next;
            top_neighbour_next_foot = (footer_node_t *)((void *) top_neighbour_head->next + top_neighbour_head->next->size + sizeof(header_node_t));
            top_neighbour_next_foot->prev = top_neighbour_foot->prev;
        }
    }

    top_neighbour_head->next = new_head->next;
    top_neighbour_next_foot = (footer_node_t *)((void *) top_neighbour_head->next + top_neighbour_head->next->size + sizeof(header_node_t));
    top_neighbour_next_foot->prev = (header_node_t *)((void *) foot);
    head = top_neighbour_head;

    top_neighbour_head->size = new_size;
    new_foot->size = new_size;


    memset((void *) head + sizeof(header_node_t), 0x00, new_size);
    }




    else if(!top_free && bottom_free){
        if (bottom_neighbour_head == next){
            next = new_head;
        }
        int new_size = new_head->size + bottom_neighbour_head->size + METADATA_SIZE;


        if (bottom_neighbour_head->next == NULL){
            //hit end of list
            bottom_neighbour_foot->prev = new_foot->prev;
        }
        if (bottom_neighbour_head->next != NULL){
            new_head->next = bottom_neighbour_head->next;
        }

        /*
        if(bottom_neighbour_foot->prev != NULL){
            header_node_t *temp_header = (header_node_t *)((void *) bottom_neighbour_foot->prev - bottom_neighbour_foot->size - sizeof(header_node_t));
            temp_header->next = head;
        }  
        */

        new_head->size = new_size;
        bottom_neighbour_foot->size = new_size;
        
       memset((void *) new_head + sizeof(header_node_t), 0x00, new_size);

    }
    else{
        //no coalesce needed
    }

    header_node_t *testheadend = head;
    return 0;
}

// ----------

void M_Display() {
    header_node_t *temp_head = head;
    int counter = 1;

    while(temp_head != NULL) {
        printf("\n");
        printf("Free Chunk #%d\n", counter);
        printf("Address: %p\n", temp_head); 
        printf("Size: %d\n", temp_head->size);
        printf("\n----------\n");

        counter++;
        temp_head = temp_head->next;
        
    }
}

// ----------

int round_up_helper(int number) {
    
    int remainder = number % 16;

    if (remainder == 0) {
        return number;
    } else {
        return number + 16 - remainder;
    }
}

void *search_helper(void *start, int total_size) {

    header_node_t *original_ptr = (header_node_t*) (start);
    header_node_t *temp = (header_node_t *) (start);
    while (temp != NULL) {
        if(temp->size == total_size){
            if (temp->next != NULL){
                next = temp->next;
            }
            else{
                next = head;
            }
        }
        if (temp->size >= total_size) {
            break;
        } else {
            temp = temp->next;
        }
    }
    if(temp == NULL){
        temp = head;
        while((void *) temp > (void *) original_ptr){
            if(temp->size >= total_size){
                break;
            }else{
                temp = temp->next;
            }
        }
    }
    // if there is no free chunk that can be used to alloc -> return NULL
    if (temp->size < total_size) {
        return NULL;
    }

    return (void *) temp;
}

/*
    For testing purposes
*/
int main(int argc, char const *argv[])
{
    printf("\n\nTesting the sizes of header and footer\n");

    printf("Size of Header: %lu\n", sizeof(header_t));
    printf("Size of Header Node: %lu\n", sizeof(header_node_t));
    printf("Size of Footer: %lu\n", sizeof(footer_t));
    printf("Size of Footer Node: %lu\n", sizeof(footer_node_t));
    printf("\n");

    M_Init(1024);
    void *p1 = M_Alloc(15); // eventually becomes 16 (since went through round_up_helper)
    void *p2= M_Alloc(128);
    void *p3 = M_Alloc(64);
    void *p4 = M_Alloc(256);
    M_Display();

    printf("\n###################\n");

    M_Free(p1);
    M_Free(p2);
    M_Display();

    printf("\n###################\n");

    // Bug -> Infinite loop
    // void *p5 = M_Alloc(16);
    // M_Free(p5);
    // M_Display();

    printf("\n\nEnd Testing!\n\n");

    return 0;
}
