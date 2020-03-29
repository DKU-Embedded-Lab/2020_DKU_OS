/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*
*   lab2_bst_test.c :
*       - thread-safe bst test code.
*       - coarse-grained, fine-grained lock test code
*
* You can compare single thread result, coarse grained result and fine grained result.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "lab2_sync_types.h"

#define LAB2_TYPE_FINEGRAINED       0
#define LAB2_TYPE_COARSEGRAINED     1
#define LAB2_TYPE_SINGLE            2

#define LAB2_OPTYPE_INSERT          0
#define LAB2_OPTYPE_DELETE          1

void lab2_sync_usage(char *cmd)
{
    printf("\n Usage for %s : \n",cmd);
    printf("    -t: num thread, must be bigger than 0 ( e.g. 4 )\n");
    printf("    -c: test node count, must be bigger than 0 ( e.g. 10000000 ) \n");
}

void lab2_sync_example(char *cmd)
{
    printf("\n Example : \n");
    printf("    #sudo %s -t 4 -c 10000000 \n", cmd);
    printf("    #sudo %s -t 4 -c 10000000 \n", cmd);
}

static void print_result(lab2_tree *tree,int num_threads,int node_count ,int is_sync, int op_type ,double time){
    char *cond[] = {"fine-grained BST  ", "coarse-grained BST", "single thread BST"};
    char *op[] = {"insert","delete"};
    int result_count=0;


    printf("=====  Multi thread %s  %s experiment  =====\n",cond[is_sync],op[op_type]);
    printf("\n Experiment info \n");
    printf("    test node           : %d \n",node_count);
    printf("    test threads        : %d \n",num_threads);
    printf("    execution time      : %lf seconds \n\n",time);

    printf("\n BST inorder iteration result : \n");
    result_count=lab2_node_print_inorder(tree);
    printf("    total node count    : %d \n\n",node_count);


}

void* thread_job_delete(void *arg){

    thread_arg *th_arg = (thread_arg *)arg;
    lab2_tree *tree = th_arg->tree;
    int is_sync = th_arg->is_sync;
    int *data_set = th_arg->data_set;
    int start = th_arg->start, end = th_arg->end;
    int i;
    for(i=start ; i < end; i++ ){
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            lab2_node_remove_fg(tree, data_set[i]);        
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_remove_cg(tree, data_set[i]);
    }
}

void* thread_job_insert(void *arg){

    thread_arg *th_arg = (thread_arg *)arg;
    lab2_tree *tree = th_arg->tree;
    int is_sync = th_arg->is_sync;
    int *data = th_arg->data_set;
    int start = th_arg->start, end = th_arg->end;
    int i;

    for (i=start ; i < end ; i++) {               
        lab2_node* node = lab2_node_create(data[i]);
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            lab2_node_insert_fg(tree, node);
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_insert_cg(tree, node);
    }
}

void bst_test(int num_threads,int node_count){

    lab2_tree *tree;
    lab2_node *node;    
    struct timeval tv_insert_start, tv_insert_end, tv_delete_start, tv_delete_end, tv_start, tv_end;
    int errors,i=0,count=0;
    int root_data = 40; 
    int term = node_count / num_threads, is_sync;
    double exe_time=0.0;
    thread_arg *threads;
    int *data = (int*)malloc(sizeof(int)*node_count);

    srand(time(NULL));
    for (i=0; i < node_count; i++) { 
        data[i] = rand();
    }

    if (!(threads = (thread_arg*)malloc(sizeof(thread_arg) * num_threads)))
        abort();

    /*
     * single thread insert test.
     */
    gettimeofday(&tv_start, NULL);
    printf("\n");
    tree = lab2_tree_create();
    for (i=0 ; i < node_count ; i++) {               
        lab2_node* node = lab2_node_create(data[i]);
        lab2_node_insert(tree, node);
    }

    gettimeofday(&tv_end, NULL);
    exe_time = get_timeval(&tv_start, &tv_end);
    print_result(tree,num_threads, node_count, LAB2_TYPE_SINGLE,LAB2_OPTYPE_INSERT ,exe_time);
    lab2_tree_delete(tree);

    /* 
     * multi therad insert test coarse-grained 
     */
    is_sync = LAB2_TYPE_COARSEGRAINED;
    tree = lab2_tree_create();

    gettimeofday(&tv_insert_start, NULL);
    for(i=0; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_insert_end, NULL);
    exe_time = get_timeval(&tv_insert_start, &tv_insert_end);
    print_result(tree,num_threads, node_count, is_sync,LAB2_OPTYPE_INSERT ,exe_time);
    lab2_tree_delete(tree);

    /*
     *  multi thread insert test fine-grained \
     */
    is_sync = LAB2_TYPE_FINEGRAINED;
    tree = lab2_tree_create();

    gettimeofday(&tv_insert_start, NULL);
    for(i=0; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_insert_end, NULL);
    exe_time = get_timeval(&tv_insert_start, &tv_insert_end);
    print_result(tree,num_threads, node_count, is_sync, LAB2_OPTYPE_INSERT,exe_time);
    lab2_tree_delete(tree);
    
    /* 
     * single thread delete test
     */

    tree = lab2_tree_create();
    for (i=0 ; i < node_count ; i++) {               
        lab2_node* node = lab2_node_create(data[i]);
        lab2_node_insert(tree, node);
    }

    gettimeofday(&tv_start, NULL);
    for(i=0 ; i < node_count ; i++){
        lab2_node_remove(tree,data[i]);
    }

    gettimeofday(&tv_end, NULL);
    exe_time = get_timeval(&tv_start, &tv_end);
    print_result(tree ,num_threads, node_count, LAB2_TYPE_SINGLE, LAB2_OPTYPE_DELETE,exe_time);
    lab2_tree_delete(tree);
    
    /* 
     * multi thread delete test coarse-grained  
     */
    is_sync = LAB2_TYPE_COARSEGRAINED;
    tree = lab2_tree_create();

    for (i=0; i < node_count; i++) { 
        node = lab2_node_create(data[i]);
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            lab2_node_insert_fg(tree,node);
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_insert_cg(tree,node);
    }            
    
    gettimeofday(&tv_delete_start, NULL);
    for(i=0 ; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);
    gettimeofday(&tv_delete_end, NULL);
    exe_time = get_timeval(&tv_delete_start, &tv_delete_end);

    print_result(tree,num_threads, node_count, is_sync,LAB2_OPTYPE_DELETE,exe_time);
    lab2_tree_delete(tree);

    /* 
     * multi thread delete test fine-grained  
     */
    is_sync = LAB2_TYPE_FINEGRAINED;
    tree = lab2_tree_create();
    for (i=0; i < node_count; i++) { 
        node = lab2_node_create(data[i]);
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            lab2_node_insert_fg(tree,node);
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_insert_cg(tree,node);
    }

    gettimeofday(&tv_delete_start, NULL);
    for(i=0 ; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_delete_end, NULL);
    exe_time = get_timeval(&tv_delete_start, &tv_delete_end);

    print_result(tree ,num_threads, node_count, is_sync, LAB2_OPTYPE_DELETE,exe_time);
    lab2_tree_delete(tree);

    printf("\n");

    free(threads);
    free(data);
}

int main(int argc, char *argv[]) 
{
    char op;
    int num_threads=0, node_count=0;
    int fd;

    optind = 0;

    while ((op = getopt(argc, argv, "t:c:")) != -1) {
        switch (op) {
            case 't':
                num_threads=atoi(optarg);
                break;
            case 'c':
                node_count = atoi(optarg);
                break;
            default:
                goto INVALID_ARGS;
        }
    }
    if((num_threads>0) && (node_count > 0)){
        bst_test(num_threads,node_count);
    }else{
        goto INVALID_ARGS;
    }

    return LAB2_SUCCESS;
INVALID_ARGS:
    lab2_sync_usage(argv[0]);
    lab2_sync_example(argv[0]);

    return LAB2_ERROR;
}

