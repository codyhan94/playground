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


/* TODO:  The unacceptable allocator uses an external "free-pointer" to track
 *        where free memory starts.  If your allocator doesn't use this
 *        variable, get rid of it.
 *
 *        You can declare data types, constants, and statically declared
 *        variables for managing your memory pool in this section too.
 */

/* Header struct */
typedef struct header {
    /* Negative means block is allocated */
    /* Postive means block is available */
    int size;
} header;


unsigned char *freeptr;

header *h;


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

    /* TODO:  You can initialize the initial state of your memory pool here. */
    h = (header *) mem;
    h->size = (MEMORY_SIZE - 4);
    freeptr = mem;
   
}


/*!
 * Attempt to allocate a chunk of memory of "size" bytes.  Return 0 if
 * allocation fails.
 */
unsigned char *myalloc(int size) {

    /* TODO:  The unacceptable allocator simply checks to see if there are at
     *        least "size" bytes left in the pool, and if so, the caller gets
     *        the current "free-pointer" value, and then freeptr is incremented
     *        by size bytes.
     *
     *        Your allocator will be more sophisticated!
     */
    header *new_h;
    header *current_head;
    freeptr = mem;
    int prev_size;
    unsigned char *resultptr;

    current_head = (header *)freeptr;
    new_h = (header *)freeptr;
    printf("total: %d\n", MEMORY_SIZE);
    printf("size: %d\n", size);

    while (freeptr + size  <= mem + MEMORY_SIZE - 4) {
       printf("one: %d two %d \n", freeptr + size, mem + MEMORY_SIZE - 4);
       printf("allo: %d\n", current_head->size);
        if (current_head->size >= size + 4 ) {
            resultptr = freeptr;
            prev_size = current_head->size;
            printf("before: %d\n", current_head->size);
            current_head->size = -1 * size;
            printf("after: %d\n", current_head->size);
            new_h = (header *)(freeptr + size + 4);
            new_h->size = prev_size - size - 4;
            printf("set: %d\n", current_head->size);
            printf("diff: %d\n", new_h->size);
            return (resultptr + 4);
        }
         else if (current_head->size >= size && current_head->size < size + 4 ) {
            resultptr = freeptr;
            printf("before: %d\n", current_head->size);
            current_head->size = -1 * current_head->size;
            
            printf("set: %d\n", current_head->size);
            return (resultptr + 4);
        } 
      
            printf("biter:%d %d\n", freeptr, current_head->size);
             printf("biter_size: %d\n", size);
            
            if (current_head->size > 0) {
                 freeptr = (freeptr + (current_head->size + 4));
            } else {
                 freeptr = (freeptr - current_head->size + 4);
            }
            
            current_head = (header *)freeptr;
            printf("one: %d two %d \n", freeptr, mem + MEMORY_SIZE - 4);
            printf("iter:%d %d\n", freeptr, current_head->size);
        
    
    }
   
         fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, ((unsigned char *)freeptr - mem));
        return (unsigned char *) 0;
    
    
    /*if (freeptr + size < mem + MEMORY_SIZE) {
        unsigned char *resultptr = freeptr;
        freeptr += size;
        return resultptr;
    }
    else {
        fprintf(stderr, "myalloc: cannot service request of size %d with"
                " %d bytes allocated\n", size, (freeptr - mem));
        return (unsigned char *) 0;
    } */
}

/*!
 * Free a previously allocated pointer.  oldptr should be an address returned by
 * myalloc().
 */
void myfree(unsigned char *oldptr) {
    /* TODO:
     *
     * The unacceptable allocator does nothing -- that's part of why this is
     * unacceptable!
     *
     * Allocations will succeed for a little while...
     */
     header *old = (header *)(oldptr - 4);
     int old_size;
     unsigned char *preptr;
     unsigned char *right;
     header *current_head;
     header *preptr_header;
     header *right_header;
     preptr = mem; 
     freeptr = mem;
     old_size = -1 * old->size;
     right = (oldptr + old_size);
     right_header = (header *)right;
     current_head = (header *)freeptr;
     old->size = old_size;
     printf("old_size is: %d\n", old_size);
     while (freeptr < mem + MEMORY_SIZE - 4) {
          printf("included: %d\n", current_head->size);
          preptr = freeptr;
          freeptr = freeptr + (abs(current_head->size) + 4);
          current_head = (header *)freeptr;
         
     }
     freeptr = mem;
     current_head = (header *)freeptr;
     while (freeptr != (oldptr - 4) ) {
          printf("free: %d\n", current_head->size);
          preptr = freeptr;
          freeptr = freeptr + (abs(current_head->size) + 4);
          current_head = (header *)freeptr;
         
     }
     printf("exit\n");
     preptr_header = (header *)preptr;

     if (right_header->size > 0) {
       old->size = (old_size + right_header->size + 4);
       old_size = old->size;
       printf("right size: %d\n", old_size);
     
     }
     else if (preptr_header->size > 0) {
        preptr_header->size = (preptr_header->size + old_size + 4);
        printf("left size: %d\n", preptr_header->size);
        
     }
     
     return;
}

