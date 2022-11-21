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
    int result = 0;

    // printf("\n --- inside reduce.c, argv[1] = %s\n", argv[1]);
    int fd_unname_pipe = atoi(argv[1]);
    char read_msg[BUFFER_SIZE];
    read(fd_unname_pipe, read_msg, BUFFER_SIZE);
    close(fd_unname_pipe);
    char *fifo_name = strtok(read_msg, ",");
    int map_size = atoi(strtok(NULL, ","));

    int fd = open(fifo_name, O_RDONLY);
    int x;
    for (int i = 0; i < map_size; i++)
    {
        int y;
        while (1)
        {
            sleep(1);
            int y = read(fd, &x, sizeof(int));
            if (y == -1)
            {
                printf("OH NO!\n");
                return 33;
            }
            if (y == 0)
                printf("didnit get anythin!\n");
            else
            {
                // printf("-----GOT %d, return value = %d!\n", x, y);
                break;
            }
        }
        // sleep(1);
        result += x;
    }
    printf("Ganre:%s Count: %d\n", fifo_name, result);
    close(fd);
    exit(EXIT_SUCCESS);
}
