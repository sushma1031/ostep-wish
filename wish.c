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
    int nread, rc, rc_wait;
	
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
        	printf("Input: %s\n", input);
      
        	rc = fork();
        	if(rc < 0){
        		printError();
        	} else if(rc == 0){ //child
        		printf("child: %d\n", (int) getpid());
        		char *myargs[2];
        		myargs[0] = strdup("/bin/ls");
        		myargs[1] = NULL;
        		execv(myargs[0], myargs);
				// any statement from here, within the block scope, should not execute
        	} else {
        		rc_wait = wait(NULL);
        		printf("parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
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
