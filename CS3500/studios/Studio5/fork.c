// Matt Gannon and Matt Gottsacker
// 9/11/2017
// A program to study the usage of fork() and exec()

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main( int argc, char* argv[] ) {
    
    char* child_argv[] = {"ls", "-l", NULL};
    char* child_prog = child_argv[0];
    
    int f = fork();
    
    if (f == -1) {
        perror("Error forking process");
        exit(-1);
    } else if (f == 0) {
        sleep(3);
        printf("Child PID: %d\n", getpid());
        execvp( child_prog, child_argv );
    } else {
        waitpid( f, NULL, 0 );
        printf("Parent PID: %d\n", getpid());
    }
    
    return(0);
}
