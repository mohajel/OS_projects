#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    printf("WRITING\n");
    int fd = open("fifo", O_WRONLY, 0666);
    if (fd == -1)
    {
        printf("cant open -1 \n");
        return -1;
    }
    
    write(fd, "salam", 6);
    write(fd, "balajan", 6);
    return 0;
}
