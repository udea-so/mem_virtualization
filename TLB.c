/*
 * File: TLB.c
 * Author: Danny Munera
 * Date: March 7, 2024
 * Description: This code implements the TLB algorithm
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 

int main(int argc, char *argv[]) {

    struct timespec start, end;
    long cpu_time;
    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s pages trials\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    
    long PAGESIZE = sysconf(_SC_PAGESIZE); // 4096
    long jump = PAGESIZE / sizeof(int);    // 1024
    int pages = atoi(argv[1]);
    int trials = atoi(argv[2]);
    
    printf("page size %ld, jump %ld", PAGESIZE, jump);
    
    int *a = calloc(pages*jump, sizeof(int));

    //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); // Get the start time
    clock_gettime(CLOCK_MONOTONIC, &start); // Get the start time


    for (int j = 0; j < trials; j++) {
        for (int i = 0; i < pages * jump; i += jump)
            a[i] += 1;
    }
    //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); // Get the end time
    clock_gettime(CLOCK_MONOTONIC, &end); // Get the end time

    // Calculate the CPU time in nanoseconds
    cpu_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

    printf("CPU time: %ld nanoseconds\n", cpu_time/(trials * pages));

    return 0;
}