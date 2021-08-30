#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    if (argc<2)
    {
        printf("File name is required");
        return -1;
    }
    
    int desc = open(argv[1], O_RDONLY);

    if(desc==-1){
        printf("No such file");
        return -1;
    }
    char ch;
    while (read(desc,&ch,sizeof(char)))
    {
        write(1,&ch,sizeof(char));
    }
    ch="\n";
    write(1,&ch,sizeof(char));
    close(desc);
    return 0;
}

