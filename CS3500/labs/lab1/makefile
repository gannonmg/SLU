all:
	gcc -o encrypt encrypt.c
	./encrypt abcd1234 test3 encrypt3.txt 0
	cat encrypt3.txt
	./encrypt abcd1234 encrypt3.txt decrypt3.txt 1
	cat decrypt3.txt
