
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "lab2_sync_types.h"

double get_timeval(struct timeval *tv, struct timeval *tv_end){

    double tv_s = tv->tv_sec + (tv->tv_usec / 1000000.0);
    double tv_e = tv_end->tv_sec + (tv_end->tv_usec / 1000000.0);

    return (tv_e - tv_s);
}



