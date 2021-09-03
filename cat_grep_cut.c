#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat fixtures | grep <search_term> | cut -b 1-9".
 */

#define INPUTFILE "fixtures"

int main(int argc, char **argv)
{
	int status;
	int pid1,pid2;
    if (argc < 2)
	{
		printf("%s: missing operand\n", argv[0]);
		printf("Usage: %s <search_term in %s>\n", argv[0],INPUTFILE);
		exit(EXIT_FAILURE);
	}

	// arguments for commands
	char *cat_args[] = {"cat", INPUTFILE, NULL};
	char *grep_args[] = {"grep", "-i", argv[1], NULL};
	char *cut_args[] = {"cut", "-b", "1-9", NULL};

	int pipes[4];

    if(pipe(pipes) == -1){
        perror("Pipe Error");
        return -1;
    }

    if(pipe(&pipes[2]) == -1){
        perror("Pipe Error");
        return -1;
    }



    pid1=fork();
    if(pid1 == -1){
        perror("pid1");
        return -1;
    }

	if ( pid1== 0)
	{
        // child 1 process
        close(pipes[0]);
        close(pipes[2]);
        close(pipes[3]);
        if(dup2(pipes[1],STDOUT_FILENO) == -1){
            perror("Dup2");
            exit(-1);
        }

        if(execvp("cat", cat_args) == -1){
            perror("cat");
            exit(-1);
        }
	}
	else
	{
		// parent continue 
        pid2=fork();
		if (pid2 == 0)
		{
            // child2 process
            close(pipes[1]);
            close(pipes[2]);
            if(dup2(pipes[0],STDIN_FILENO) == -1){
                perror("Dup2");
                exit(-1);
            }
            if(dup2(pipes[3],STDOUT_FILENO) == -1){
                perror("Dup2");
                exit(-1);
            }
            if(execvp("grep", grep_args) == -1){
               perror("grep");
                exit(-1); 
            }
		}
		else
		{
			// parent continue
            close(pipes[0]);
            close(pipes[1]);
            close(pipes[3]);
            if(dup2(pipes[2],STDIN_FILENO) == -1){
                perror("Dup2");
                exit(-1);
            }
  
            if(execvp("cut", cut_args) == -1){
                perror("cut");
                exit(-1); 
            }
		}
	}

}