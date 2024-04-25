#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include "wish.h"

char *paths[BUFFER_SIZE];

int main(int argc, char *argv[]){
	char *input = NULL;
    size_t len = 0;
    int nread, tokens;
    paths[0] = strdup("/bin");
	paths[1] = NULL;
	
	while(1){
		printf("wish> ");
		
		if((nread = getline(&input, &len, stdin)) > 0){
			//remove newline character
			if(input[nread-1] == '\n')
			input[nread-1] = '\0';
			
        	if(strcmp(input, "exit") == 0){
        		free(input);
        		printf("Bye.\n");
        		exit(EXIT_SUCCESS);
        	}
        	
        	char *buffer[BUFFER_SIZE];
        	split_input(input, buffer, &tokens);
        	
        	// printf("no. of tokens: %d\n", tokens);
        	
        	if(strcmp(buffer[0], "cd") == 0){
        		if(tokens != 2){
        			print_error();
        		} else {
        			int val = chdir(buffer[1]);
        			if(val == -1)
        				print_error();
        		}
        	} else if(strcmp(buffer[0], "path") == 0){
        		// handle path
        		int i;
        		for(i=0; paths[i] != NULL; i++){
        			free(paths[i]);
        			paths[i] = NULL;
        		}
				
        		if(tokens > 1){
		    		for(i=1; i<tokens; i++){
		    			paths[i-1] = strdup(buffer[i]);
		    		}
        		}
        	} else { // if not built-in
				handle_external(buffer, tokens);
			}
        	
    	} else if (nread == -1){ // eof marker encountered
    		free(input);
    		printf("Exit gracefully.\n");
        	exit(EXIT_SUCCESS);
    	}
    	input = NULL;
    	len = 0;
	}
	free(input);
	return (0);
}

void split_input(char* ip, char** buffer, int* count){
	int n = 0;
	while(ip != NULL){
		if(ip[0] == ' '){
			*ip++;
			continue;
		}
		buffer[n] = strsep(&ip, "\t ");
		n++;
	}
	*count = n;
}

char* concat_n_str(int n, char* dest, char* srcs[]){
	int i;
	strcpy(dest, srcs[0]);
	for(i=1; i<n; i++){
		strcat(dest, srcs[i]);
	}
	return dest;
}

char* search_path(char* cmd){
	int i;
	char *path;
	for(i=0; paths[i] != NULL; i++){
		path = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		char *s[3];
		s[0] = paths[i];
		s[1] = strdup("/");
		s[2] = cmd;
		concat_n_str(3, path, s);
		
		free(s[1]);
		s[0] = s[2] = NULL;

		if(access(path, X_OK) == 0){
			return path;
		}
		free(path);
		path = NULL;
	}
	// if no paths were valid
	return NULL;
}

int handle_external(char** buffer, int count){
	// printf("Input: %s\n", ip);
	char **proc_args;
	int i;
	
	int rc = fork();
        	if(rc < 0){
        		print_error();
        		exit(1);
        	} else if(rc == 0) { // child
        		char *path = search_path(buffer[0]);
        		if(path == NULL){
        			print_error();
        			exit(1);
        		}
        		proc_args = malloc((count+1) * sizeof(char*));      		
        		
        		proc_args[0] = path;
        		
        		for(i=1; i<count; i++){
		    		proc_args[i] = strdup(buffer[i]);
        		}
        		proc_args[count] = NULL;
        		int val = execv(proc_args[0], proc_args);
				// any statement(s) from here execute only if execv fails
				if(val == -1){
					print_error();
					exit(1);
				}
        	} else {
        		int rc_wait = wait(NULL);
        		// printf("parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
        	}
	return 0;
}
