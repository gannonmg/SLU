// Matt Gannon and Matt Gottsacker
// 9/27/2017
// A program to learn about the inner workings of mutex in locking race conditions

#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()

pthread_mutex_t m;
int race = 0;

void* adder(void *args) {
    int i = 0;
    pthread_mutex_lock(&m);
    for(; i<20000000; i++) {
        race++;
    }
    pthread_mutex_unlock(&m);
    return NULL;
}

void* subtractor(void *args) {
    int i = 0;
    pthread_mutex_lock(&m);
    for(; i<20000000; i++) {
        race--;
    }
    pthread_mutex_unlock(&m);
    return NULL;
}

int main( int argc, char* argv[] ){
    
    //Create mutex
    int mutex_ret = pthread_mutex_init(&m, NULL);
    
    
    pthread_t adder_thread;
    pthread_t subtractor_thread;
    
    int create_add_ret = pthread_create(&adder_thread, NULL, adder, &m);
    if(create_add_ret != 0){
        printf("Error creating thread. Error #%d. Exiting.", create_add_ret);
        exit(-1);
    }
    
    int create_sub_ret = pthread_create(&subtractor_thread, NULL, subtractor, &m);
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

