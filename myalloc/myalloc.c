/*! \file
 * Implementation of a simple memory allocator.  The allocator manages a small
 * pool of memory, provides memory chunks on request, and reintegrates freed
 * memory back into the pool.
 *
 * Adapted from Andre DeHon's CS24 2004, 2006 material.
 * Copyright (C) California Institute of Technology, 2004-2010.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>

#include "myalloc.h"


/*!
 * These variables are used to specify the size and address of the memory pool
 * that the simple allocator works against.  The memory pool is allocated within
 * init_myalloc(), and then myalloc() and free() work against this pool of
 * memory that mem points to.
 */
int MEMORY_SIZE;
unsigned char *mem;



/* Header struct that we use to contain
 * the information of our block such as 
 * the size. 
*/
typedef struct header {
    /* Negative means block is allocated */
    /* Postive means block is available */
    int size;
} header;





/*!
 * This function initializes both the allocator state, and the memory pool.  It
 * must be called before myalloc() or myfree() will work at all.
 *
 * Note that we allocate the entire memory pool using malloc().  This is so we
 * can create different memory-pool sizes for testing.  Obviously, in a real
 * allocator, this memory pool would either be a fixed memory region, or the
 * allocator would request a memory region from the operating system (see the
 * C standard function sbrk(), for example).
 */


void init_myalloc() {

    /*
     * Allocate the entire memory pool, from which our simple allocator will
     * serve allocation requests.
     */
    mem = (unsigned char *) malloc(MEMORY_SIZE);
    if (mem == 0) {
        fprintf(stderr,
                "init_myalloc: could not get %d bytes from the system\n");
        abort();
    }

    /* initialize the initial state of the memory pool
     * we assign our first header to be the first 4 bytes 
     * of memory and then the size of it to be the 
     * MEMORY_SIZE - 4 bytes. 
     */
    header *h;

    h = (header *) mem;
    h->size = (MEMORY_SIZE - 4);
    
   
}


/*!
 * Attempt to allocate a chunk of memory of "size" bytes.
 * Return 0 if allocation fails.
 * This function uses First fit strategy where it
 * starts at the beginning of the list and finds the first
 * free block. This allocation runs in linear time.
 * because we search through the list from beginning till
 * we hit a free spot. This would be good in when we just have 
 * to fill in a spot of memory quickly and want to use
 * the first part of our memory as much as possible.
 * However the downside to this approach is the fact we
 * might be just wasting a big chunk of memory that could 
 * have held a bigger piece of memory. 
 * It would also be a waste of time to start from
 * the beginning of the list each time.. it be better
 * to just know where all the free pointers are. 
 */
unsigned char *myalloc(int size) {
    unsigned char *freeptr; /* points to the address of current head */
    header *current_head; /* header of the current address */
    header *new_h; /* new header that is being created */
    freeptr = mem; /* sets our freeptr to beginning of memory pool */
    int prev_size; /* keeps the previous size of the current header */
    unsigned char *resultptr; /* contains the freeptr that we allocated */
    
    /* Assign the headers to the header of freeptr (first block of memory) */
    current_head = (header *)freeptr;
    new_h = (header *)freeptr;
  
   /* Checks if our current freeptr + size is still within our memory */
    while (freeptr + size  <= mem + MEMORY_SIZE - 4) {
        /* Checks to see if current header has room if we can allocate
         * header + size we want to input. 
         */
        if (current_head->size >= size + 4) {
            /* if true we can set the resultptr to freeptr */
            resultptr = (freeptr + 4);
            prev_size = current_head->size;
            /* change current_head->size to indicate allocated */
            current_head->size = -1 * size;
            /* create a new header to represent the amount of free
             * blocks left
             */
            new_h = (header *)(freeptr + size + 4);
            /* gets remaning size left from allocating a new block
             * and sets it to the new headers size.
             */
            new_h->size = prev_size - size - 4;
            
            return resultptr;
        }
        /* Checks to see if our current head size can hold only 
         * between our exact size or +3 of that
         * If this is true we just negate the current_head->size
         * and just waste at most 3 bytes of space. 
         * then we return our resultptr which is the payload of it.
         */
        else if (current_head->size >= size && current_head->size < size + 4) {
            resultptr = (freeptr + 4);
            
            current_head->size = -1 * current_head->size;
            
           
            return resultptr;
        } 
      
        /* If that header indicated we had no room we increment
         * to the next header and repeat we use abs in case negative
         */
            
        freeptr = freeptr + (abs(current_head->size) + 4);
            
        current_head = (header *)freeptr;
         
        
    
    }
    /* If never find an empty spot we return 0 */
      
     fprintf(stderr, "myalloc: cannot service request of size %d with"
            " %d bytes allocated\n", size, ((unsigned char *)freeptr - mem));
     return (unsigned char *) 0;
    

}

/*!
 * Free a previously allocated pointer.  oldptr should be an address returned by
 * myalloc().
 * myfree takes linear time it takes O(n) to find the previous pointer
 * and it takes constant time to coalesce. 
 */
void myfree(unsigned char *oldptr) {
    
     unsigned char *freeptr; /* points to the address of current head */
     header *old; /* points to the header address of oldptr */
     int old_size; /* stores size of oldptr */
     unsigned char *preptr; /* stores address of previous header of oldptr */
     unsigned char *right; /* stores address of right/next header of oldptr */
     header *current_head; /* stores header of the freeptr */
     header *preptr_header; /* stores header of preptr */
     header *right_header; /* stores header of right */
     old = (header *)(oldptr - 4); /* Set old to header of oldptr */
     preptr = mem; /* assign to beginning of memory pool */
     freeptr = mem; /* assign to beginning of memory pool */
     old_size = -1 * old->size; /* calculate size of old_ptr and free it */
     right = (oldptr + old_size); /* set the right to the right of oldptr */
     right_header = (header *)right; /* set header of right */
     current_head = (header *)freeptr; /* set current ptrs header */
     old->size = old_size; /* change old->size to indicate it free */
     
  while (freeptr < mem + MEMORY_SIZE - 4) {
     	          printf("%d\n", current_head->size);
          preptr = freeptr;
          freeptr = freeptr + (abs(current_head->size) + 4);
          current_head = (header *)freeptr;
         
     }
     
     
     freeptr = mem;
     current_head = (header *)freeptr;
     
  
     while (freeptr != (oldptr - 4) ) {
          printf("%d\n", current_head->size);
          preptr = freeptr;
          //if (freeptr >= mem + MEMORY_SIZE) {
           //      break;
          //}
          freeptr = freeptr + (abs(current_head->size) + 4);
          current_head = (header *)freeptr;    
     }
   
     preptr_header = (header *)preptr;

     if (right_header->size > 0) {
       old->size = (old_size + right_header->size + 4);
       old_size = old->size;
      
     
     }
     else if (preptr_header->size > 0) {
        preptr_header->size = (preptr_header->size + old_size + 4);
        
        
     }
     
     return;
}

