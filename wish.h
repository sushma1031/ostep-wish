#include<stdio.h>
#define BUFFER_SIZE 256

#define INTERACTIVE 1
#define BATCH 2

void print_error(){
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));
}

void split_input(char*, char**, int*);
void execute_command(char**, int);
char *concat_n_str(int, char*, char*[]);
char *search_path(char*);
void print_paths();
char* trim(char*, int);
void cleanup();
