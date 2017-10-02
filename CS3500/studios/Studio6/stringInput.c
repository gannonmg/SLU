// Matt Gannon and Matt Gottsacker
// 9/16/2017
// A program to get and parse user input

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define bufferSize 4096

int main( int argc, char* argv[] ) {
    char input[ bufferSize ];
    int max_args = 15;
    int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
    char* cmd;
    char* my_argv[max_argv_size];
    
    
    printf("Please enter your input here: ");
    
    char* got = fgets(input, bufferSize, stdin);
    if(got == NULL) {
        printf("Error reading input");
        exit(-1);
    }

    printf("User input was: %s", input);
    
    char* res = strtok(input, " ");
    //printf("First token was: %s\n", res);
    cmd = res;
    my_argv[0] = cmd;
    int i = 1;
    
    while (res != NULL)
    {
        printf ("%s\n", res);
        res = strtok(NULL, " \n");
        my_argv[i] = res;
        i++;
    }
    my_argv[i] = NULL;
    
    execvp( cmd, my_argv );
    
    return(0);
}
