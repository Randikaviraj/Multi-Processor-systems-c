#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pipe_parent_to_child[2],pipe_child_to_parent[2];
    int pid;
    if(pipe(pipe_parent_to_child)){
        perror("Create pipe");
        return -1;
    }
    if(pipe(pipe_child_to_parent)){
        perror("Create pipe");
        return -1;
    }
    pid = fork();
    if(pid < 0){
        perror("Fork");
        exit(1);
    }

    if(pid > 0){ /* parent */
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
        char message[100];
        printf("Enter your string: ");
        int n=1;
        scanf("%s",message);
        while (message[n]!='\0')
        {
           n++;
        }
        write(pipe_parent_to_child[1],message,n);
        close(pipe_parent_to_child[1]);
        n=read(pipe_child_to_parent[0], message, sizeof(message)/sizeof(char));
        message[n]='\0';
        printf("message from child %s\n",message);
        close(pipe_child_to_parent[0]);
        wait(NULL);
    }else{
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        char message[100];
        int n=read(pipe_parent_to_child[0], message, sizeof(message)/sizeof(char));
        message[n]='\0';
        for (int i = 0; i < n; i++)
        {
            message[i]=message[i]-32;
        }
        write(pipe_child_to_parent[1],message,n);
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
    }
    
    return 0;
}
