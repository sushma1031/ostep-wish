#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include "wish.h"

int main(int argc, char *argv[]){
	char *input = NULL;
    size_t len = 0;
    int nread;
	
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
//        	if not built in
			handleExternal(input);
        	
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

int handleExternal(char* ip){
	// printf("Input: %s\n", ip);
	char *buffer[BUFFER_SIZE];
	char **proc_args;
	int i, count = 0;
	while(ip != NULL){
		if(ip[0] == ' '){
			*ip++;
			continue;
		}
		buffer[count] = strsep(&ip, "\t ");
		count++;
	}
	// printf("no. of tokens: %d\n", count);
	int rc = fork();
        	if(rc < 0){
        		printError();
        	} else if(rc == 0) { //child
        		printf("child: %d\n", (int) getpid());
        		proc_args = malloc((count+1) * sizeof(char*));
        		proc_args[0] = malloc(6 + strlen(buffer[0]));
        		strcpy(proc_args[0], "/bin/");
        		strcat(proc_args[0], buffer[0]);
        		for(i=1; i<count; i++){
		    		proc_args[i] = strdup(buffer[i]);
        		}
        		proc_args[count] = NULL;
        		for(i=0; i<count; i++){
		    		printf("%s ", proc_args[i]);
        		}
        		printf("\n");
        		execv(proc_args[0], proc_args);
				// any statement from here, within the block scope, should not execute
        	} else {
        		int rc_wait = wait(NULL);
        		// printf("parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
        	}
	return 0;
}
