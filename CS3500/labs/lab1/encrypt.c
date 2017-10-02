// Matt Gannon
// 9/5/2017
// A program to encrypt and decrypt files using the GNU C library function ecb_crypt.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/des_crypt.h>
#include <string.h>
#include <errno.h>

#define bufferSize 200


int main( int argc, char* argv[] ){

    if(argc != 5){
		perror("Error: encrypt requires 4 parameters\n");
		printf("errno: %d\n", errno);
		exit(-1);
	}

	char buffer[ bufferSize ];

    char* key = argv[1];

	if(strlen(key) != 8){
		perror("Error: Key must be 8 characters long.\n");
		printf("errno: %d\n", errno);
		exit(-1);
	}

    int input = open(argv[2], O_RDONLY);
	
	if(input == -1){
		perror("Error: invalid input file.\n");
		printf("errno: %d\n", errno);
		exit(-1);
	}

    int output = open(argv[3], O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

    int mode = atoi(argv[4]);

	if(mode != 0 && mode != 1){
		perror("Error: Encryption mode must be either 0 (to encrypt) or 1 (to decrypt).\n");
		printf("errno: %d\n", errno);
		exit(-1);
	}

        des_setparity(key);

        // Read and write the input file to a separate output file

        for (;;) {
                int reader = read(input, buffer, 4096);
		
		if(reader == -1){
			perror("Error reading file.");
			printf("errno: %d\n", errno);
			exit(-1);
		}

                if (reader==0) {
                        break;
                }

                // Here, make sure reader is an even multiple of 8
                // Add spaces to buffer array until buffer length is a multiple of 8
               
                // Padding files to have byte sizes divisible by 8
                        
                int buffBytes = (8-reader%8);

                
                //reader += buffBytes;
                
                if ( (reader % 8) != 0){
                        for (; (reader % 8) != 0 ; reader++){
                                buffer[reader] = ' '; 
                        } 
                }

/*  
                int i = buffBytes;
                for (i; i!=0; i--) {
                        buffer[reader-1] = " ";
                }
*/
                if (mode == 0) {
                        ecb_crypt(key, buffer, reader, DES_ENCRYPT);
                }
                if (mode == 1) {
                        ecb_crypt(key, buffer, reader, DES_DECRYPT);
                }
                
                int bytesWritten = write(output, buffer, reader);
                
		if(bytesWritten == -1){
			perror("Error on writing file.");
			printf("errno: %d\n", errno);
			exit(-1);
		}
        }

	int closeInput = close(input);
	
	if(closeInput == -1){
		perror("Error on closing input file.");
		printf("errno: %d\n", errno);
		exit(-1);
	}
	
	int closeOutput = close(output);
	
	if(closeOutput == -1){
		perror("Error on closing output file.");
		printf("errno %d\n", errno);
		exit(-1);
	}        

	return 0;
}

