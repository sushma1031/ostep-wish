#include<stdio.h>
#define BUFFER_SIZE 256

void printError(){
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

int handleExternal(char*);
