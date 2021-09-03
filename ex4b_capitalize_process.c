#include <fcntl.h> 
#include <stdio.h> 
#include <sys/stat.h> 
#include <errno.h> 
#include <unistd.h> 
#include <stdlib.h>

#define MAX_SIZE 100 

int main() 
{ 
    int fd1,fd2,pid; 
    char* fifo1 = "normal"; 
    char* fifo2 = "capitalize";
    char buf[MAX_SIZE]; 

    if (mkfifo(fifo1,0666)==-1)
    {
        if (errno!=EEXIST)
        {
            perror("Failed create fifo_normal");
            exit(1);
        }
        
    }

    if (mkfifo(fifo2,0666)==-1)
    {
        if (errno!=EEXIST)
        {
            perror("Failed create fifo1_capitalize");
            exit(1);
        }
        
    }


    fd1 = open(fifo1, O_RDONLY);
    if (fd1==-1)
    {
        perror("Failed open fifo_normal");
    } 
    
    fd2 = open(fifo2, O_WRONLY);
    if (fd2==-1)
    {
        perror("Failed open fifo1_capitalize");
    }


    if(dup2(fd1,STDIN_FILENO) == -1){
        perror("Dup2");
        exit(-1);
    }
    if(dup2(fd2,STDOUT_FILENO)== -1){
        perror("Dup2");
        exit(-1);
    }
    char *tr_args[] = {"tr", "'a-z'", "'A-Z'", NULL};
    if(execvp("tr", tr_args) == -1){
        perror("tr()");
        exit(-1);
    }
  

    return 0; 
}
