#include <fcntl.h> 
#include <stdio.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <errno.h> 
#include <stdlib.h>

#define MAX_SIZE 100 

int main() 
{ 
    int fd1,fd2; 
    char* fifo1 = "normal"; 
    char* fifo2 = "capitalize";
    char buf[MAX_SIZE];
    char input[MAX_SIZE];

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

    printf("Enter the string :\n");
    fgets (input, MAX_SIZE, stdin);
    
    fd1 = open(fifo1, O_WRONLY); 
    if (fd1==-1)
    {
        perror("Failed open fifo_normal");
        exit(1);
    } 
    write(fd1, input,MAX_SIZE); 
    close(fd1);

    fd2 = open(fifo2, O_RDONLY);
    if (fd2==-1)
    {
        perror("Failed open fifo1_capitalize");
        exit(1);
    } 
    read(fd2, buf, MAX_SIZE); 
    printf("message read = %s\n", buf); 


    unlink(fifo1); 
    unlink(fifo2); 
    close(fd2); 

    return 0; 
}
