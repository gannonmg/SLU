// Matt Gannon & Matt Gottsacker
// 11/4/2017
// Program to recreate memory allocation routines
// Doubly linked list implementation borrowed from https://gist.github.com/mycodeschool/7429492

#include <sys/types.h>
#include <pthread.h> // for pthread_create(), phtread_mutex_init
#include <unistd.h> //fork(), execvp(), perror(), waidpid()
#include <stdlib.h> //For exit()
#include <stdio.h> //For printf()
#include <signal.h> //For signal()
#include <errno.h> //For perror & errno

//extern void* mymalloc(size_t size);
//extern void myfree(void *ptr);

#define array_size 500
#define short_array 250

// Create linked list structure
struct Node {
    size_t size;
    int free;               // 1 to indicate free, 0 to indicate not free
    struct Node* next;
};

#define NODE_SIZE sizeof(struct Node)

struct Node* head;          // global variable - pointer to head node.
struct Node* tail;

int main () {
    
    //Create and destroy two arrays twice
    int *array = (int*) mymalloc( sizeof(int) * array_size);
    int *array2 = (int*) mymalloc( sizeof(int) * array_size);
    
    int i;
    for(i =0 ; i< array_size; i++) {
        array[i] = i;
    }
    
    myfree(array);
    myfree(array2);
    
    return 0;
}

//Helper functions for malloc

//Finding a the first fitting free node
struct Node* first_free(size_t size) {
    struct Node *current = head;
    while((current->next != NULL) && !((current->free == 1) && (current->size >= size))){
        current = current->next;
    }
    return current;
}

//Create a new node/memory block
struct Node *new_node(size_t size) {
    
    //Get memory chunk from OS
    void* ret = sbrk(size  + NODE_SIZE);
    struct Node *new_node;
    new_node = ret;
    
    //Check for success, else return NULL
    if( ret == (void*)-1 ){
        printf("Error calling sbrk!\n");
        errno = ENOMEM;
        return NULL;
    }
    
    new_node->size = size;
    new_node->next = NULL;
    new_node->free = 0;
    
    tail->next = new_node;
    tail = new_node;
}


void* mymalloc(size_t size) {
    
    struct Node *pointer;
    
    if (!head) {        // first call
        pointer = request_space(size);
        head = pointer;
        tail = head;
    } else {
        pointer = first_free(size);
        if (!pointer) {
            pointer = new_node(size);
        } else {
            pointer->free = 0;
        }
    }
    
    return(pointer+1);
    
    /*
    int page_size = sysconf(_SC_PAGESIZE);
    
    //check if there is a free block in list of free blocks
    if(head->next != NULL){ //there is a block in list of free blocks
        
        Node *pointer = (Node*) sbrk(sizeof(Node));
        
        pointer -> address = allocation_ptr;
        pointer -> size = size;
        pointer -> flag = 0;
        pointer -> next = NULL;
        
        head->next  = pointer;
        
        int bigenough = 0;
        
        struct Node *current = head;
        
        //search list for big enough block
        struct Node *free_node = first_free(current);
        
        
    } else {

    printf("Malloc called with size %d, returning pointer to %p\n", size, ret);
    
    return ret;
    */
}



/*
 void myfree( void *ptr );
 void *mycalloc( size_t num_of_elts, size_t elt_size ) {;
 void *myrealloc( void *pointer, size_t size);
 */





