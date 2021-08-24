// E16332
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void getFlagArgument(int sizeOfArray, int *i, int stringLength, char **ptr, char *command);

int main(int c, char *args[])
{

    FILE *file;

    while (1)
    {
        char *path;
        char command[100], filepath[105];
        int sizeOfArray = 1;
        char **ptr = (char **)calloc(sizeOfArray, sizeof(char *));

        if ((path = getcwd(NULL, 0)) == NULL) //Get the current working path to display in shell
        {
            perror("Error :exited");
            return -1;
        }

        printf("\n%s", path); //print the shell symbols
        printf("$$");
        fgets(command, sizeof(command), stdin); //prompt for command

        int i = 0;

        getFlagArgument(sizeOfArray - 1, &i, 100, ptr, command);

        if (strlen(ptr[0]) <= 0) //when nothing enterd continue
        {
            continue;
        }
        strcpy(filepath, "/bin/"); //set the file path accroding to command
        strcat(filepath, ptr[0]);

        if (!(file = fopen(filepath, "r"))) //check wether file path exist
        {
            printf("\nCommand not found\n");
            continue;
        }
        fclose(file);
        for (; i < 100;)
        {   if (isspace(command[i])){
                i++;
                continue;
            }
            if (command[i] == '\0')
            {
                break;
            }
            sizeOfArray++;
            ptr = (char **)realloc(ptr, sizeof(char *) * (sizeOfArray + 1));
            getFlagArgument(sizeOfArray - 1, &i, 100, ptr, command);
        }

        int pid = fork(); //create child process

        if (pid == 0) //child process handler
        {
            char *args[] = {filepath, NULL};
            execvp(filepath, ptr);
            break;
        }
        else
        {
            wait(NULL);
            for (int j = 0; j < sizeOfArray + 1; j++)
            { //freed memory in parent process
                free(ptr[j]);
            }
            free(ptr);
            free(path);
        }
    }
    return 0;
}

void getFlagArgument(int sizeOfArray, int *i, int stringLength, char **ptr, char *command)
{
    while (isspace(command[*i]))
    { //removing whitespaces
        (*i)++;
    }
    ptr[sizeOfArray] = (char *)calloc(stringLength, sizeof(char));
    for (int j = 0; *i < 100; (*i)++)
    { //get the command ,flags from the entered string line
        if (isspace(command[*i]) || command[*i] == '\0')
        {
            ptr[sizeOfArray][j] = '\0';
            break;
        }

        ptr[sizeOfArray][j] = command[*i];
        j++;
    }
}