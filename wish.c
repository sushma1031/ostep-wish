#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argc, char *argv[]){
	char *input = NULL;
	// char *EXIT_CMD = (char*)malloc(5);
	char EXIT_CMD[5];
	strcpy(EXIT_CMD, "exit");
    size_t len = 0;
    int nread;
	
	while(1){
		printf("wish> ");
		
		if((nread = getline(&input, &len, stdin)) > 0){
			//remove newline character
			if(input[nread-1] == '\n')
			input[nread-1] = '\0';
			
        	if(strcmp(input, EXIT_CMD) == 0){
        		free(input);
        		printf("Bye.\n");
        		exit(EXIT_SUCCESS);
        	}
        	printf("Input: %s\n", input);
    	} else if (nread == -1){ // eof marker encountered
    		free(input);
    		printf("Exit gracefully.\n");
        	exit(EXIT_SUCCESS);
    	}
    	input = NULL;
    	len = 0;
	}
	free(input);
	// free(EXIT_CMD);	
	return (0);
}
