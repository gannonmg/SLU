// Matt Gannon & Matt Gottsacker
// 9/23/2017
// Program to learn and use threading

#include <pthread.h> // for pthread_create()
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()
#define buffSize 2048

struct thread_struct{
    int arg1;
    char arg2[buffSize];
    int ret;
};

void* thread_entry(void* args) {
    printf("Starting thread entry function...\n");

    struct thread_struct* arg_ptr = (struct thread_struct*) args;
    printf("thread arg1: %d\n", arg_ptr->arg1);
    printf("thread arg2: %s\n", arg_ptr->arg2);
    printf("thread ret: %d\n", arg_ptr->ret);

    arg_ptr->ret = 2;

    return NULL;
}

int main( int argc, char* argv[] ){


    //Define our struct
    struct thread_struct my_struct;

    //Initialize a pointer to the instance of our structure
    struct thread_struct *my_struct_ptr = &my_struct;

    int N = 5;
    pthread_t threads[N];
    struct thread_struct structs[N];

    int i =0;

    for(; i < N; i++) {

        //Define structs values
        structs[i].arg1 = i;
        int print_ret = snprintf(structs[i].arg2, buffSize, "This is thread %d", i);
        structs[i].ret = i;

        int create_ret = pthread_create(&threads[i], NULL, thread_entry, &structs[i]);
        if(create_ret != 0){
            printf("Error creating thread. Error #%d. Exiting.", create_ret);
            exit(-1);
        }
    }

    int c = 0;
    for(; c < N; c++) {
        int join_ret = pthread_join(threads[c], NULL);
        if(join_ret != 0){
            printf("Error joining thread. Error #%d. Exiting.\n", join_ret);
            exit(-1);
        }
    }
}
