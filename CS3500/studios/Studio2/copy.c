// Matt Gannon
// 9/2/2017
// A short program to mimic the behavior of cat

include <unistd.h>

int main( int argc, char* argv[] ){
        #define bufferSize 200
        char buffer[ bufferSize ];

        for (;;) {
                int reader = read(STDIN_FILENO, buffer, bufferSize);
                if (reader==0) {
                        break;
                }
                write(STDOUT_FILENO, buffer, reader);
        }
        return 0;
}
