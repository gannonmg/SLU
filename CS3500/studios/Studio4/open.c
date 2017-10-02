// Matt Gannon
// 9/11/2017
// A program to open a file stored on the computer

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define bufferSize 200

int main( int argc, char* argv[] ) {
    
    char buffer[bufferSize];
    
    int input = open(argv[1], O_RDONLY);
    
    if (input == -1) {
        error("Error opening file");
        exit(-1);
    }
    
    for (;;) {
        int reader = read(input, buffer, bufferSize);
        if (reader==0) {
            break;
        }
        write(STDOUT_FILENO, buffer, reader);
    }

    return 0;
}
