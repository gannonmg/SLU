// Matt Gannon and Matt Gottsacker
// 9/27/2017
// A program to learn to use atomic instructions to synchronize access to a variable and measure the overhead costs of these instructions.

#include <stdio.h> //For printf()

int main( int argc, char* argv[] ){
    
    int* test = 0;
    printf("Test before: %d\n", test);
    
    __sync_add_and_fetch(&test, 1);
    
    printf("Test after: %d\n", test);
    
}
