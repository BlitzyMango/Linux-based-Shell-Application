#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXARG 2
#define MAXLEN 1048

int main(int argc, char *argv[])
{
	if(argc == 1) 
	{
		/** ZERO COMMANDS **/
		printf("Error: Must enter at least %d command\n", MAXARG-1);
		return 1;
	}

	int sep;
	int cmdcount = 1;
	
	for(int i = 1; i < argc; i++) 
	{
		if(argv[i][0] == '+') {			// check if element in argv is '+'
			++cmdcount;					// keep track of commands
			sep = i;					// record index of '+' for later
		}
	}

	if(cmdcount == 1) 
	{
		/** ONE COMMAND **/
		int size = argc;
		char *cmd[size+1];

		memcpy(cmd, &argv[1], 
		       size * sizeof(char*));	// cmd = all of argv (except index 1)
		cmd[size-1] = NULL;

		int pid = fork();				// child
		if(pid < 0) {
			perror("fork");
			return 1;
		}

		if(pid == 0) {
			execvp(cmd[0], cmd);		// execvp cmd and its arguments
			perror("execvp");
			return 1;
		}
		else {wait(0);}					// parent
	}
	
	else if(cmdcount > 2) 
	{
		/** OVER TWO COMMANDS **/
		printf("Error: Command count cannot exceed %d\n", MAXARG);
		return 1;
	}

	else 			
	{
		/** TWO COMMANDS **/
		int size1 = sep;				// cmd1 goes from start (1) to '+'
		int size2 = argc-sep;			// cmd2 goes from '+' to end (argc)

		char *cmd1[size1];
		char *cmd2[size2+1];

		memcpy(cmd1, &argv[1], 
		       size1 * sizeof(char*));	// cmd1 = first half of argv
		memcpy(cmd2, &argv[sep+1], 
		       size2 * sizeof(char*));	// cmd2 = second half of argv

		cmd1[size1-1] = NULL;
		cmd2[size2-1] = NULL;

		int fd[2];	
		if(pipe(fd) < 0) {			// pipe
			perror("pipe");
			return 1;
		}

		int pid_1 = fork();			// child 1
		if(pid_1 < 0) {
			perror("fork");
			return 1;
		}

		if(pid_1 == 0) {
			close(fd[0]); 
			dup2(fd[1], 1);
			close(fd[1]);
			execvp(cmd1[0], cmd1);	// execvp cmd1 and its arguments
			perror("execvp");
			return 1;
		}
				
		int pid_2 = fork();			// child 2
		if(pid_2 < 0) {
			perror("fork");
			return 1;
		}

		if(pid_2 == 0) {
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			execvp(cmd2[0], cmd2);	// execvp cmd2 and its arguments
			perror("execvp");
			return 1;
		}
		
		close(fd[0]);	// close the side of the pipe used by child 2
		close(fd[1]);	// close the side of the pipe used by child 1
		
		wait(0);		// parent waits for child processes to finish
		wait(0);
	}
	return 0;
}
