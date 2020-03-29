/*
*	DKU Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched.c :
*       - Lab2 header file.
*       - It contains bst node structure, timeval functions, atomic functions .. etc.
*
*/


#ifndef _LAB2_HEADER_H
#define _LAB2_HEADER_H

#include <pthread.h>

#define LAB2_SUCCESS                0
#define LAB2_ERROR                 -1

/*
 * lab2_node
 *
 *  struct lab2_node *left  : left child link
 *  struct lab2_node *right : right child link
 *  int key                 : node key value 
 */
typedef struct lab2_node {

    pthread_mutex_t mutex;
    struct lab2_node *left;
    struct lab2_node *right;
    int key;

} lab2_node;

/*
 * lab2_tree
 *
 *  struct lab2_node *root  : root node of bst.
 */
typedef struct lab2_tree {
    struct lab2_node *root;
} lab2_tree;

/* 
 * lab2_bst_test.c related structure.  
 */
typedef struct thread_arg{
    pthread_t thread;
    lab2_tree *tree;
    int node_count;
    int num_iterations;
    int is_sync;
    int *data_set;
    int start;
    int end;
}thread_arg;

/* 
 * lab2_bst.c related functions 
 * TODO
 *  You need to implement these functions. 
 */
int lab2_node_print_inorder(lab2_tree *tree);
lab2_tree *lab2_tree_create();
lab2_node *lab2_node_create(int key);
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node);
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node);
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node);
int lab2_node_remove(lab2_tree *tree, int key);
int lab2_node_remove_fg(lab2_tree *tree, int key);
int lab2_node_remove_cg(lab2_tree *tree, int key);
void lab2_tree_delete(lab2_tree *tree);
void lab2_node_delete(lab2_node *node);


/*
 * Lab2 bonus(lab2_bonus.c)
 *  Spin Lock implementation in assembly level. 
 *
 * lab2_bonus.c related functions  
 */

/* assembly implementation example functions */
void atomic_add( int * value, int inc_val);
void atomic_sub( int * value, int dec_val);
void atomic_inc( int * value);
void atomic_dec( int * value);

/*
 * lab2_bonus.c related functions 
 * TODO
 *  You need to implement these function.
 */
void lab2_spin_lock(int volatile *lock);
void lab2_spin_unlock(int volatile *lock);


/* lab2_timeval.c related function */
double get_timeval(struct timeval *tv, struct timeval *tv_end);

#endif /* LAB2_HEADER_H*/
