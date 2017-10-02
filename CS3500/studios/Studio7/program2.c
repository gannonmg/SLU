// Matt Gannon and Matt Gottsacker
// Program 2
// A program to read and write user input using fgets until the end of file character is entered

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define bufferSize 4096

int main( int argc, char* argv[] ) {
    
    char input[ bufferSize ];
    char* got = "0";

    while (got != NULL) {
        got = fgets(input, bufferSize, stdin);
        if (got == NULL)
            break;
        printf("Program 2 got: %s", input);
    }
    
    return(0);
}
