// in the name of God

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "manual.h"

int main(int argc, char const *argv[])
{
    printf(" --- inside reduce.c, argv[1] = %s\n", argv[1]);
    int fd = atoi(argv[1]);
    char read_msg[BUFFER_SIZE];
    read(fd, read_msg, BUFFER_SIZE);
    close(fd);
    char* fifo_name = strtok(read_msg, ",");
    int map_size = atoi(strtok(NULL, ","));
    printf("        ---fifo_name:%s map_size:%d\n", fifo_name, map_size);
    return 0;
}
