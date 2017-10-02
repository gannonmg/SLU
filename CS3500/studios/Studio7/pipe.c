// Matt Gannon and Matt Gottsacker
// 9/17/17
// A program to

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main( int argc, char* argv[] ) {

    int fd[2];
    pipe(fd);
    
    int f1 = fork();
    //printf("%d %d\n", fd[0], fd[1]);
    
    if (f1 == -1) {
        perror("Error forking first process");
        exit(-1);
    } else if (f1 == 0) {
        dup2(fd[1], STDOUT_FILENO);
        char* cmd = "./print";
        char* myargv[] = {cmd, NULL};
        close(fd[0]);
        int ret = execvp( cmd, myargv );
        if (ret == -1) {
            perror("Error exec'ing print");
        }
    } else {
        int f2 = fork();
        if (f2 == -1) {
            perror("Error forking second process");
            exit(-1);
        } else if (f2 == 0) {
            dup2(fd[0], STDIN_FILENO);
            char* cmd = "./program2";
            char* myargv[] = {cmd, NULL};
            close(fd[1]);
            int ret = execvp( cmd, myargv );
            if (ret == -1) {
                perror("Error exec'ing program 2");
            }
        } else {
            close(fd[0]);
            close(fd[1]);
            waitpid( f1, NULL, 0 );
            waitpid( f2, NULL, 0 );
        }
    }
    return(0);
}
