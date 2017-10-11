#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()
#include <errno.h> //For perror & errno
#include <math.h>
#include <omp.h> //For parallel loops

int is_prime(int arg) {
    
    if (arg == 2) {
        return 1;
    }
    
    if (arg%2 == 0) {
        return 0;
    }
    int j = 3;
    for(; j<=sqrt(arg); j+=2) {
        if (arg%j == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    
    if(argc != 2){
        perror("Error: config requires 1 parameter\n");
        exit(-1);
    }
    
    /*
    // Code to get input and check if a number is prime
    int num = atoi(argv[1]);
    
    int prime = is_prime(num);
    
    if (prime == 1) {
        printf("%d is prime\n", num);
    }
    if (prime == 0) {
        printf("%d is not prime\n", num);
    }
    */
    
    //Code to get all primes from 1 to N
    int N = atoi(argv[1]);
    //int i = 2;
    omp_set_num_threads(5);
    #pragma omp parallel for schedule(dynamic, 25)
    for(int i=2; i <= N; i++) {
        if ( i <= 6 ) sleep(1);
        
        int prime = is_prime(i);
        /*
        if (prime == 1) {
            printf("%d is prime\n", i);
        }
        */
        int thread_num = omp_get_thread_num();
        printf("Thread_num is %d, loop index is %d\n", thread_num, i);
    }
    int max_threads = omp_get_max_threads();
    printf("max_threads is %d\n",max_threads);
}
