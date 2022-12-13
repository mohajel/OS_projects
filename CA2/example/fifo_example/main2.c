//in the name of God

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    int fd = open("sum", O_RDONLY);
    if (fd == -1)
    {
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        int z;
        if (read(fd, &z, sizeof(int)) == -1)
        {
            printf("OH !");
            return 80;
        }
        printf("GOT %d \n\n", z);   
    }

    close(fd);
    
}