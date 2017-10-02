// Matt Gannon, Matt Gottsacker
// 8/30/2017
// A program to print "hello world" in c using fprintf

#include <unistd.h>

int main( int argc, char* argv[] ){
	
	char buffer[] = "Hello, world!\n";
	write(STDOUT_FILENO, buffer, 14);
	return 0;
}
