//Matts, Gannon and Gottsacker
//9/8/2017
//This program illustrates some uses of pointers in C

#include <stdio.h>
#include <string.h>

char linString[] = "Linux!";
int  linSize = 6;

char *winString = "Windows!";
int  winSize = 8;

int main ( int argc, char* argv[] ){
    
    /*
    linString[0] = 'M';
    linString[3] = 'i';
    
    int i = 0;
    for (; i != strlen(linString); i++) {
        printf("%c\n", linString[i]);
    }
    */
    
    //winString[0] = 'M';
    int j=0;
    for (; j != winSize; j++) {
        printf("%s\n", (winString+j));
    }
    
    
    
    return 0;
}


