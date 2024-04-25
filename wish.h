#include<stdio.h>
#define BUFFER_SIZE 256

void print_error(){
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

void split_input(char*, char**, int*);
int handle_external(char**, int);
char *concat_n_str(int, char*, char*[]);
char *search_path(char*);
void print_paths();
