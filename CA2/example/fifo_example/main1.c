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
    // if ((mkfifo("myfifo1", 0777) == -1) && (errno != EEXIST))
    // {
    //     perror("could not create fifo file");
    //     return -1;
    // }


    int x[12];
    x[0] = 12;
    x[1] = 14;
    x[2] = 33;
    x[3] = 44;
    x[4] = 456;

    int fd = open("sum", O_WRONLY);
    if (fd == -1)
    {
        return 1;
    }
    
    for (int i = 0; i < 5; i++)
    {
        write(fd, &x[i], sizeof(int));
    }
    
    // char s[200] = salam
    close(fd);
    
    return 0;
}
