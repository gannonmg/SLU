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

// Create linked list structure

struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

struct Node* head; // global variable - pointer to head node.

//Creates a new Node and returns pointer to it.
struct Node* GetNewNode(int x) {
    struct Node* newNode
    = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = x;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

//Inserts a Node at head of doubly linked list
void InsertAtHead(int x) {
    struct Node* newNode = GetNewNode(x);
    if(head == NULL) {
        head = newNode;
        return;
    }
    head->prev = newNode;
    newNode->next = head;
    head = newNode;
}

//Inserts a Node at tail of Doubly linked list
void InsertAtTail(int x) {
    struct Node* temp = head;
    struct Node* newNode = GetNewNode(x);
    if(head == NULL) {
        head = newNode;
        return;
    }
    while(temp->next != NULL) temp = temp->next; // Go To last Node
    temp->next = newNode;
    newNode->prev = temp;
}

//Prints all the elements in linked list in forward traversal order
void Print() {
    struct Node* temp = head;
    printf("Forward: ");
    while(temp != NULL) {
        printf("%d ",temp->data);
        temp = temp->next;
    }
    printf("\n");
}

//Prints all elements in linked list in reverse traversal order.
void ReversePrint() {
    struct Node* temp = head;
    if(temp == NULL) return; // empty list, exit
    // Going to last Node
    while(temp->next != NULL) {
        temp = temp->next;
    }
    // Traversing backward using prev pointer
    printf("Reverse: ");
    while(temp != NULL) {
        printf("%d ",temp->data);
        temp = temp->prev;
    }
    printf("\n");
}

// memory block structure
struct memory_block {
    size_t size;
    struct memory_block* next_block;
    struct memory_block* prev_block;
};

int main () {
    char *str;
    
    /* Initial memory allocation */
    str = (char *) malloc(15);
    strcpy(str, "tutorialspoint");
    printf("String = %s,  Address = %u\n", str, str);
    
    /* Reallocating memory */
    str = (char *) realloc(str, 25);
    strcat(str, ".com");
    printf("String = %s,  Address = %u\n", str, str);
    
    free(str);
    
    return(0);
}

void *mymalloc(size_t size) {
    
    int page_size = sysconf(_SC_PAGESIZE);
    
    //Get memory chunk from OS
    void* ret = sbrk(size);
    
    struct memory_block block
    
    //Check for success, else return NULL
    if( ret == (void*)-1 ){
        printf("Error calling sbrk!\n");
        errno = ENOMEM;
        return NULL;
    }
    
    printf("Malloc called with size %d, returning pointer to %p\n", size, ret);
    
    return ret;
    
}

/*
 void myfree( void *ptr );
 
 void *mycalloc( size_t num_of_elts, size_t elt_size ) {;
 
 void *myrealloc( void *pointer, size_t size);
 */
