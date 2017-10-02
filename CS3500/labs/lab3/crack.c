// Matt Gannon
// 9/29/2017
// A program to use threading to crack hash-encrypted passwords

#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()

int main( int argc, char* argv[] ){
    
    int* threads = argv[1];
    
    
    
}
