/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bonus_test.c :
*       - spin lock aessembly test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

int shared_variable=0;
int lock=0;

void lab2_bonus_usage(char *cmd)
{
    printf("\n Usage for %s : \n",cmd);
    printf("    -t: num thread, must be bigger than 0 ( e.g. 4 )\n");
    printf("    -i: iteration count, must be bigger than 0 ( e.g. 100000 ) \n");
    printf("    -s: sync mode setting (e.g. s : your implemted spin lock , o : original) \n");
}

void lab2_bonus_example(char *cmd)
{
    printf("\n Example : \n");
    printf("    #sudo %s -t 4 -i 1000000 -s o \n", cmd);
    printf("    #sudo %s -t 4 -i 1000000 -s s \n\n", cmd);

}

static void print_result(int num_threads, int num_iterations, int is_sync){
    char *cond[] = {"original(race condition)","lab2 spin lock"};


    printf("\n Experiment info \n");
    printf("    num_threads         : %d \n",num_threads);
    printf("    num_iterations      : %d \n",num_iterations);
    printf("    experiment type     : %s \n",cond[is_sync]);


    printf("\n Experiment result : \n");
    printf("    expected result     : %d \n",num_threads * num_iterations);
    printf("    result              : %d \n\n", shared_variable);

}

static void* add_shared_variable(void* arg){

    thread_arg *th_arg = (thread_arg*)arg;
    int num_iterations = th_arg->num_iterations;
    int is_sync = th_arg->is_sync;
    int i=0;

    if(is_sync){
        for(i=0; i < num_iterations ;i++){
            lab2_spin_lock(&lock);
            shared_variable++;
            lab2_spin_unlock(&lock);
        }    
    }else{
        for(i=0; i < num_iterations ;i++){
            shared_variable++;
        }    

    }
}

int your_spinlock_test(int num_threads,int num_iterations, int is_sync)
{
    pthread_t *pthreads = NULL;
    int res=LAB2_ERROR, i;
    long double result= 0.0;
    thread_arg arg;

    arg.is_sync = is_sync;
    arg.num_iterations = num_iterations;

    pthreads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
    memset(pthreads, 0x0, sizeof(pthread_t) * num_threads);

    for(i = 0 ; i < num_threads; i++){ 
        res = pthread_create(&pthreads[i], NULL, add_shared_variable,(void*)&arg);
        if(res == LAB2_ERROR){
            printf(" Error: _perf_metadata - pthread_create error \n");
            goto TEST_ERROR;
        }
    }

    for(i = 0 ; i < num_threads ; i++){
        pthread_join(pthreads[i], NULL);
    }

    print_result(num_threads, num_iterations, is_sync);

    return LAB2_SUCCESS;
TEST_ERROR:
    free(pthreads);
    return LAB2_ERROR;
}


int main(int argc, char *argv[]){

    char op;
    int num_threads=0, num_iterations=0, is_sync=LAB2_ERROR;
    int fd;

    optind = 0;

    while ((op = getopt(argc, argv, "t:i:s:")) != -1) {
        switch (op) {
            case 't':
                num_threads=atoi(optarg);
                break;
            case 'i':
                num_iterations = atoi(optarg);
                break;
            case 's':
                if(!strcmp(optarg, "o")){
                    is_sync=0;
                    break;
                }
                else if(!strcmp(optarg, "s")){
                    is_sync=1;
                    break;
                }
            default:
                goto INVALID_ARGS;
        }
    }
    if((num_threads > 0) && (num_iterations > 0) && (is_sync != LAB2_ERROR)){
        your_spinlock_test(num_threads,num_iterations,is_sync);
    }else{
        goto INVALID_ARGS;
    }

    return LAB2_SUCCESS;
INVALID_ARGS:
    lab2_bonus_usage(argv[0]);
    lab2_bonus_example(argv[0]);

    return LAB2_ERROR;
}
