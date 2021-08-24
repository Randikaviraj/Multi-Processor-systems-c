#include <stdio.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void handleclient(int ,struct sockaddr_in ,socklen_t );

int main(char argc, char **argv){
    int server_listen_fd,clientfd,n,pid;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilent;
 

    /* one socket is dedicated to listening */
    server_listen_fd=socket(AF_INET,SOCK_STREAM,0);


    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(12345);

    bind(server_listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    listen(server_listen_fd,0);
    clilent=sizeof(cliaddr);
    
    while (1)
    {
        clientfd = accept(server_listen_fd,(struct sockaddr *) &cliaddr, &clilent);

        if (clientfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }

        pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
            exit(1);
        }
        if (pid == 0)
        {
        /* In child process which the handles client connection */
            close(server_listen_fd);
            handleclient(clientfd,cliaddr,clilent);
            close(clientfd);
            exit(0);
        }
        else{
            printf("%d",pid);
            /* In parent process which continues to listen for new clients */
            close(clientfd);
        }
    }

    return 0;

}


void handleclient(int clientfd,struct sockaddr_in cliaddr,socklen_t clilent){

    char* banner = "Hello TCP client! File path is wrong";
    char line[1000];
    char buffer[1000]; 
    FILE *fptr;
    int n = recvfrom(clientfd,buffer,1000,0,(struct sockaddr *)&cliaddr,&clilent);
    buffer[n-1] = '\0';
    printf("Received:%s\n",buffer);
    if ((fptr = fopen(buffer,"r")) == NULL){
       sendto(clientfd,banner,strlen(banner),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));

    }else{
            while ( fgets(line, 1000, fptr ))
            {
                sendto(clientfd,line,strlen(line),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
            }
        fclose(fptr);
    }
        
    return;
}