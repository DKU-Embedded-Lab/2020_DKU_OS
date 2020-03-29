/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bonus.c :
*       - spin lock aessembly code.
*
*   Implement spin lock in assembly level. 
*/


#include <sys/types.h>
#include <pthread.h>

#include "lab2_sync_types.h"

#define LOCK_PREFIX "lock\n\t"

/* 
 * Assembly level implementation example.  
 *
 * atomic_add
 *  @param int *value   : variable which will be added by add_val
 *  @param int add_val  : variable which will be added to *value
 */
void atomic_add( int * value, int add_val) {

    asm volatile(
            LOCK_PREFIX
            "addl %1, %0      \n\t"
            : "+m"(*value)
            : "ir"(add_val)
            : /*no clobber-list*/
            );
}

void atomic_sub( int * value, int dec_val) {

    asm volatile(
            LOCK_PREFIX
            "subl %1, %0      \n\t"
            : "+m"(*value)
            : "ir"(dec_val)
            : /*no clobber-list*/
            );

}

void atomic_inc( int * value) {

    asm volatile(
            LOCK_PREFIX
            "incl %0        \n\t"
            : "+m"(*value)
            : /* no input */
            : /*no clobber-list*/
            );

}

void atomic_dec( int * value) {

    asm volatile(
            LOCK_PREFIX
            "decl %0        \n\t"
            : "+m"(*value)
            : /* no input */
            : /*no clobber-list*/
            );

}

/* 
 * TODO - lab2 bonus
 *  Implement a function which acts like spin_lock.
 *
 *  @param int *lock        : lock variable's current status. 
 */

void lab2_spin_lock(int volatile *lock)
{
    
}
/* 
 * TODO - lab2 bonus
 *  Implement a function which acts likes spin_unlock.
 *
 *  @param int *lock        : lock variable's current status. 
 */
void lab2_spin_unlock(int volatile *lock)
{

}


