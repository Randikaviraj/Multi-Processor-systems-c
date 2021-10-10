#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>


void* handle_client(void*);
int main()
{
    int listenfd;
    int* connfd;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilen;
    pthread_t clithread;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);
    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    listen(listenfd,5);
    clilen = sizeof(cliaddr);
    
    while(1)
    {
        connfd = malloc(sizeof(int));
        *connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        /* now create a new thread, pass it the socket and run the thread.
        */
        if ( pthread_create( &clithread, NULL, handle_client, connfd) )
        {
            printf("error creating thread.");
        }
    }
}

void* handle_client(void* connfd)
{
    /*read a string sent by the client,
    print it and then send the string
    “Hello from the server” to the client*/
    char* banner = "Hello from the server";
    char buffer[1000]; 
    int cliSocket=*((int *)connfd);
    
    int n=read(cliSocket , buffer, 1000);
    if (n==-1)
    {
        printf("error reading.");
        close(cliSocket);
        free(connfd);
        return NULL;
    }
    buffer[n]=0;
    send(cliSocket , banner , strlen(banner) , 0 ); 
    printf("Received:%s\n",buffer); 
    close(cliSocket);
    free(connfd);

    return NULL;
}