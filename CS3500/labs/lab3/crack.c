// Matt Gannon
// 9/29/2017
// A program to use threading to crack hash-encrypted passwords

#define _GNU_SOURCE // necessary for crypt_r
#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()
#include <string.h> //For strlen(), strcmp()
#include <crypt.h>

int found = 0;
int more = 0;

struct crack_args {
    char* place;
    char* target;
    int end;
    char* salt;
    struct crypt_data data;
    char buffer[9];
    int thread_num;
    int thread_skip;
};

void* crack (void* args, int i, int thread_num, int first_call) {   
    struct crack_args *arg = (struct crack_args*) args;
    if (more == 0) {
        int c = 'a' + thread_num;
        while(c <= 'z') {
            if (found == 1) break;
            char* swap = (arg->place)+i;
            *swap = c;
            if(i == arg->end) {
            //printf("Place is %s\n", arg->place);
                char* encrypted = crypt_r(arg->place, arg->salt, &(arg->data));
                if (strcmp(encrypted, arg->target) == 0) {
                    printf("FOUND!\n");
                    printf("Password is: %s\n", arg->place);
                    found = 1;
                    return;
                }
            } else {
            crack(args, i+1, 0, 0);
            }
            if (first_call == 1) {
                c += arg->thread_skip;
            } else {
                c++;
            }
        }
    } else {
        int c = '!' + thread_num;
        while(c <= '~') {
            if (found == 1) break;
            char* swap = (arg->place)+i;
            *swap = c;
            if(i == arg->end) {
                //printf("Place is %s\n", arg->place);
                char* encrypted = crypt_r(arg->place, arg->salt, &(arg->data));
                if (strcmp(encrypted, arg->target) == 0) {
                    printf("Password is: %s\n", arg->place);
                    found = 1;
                    return;
                }
            } else {
                crack(args, i+1, 0, 0);
            }
            if (first_call == 1) {
                c += arg->thread_skip;
            } else {
                c++;
            }
        }
    }
    return;
}

void* thread_crack (void* args) {
    struct crack_args *arg = (struct crack_args*) args;
    int i = (arg->thread_num);
    crack(args, 0, arg->thread_num, 1); 
}


int main(int argc, char* argv[]){
    
    //Make sure proper number of arguments is entered
    if((argc != 4) & (argc != 5)) {
        printf("Error: crack requires 3 parameters for lower case\n");
	printf("Enter 1 as the fourth parameter to include uppercase, digits, and symbols\n");
        exit(-1);
    }
    
    int threads = atoi(argv[1]);
    int keysize = atoi(argv[2]);
    char* target = argv[3];
    if (argc == 5) {
        if (atoi(argv[4]) != 1) {
            printf("Error: fourth param must be 1 if you want to include uppercase, digits, and symbols\n");
            exit(-1);
        } else {
            more = 1;
        }
    }


    //Make sure threads is 1 or higher
    if (threads < 1) {
        printf("A positive number of threads\n");
        exit(-1);
    }
   
    //Cap threads at 26 since that is the max letters in the alphabet
    if ((threads > 26) & (more == 0)) {
	printf("Thread cap is 26 for lowercase. Thread number has been set to 26\n");
	threads = 26;
    } else if ((threads>63) & (more == 1)) {
	printf("Thread cap is 63 when search includes uppcercase, digits, and symbols. Thread number has been set to 63\n");
	threads = 63;
    }
 
    //Make sure keysize is an appropriate length
    if ((keysize > 8) || (keysize < 1)) {
        printf("Please enter a keysize from 1 to 8\n");
        exit(-1);
    }
    
    //Get the salt
    char salt[3];
    salt[0] = target[0];
    salt[1] = target[1];
    salt[2] = '\0';
   
    //Loop to shorten length of passwords checked
    int m = 0;
    while (m != keysize) {
     
        //Initialize pthreads array and struct array of size threads
        pthread_t thread_array[threads];
        struct crack_args struct_args[threads];
        
        //Loop to create threads and call crack
        //char place[keysize+1];
        //place[keysize] = '\0';

        int i = 0;
        for(; i<threads; i++) {
            struct_args[i].buffer[keysize-m] = '\0';
            struct_args[i].place = struct_args[i].buffer;
            struct_args[i].target = target;
            struct_args[i].end = keysize - 1 - m;
            struct_args[i].salt = salt;
            struct_args[i].data.initialized = 0;
            struct_args[i].thread_num = i;
            struct_args[i].thread_skip = threads;

            int thread_ret = pthread_create(&thread_array[i], NULL, thread_crack, &struct_args[i]);
            if (thread_ret != 0) {
                printf("Thread creation error #%d\n", thread_ret);
        }
        }
        
        //Loop to join threads
        int j = 0;
        for(; j<threads; j++) {
            int join_ret = pthread_join(thread_array[j], NULL);
            if(join_ret != 0){
                printf("Error joining thread. Error #%d. Exiting.", join_ret);
                exit(-1);
            }
        }

        m++;
    }

    if (found == 0) {
        printf("Password not found\n");
    }
    
    printf("End of program\n");
}
