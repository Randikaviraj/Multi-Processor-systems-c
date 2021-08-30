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
        printf("missing file operand\n");
        return -1;
    }
    if (argc<3)
    {
        printf("missing destination file operand after %s\n",argv[1]);
        return -1;
    }

    int source = open(argv[1], O_RDONLY);
    if(source==-1){
        printf("No such file\n");
        return -1;
    }
    
    int desc = open(argv[2], O_WRONLY|O_CREAT , S_IRUSR | S_IWUSR);
    if(source==-1){
        printf("Invalid destination path\n");
        return -1;
    }
    char ch;
    while (read(source,&ch,sizeof(char)))
    {
        write(desc,&ch,sizeof(char));
    }
    close(source);
    close(desc);
    
    return 0;
}
