// Matt Gannon and Matt Gottsacker
// 9/17/2017
// A shell program made from scratch because we never want to take programs like bash or csh for granted

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#define bufferSize 4096
typedef int bool;
#define true 1
#define false 0
jmp_buf jbuf;

void handler(int signum) {
    if(signum==SIGINT)
    {
        printf("\n");
        longjmp(jbuf,1);
    }
    return;
}


int main( int argc, char* argv[] ) {
        
    char input[ bufferSize ];
    int max_cmds = 15;
    int max_cmds_size = max_cmds + 2; //one for argv[0], one for null terminator
    int max_args = 15;
    int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
    char* got = "0";
    int fd[2];
    
    signal(SIGINT, handler);
    
    while(got!=NULL) {
        setjmp(jbuf);
        char* cmd;
        char* my_cmds[max_cmds_size];
        
        // EXTRA CREDIT to add the directory address after slush
        char* direc;
        char* cwd = getcwd(direc, 20);
        char* home_env = getenv("HOME");
        
        int direcSize = strlen(cwd) - strlen(home_env);
        char* address[direcSize];
        
        int m = 0;
        for(; m < direcSize; m++) {
            address[m] = &cwd[m+strlen(home_env)];
        }

        printf("slush|%s :) ", *address);
        
        got = fgets(input, bufferSize, stdin);
        if(got == NULL) {
            printf("\n");
            exit(-1);
        }
        
        
        //Check for stray (
        bool strayParen = false;
        int j = 1;
        for(; j < strlen(input); j++) {
            if (input[j] == '(') {
                if (input[j] == input[j-1]) {
                    strayParen = true;
                }
            }
        }
        
        if((input[0] == '(') || (input[strlen(input)-2] == '(')) {
            strayParen = true;
        }
        
        //go through res and get each argument and put it in my_cmds[]
        
        char* res = strtok(input, "(\n");	//tokenize first command
        int c = 0;
        my_cmds[c] = res;
        while(res != NULL){
            c++;
            res = strtok(NULL, "(\n");
            my_cmds[c] = res;
        }
        int end = c-1;
        int num_cmds = c;
        
        
        //Infinite loop to test signal interpretation
        /*
        for(;;) {
            printf("Help me!!! I'm stuck in this loop in this laptop!! AGGHHH!!! It's coming to get me! Please!! Do something!!!");
        }
        */
         
        int lastPipe;
        for(; end>=0; end--) {
            
            char* my_argv[max_argv_size];
            char* res2 = strtok(my_cmds[end], " ");
            cmd = res2;
            int i = 0;
            my_argv[i] = res2;
            while(res2 != NULL){
                i++;
                res2 = strtok(NULL, " \n");
                my_argv[i] = res2;
            }
            
            //Checks for case where user enters two ( seperated by a space
            //ie `more ( ( ps au`
            if ((i == 0) || (strayParen == true)) {
                printf("Error: Ivalid null command\n");
                break;
            }
            
            if(num_cmds == 1) { //no piping required
                int f = fork();
                
                if (f == -1) {
                    perror("Error forking process");
                    exit(-1);
                } else if (f == 0) {
                    int ret = execvp(cmd, my_argv);
                    if (ret == -1) {
                        perror("Error exec'ing cmd");
                        exit(-1);
                    }
                } else {
                    waitpid(f, NULL, 0);
                }
            } else if(end==(num_cmds-1)) { //when cmd is reading from stdin (the furthest right)
                
                pipe(fd);
                
                int f = fork();
                
                if (f == -1) {
                    perror("Error forking process");
                    exit(-1);
                } else if (f == 0) {
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[0]);
                    int ret = execvp(cmd, my_argv);
                    if (ret == -1) {
                        perror("Error exec'ing cmd");
                        exit(-1);
                    }
                } else {
                    close(fd[1]);
                    waitpid(f, NULL, 0);
                }
            } else if(end==0) { //when cmd is outputting to stdout (the furthest left)
                
                pipe(fd);
                
                int f = fork();
                
                if (f == -1) {
                    perror("Error forking process");
                    exit(-1);
                } else if (f == 0) {
                    dup2(lastPipe, STDIN_FILENO);
                    close(fd[1]);
                    int ret = execvp(cmd, my_argv);
                    if (ret == -1) {
                        perror("Error exec'ing cmd");
                        exit(-1);
                    }
                } else {
                    close(lastPipe);
                    waitpid(f, NULL, 0);
                }
            } else { //command if feeding to and from the pipe (middle)
                
                pipe(fd);
                
                int f = fork();
                
                if (f == -1) {
                    perror("Error forking process");
                    exit(-1);
                } else if (f == 0) {
                    dup2(lastPipe, STDIN_FILENO);
                    dup2(fd[1], STDOUT_FILENO);
                    int ret = execvp(cmd, my_argv);
                    if (ret == -1) {
                        perror("Error exec'ing cmd");
                        exit(-1);
                    }
                } else {
                    close(fd[1]);
                    close(lastPipe);
                    waitpid(f, NULL, 0);
                }
            }
            
            lastPipe = fd[0];
            
        }
        
    }
    return(0);
}








