#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
    printf("READING\n");

    int fd = open("fifo", O_RDONLY, 0666);

    if (fd == -1)
    {
        printf("cant open -1 \n");
        return -1;
    }
    char s [123];
    char ss [114];
    read(fd, s, 112);
    read(fd, ss, 112);
    printf("%s\n",s);
    printf("%s\n",ss);
    return 0;
}
