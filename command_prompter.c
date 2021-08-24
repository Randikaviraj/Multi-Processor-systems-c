#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(char argc, char **argv)
{
    do
    {   char command[100];
        char *args[20];
        printf("Enter the command$\n");
        fgets (command, 20, stdin);
        for (int i = 0; i < 100; i++)
        {
            if (command[i]=='\n')
            {
                command[i]='\0';
                break;
            }
        }
        
        int i = 0;
        char delim[] = " ";
	    char *ptr = strtok(command, delim);
	    while(ptr != NULL)
        {
            
            if (i==20)
            {
                break;
            }
            args[i]=ptr;
            i++;
            printf("%s\n", ptr);
            ptr = strtok(NULL, delim);
        }
        args[i]=NULL;
        char destination[] = "/bin/";
        strcat(destination,args[0]);
        args[0]=destination;
 
        int pid=fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0){
            execv(destination, args);
        }else{
            wait(NULL);
        }
    
    } while (1);
    
    return 0;
}