#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/*
https://github.com/remzi-arpacidusseau/ostep-homework/blob/6592d3a6a0aec0179b0f4949088f59a9f52a95fc/threads-cv/main-common.c
*/

#define SIZE 64
#define DELIM ","


int verbose = 0;
long vm_size;
long pm_size;
long va = -1; 
long base_reg = -1;
long bounds_reg = -1;

#define MAX_ARGS 4
enum data_inputs {VA, SIZE_PM, BASE_REG, BOUNDS_REG};
char args[MAX_ARGS][40];
long *data[MAX_ARGS];
// default values

void usage(void);
void printInfoMem(char *sizeMem); 

long getMult(char *value_str);
long getNumValue(char *value_str);
long getPA(long va);
int getIntegerValue(char *value_str);

int main(int argc, char *argv[]) {
    // malloc
    data[SIZE_PM] = (long*)malloc(sizeof(long));
    data[BASE_REG] = (long*)malloc(sizeof(long));
    data[BOUNDS_REG] = (long*)malloc(sizeof(long));
    // default values
    

    vm_size = 1024;
    strcpy(args[BOUNDS_REG],"1k");
    pm_size = 16 * 1024;
    strcpy(args[SIZE_PM],"16k");
    /*
    // printInfoMem(args[BOUNDS_REG]);
    // printf("\n");
    // printInfoMem(args[SIZE_PM]);
    int c;
    // https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html

    while ((c = getopt(argc, argv, "A:a:p:b:l:hv")) != -1)
    {
        // printf("optarg = %s\n",optarg);
        switch (c)  {
        case 'A':
            //va = getNumValue(optarg);
            strcpy(args[VA], optarg); // args[0] = va (Virtual Address)
            break;
        case 'p':
            //pm_size = getNumValue(optarg);
            strcpy(args[SIZE_PM], optarg); // args[2] = p (Physical Memory Size)
            break;
        case 'b':
            //base_reg = getNumValue(optarg);
            strcpy(args[BASE_REG], optarg); // args[3] = b (Base Register)
            break;
        case 'l':
            //vm_size = getNumValue(optarg);
            strcpy(args[BOUNDS_REG], optarg); // args[1] = a (Address Space Size)
            break;
        case 'h':
            usage();
            exit(0);
        }
    }
    */
    
    strcpy(args[SIZE_PM],"64k");
    strcpy(args[VA], "0,1k,3300,4400");
    strcpy(args[BASE_REG],"16k");
    strcpy(args[BOUNDS_REG],"4k");
    

    // Verifica que los requerimientos minimos se cumplan


    
    assert(strlen(args[VA]) != 0);
    assert(strlen(args[BASE_REG]) != 0);
    assert(strlen(args[BOUNDS_REG]) != 0);
    int num_VAs = 0;
    for(int i = 0; i < MAX_ARGS; i++) {
        if(i == VA) {
            // split VA
            char str_VAs[strlen(args[i]) + 1]; // Allocate space for copy
            strcpy(str_VAs, args[i]);          // Create a copy
            char *ptr = strtok(str_VAs, DELIM);            
            while (ptr != NULL) {
                num_VAs++;
                // printf("%s\n", ptr);
                ptr = strtok(NULL, DELIM);
            }  
            // malloc
            data[VA] = (long*)malloc(num_VAs*sizeof(long));
            // reinit prt 
            ptr = strtok(args[i], DELIM);
            for(int j = 0; j < num_VAs; j++) {
                // printf("%s - ", ptr);
                // printf("%ld\n", getNumValue(ptr));
                *(*(data + i) + j) = getNumValue(ptr); // data[VA][i] = getNumValue(ptr); 
                printf("%ld ", data[i][j]);
                ptr = strtok(NULL, DELIM);
            }   
            printf("\n");
        }
        else {
            *(*data + i) = getNumValue(args[i]); // *data[i] = getNumValue(args[i]);
            printf("%ld\n", *(*data + i));
        }
    }
    printf("---------------------------\n");
    // long pa = getPA(data[VA][0]);
    
    long* pa = (long*)malloc(num_VAs*sizeof(long));
    int n_PA = (int)log2(*(*data + SIZE_PM));      // int n_PA = (int)log2(*data[SIZE_PM]);
    int n_VA = (int)log2(*(*data + BOUNDS_REG));    // int n_VA = (int)log2(*data[BOUNDS_REG]);
    for(int i = 0; i < num_VAs; i++) {
        printf("%ld\n", *(*(data + VA) + i));
        //*(pa + i) = getPA(data[VA][i]);
    }
    printf("---------------------------\n");
    char phys_addr[SIZE], virt_addr[SIZE];
     
    for(int i = 0; i < num_VAs; i++) {
      // ltoa(virt_addr,data[VA],size_VM);
      sprintf(virt_addr, "%lX", data[0][i]);
      // ltoa(phys_addr,data[SIZE_PM],size_PM);
      sprintf(phys_addr, "%lX", pa[i]);   
      printf("VA = 0x%.*s (decimal: %ld)\n", (int)(n_VA/4), virt_addr, data[VA][i]);
      //printf("PA = 0x%.*s (decimal: %ld)\n", (int)(n_PA/4), phys_addr, pa[i]);
    }
    
     
    // free 
    free(data[SIZE_PM]);
    free(data[BASE_REG]);
    free(data[BOUNDS_REG]);
    free(data[VA]);
    free(pa);
    return 0;
}

// Common usage() prints out stuff for command-line help
void usage(void)
{
    fprintf(stderr, "usage: \n");
    fprintf(stderr, "  -A <address>\n");
    fprintf(stderr, "  -a <Address Space Size> default=\"1k\",(e.g.,16, 64k, 32m, 1g)\n");
    fprintf(stderr, "  -p <physical memory size> default=\"16k\" (e.g., 16, 64k, 32m, 1g)\n");
    fprintf(stderr, "  -b <Base Register>\n");
    fprintf(stderr, "  -l <Base Register>\n");
    // fprintf(stderr, "  -v [ verbose flag: trace what is happening and print it ]\n");
    //  fprintf(stderr, "  -t [ timing flag: time entire execution and print total time ]\n");
    // exit(1);
}

// https://www.ibm.com/docs/es/i/7.5?topic=functions-strncpy-copy-strings
// https://www.ibm.com/docs/es/i/7.5?topic=files-stringh#stringh
int getIntegerValue(char *value_str) {
    char num_str[SIZE];
    int end_index = strlen(value_str) - 1;
    if(isalpha(value_str[end_index])) {     
        strncpy(num_str, value_str, end_index);
        num_str[end_index] = '\0'; // *(num_str + end_index) = '\0';
    }
    else {
        strcpy(num_str, value_str);
    }
    return atoi(num_str);
}

long getMult(char *value_str) {
    int end_index = strlen(value_str) - 1;
    char char_mult = value_str[end_index];
    long mult = 1L;
    if (isalpha(char_mult)) {
        switch (char_mult) {
        case 'k':
            mult = 1024L;
            break;
        case 'm':
            mult = 1024 * 1024L;
            break;
        case 'g':
            mult = 1024 * 1024 * 1024L;
            break;
        default:
            break;
        }
        
    }
    return mult;
}

long getNumValue(char *value_str) {
    return getIntegerValue(value_str)*getMult(value_str);
}

long getPA(long va) {
    long pa;
    // check limits 
    if(va < *(*data + BOUNDS_REG)) {
        pa = *(*data + BASE_REG) + va;
    }
    else {
        pa = -1; // ERROR
    }
    return pa;
}

void printInfoMem(char *sizeMem) {
    char num_str[SIZE];
    int end_index = strlen(sizeMem) - 1;
    strncpy(num_str, sizeMem, end_index);
    char units_mult = sizeMem[end_index];    
    num_str[end_index] = '\0'; 
    units_mult = isalpha(units_mult) ? toupper(units_mult) : '\0';

    printf("-------------- 0KB\n");
    printf("|            |\n");
    printf("|            |\n");
    printf("|            |\n");
    printf("|            |\n");
    printf("|            |\n");
    printf("|            |\n");
    printf("|            |\n");
    printf("-------------- %s%cB\n",num_str,units_mult);
}
