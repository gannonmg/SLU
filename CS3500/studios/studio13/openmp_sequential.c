#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()
#include <errno.h> //For perror & errno
#include <math.h>

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
        perror("Error: openmp requires 1 parameter\n");
        printf("errno: %d\n", errno);
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
    int i = 2;
    for(; i <= N; i++) {
        int prime = is_prime(i);
        /*
        if (prime == 1) {
            printf("%d is prime\n", i);
        }
        */
    }
    
}
