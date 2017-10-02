// Matt Gannon and Matt Gottsacker
// 9/27/2017
// A program to learn to use atomic instructions to synchronize access to a variable and measure the overhead costs of these instructions.

#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()
int race = 0;

void* adder(void *args) {
    int i = 0;
    for(; i<20000000; i++) {
        __sync_add_and_fetch(&race, 1);
    }
    return NULL;
}

void* subtractor(void *args) {
    int i = 0;
    for(; i<20000000; i++) {
        __sync_sub_and_fetch(&race, 1);
    }
    return NULL;
}

int main( int argc, char* argv[] ){
    
    pthread_t adder_thread;
    pthread_t subtractor_thread;
    
    int create_add_ret = pthread_create(&adder_thread, NULL, adder, NULL);
    if(create_add_ret != 0){
        printf("Error creating thread. Error #%d. Exiting.", create_add_ret);
        exit(-1);
    }
    
    int create_sub_ret = pthread_create(&subtractor_thread, NULL, subtractor, NULL);
    if(create_sub_ret != 0){
        printf("Error creating thread. Error #%d. Exiting.", create_sub_ret);
        exit(-1);
    }
    
    int join_add_ret = pthread_join(adder_thread, NULL);
    if(join_add_ret != 0){
        printf("Error joining thread. Error #%d. Exiting.", join_add_ret);
        exit(-1);
    }
    
    int join_sub_ret = pthread_join(subtractor_thread, NULL);
    if(join_sub_ret != 0){
        printf("Error joining thread. Error #%d. Exiting.", join_sub_ret);
        exit(-1);
    }
    
    printf("Race is %d\n", race);
}

