#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    unlink("fifooo");
    sleep(4);
    mkfifo("fifooo", 0666);
    return 0;
}

