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
    char* fifo_name = strtok(read_msg, ",");
    int map_size = atoi(strtok(NULL, ","));
    // printf("        ---fifo_name:%s map_size:%d\n", fifo_name, map_size);


    // sleep(2);
    int fd = open(fifo_name, O_RDONLY, 0666);

    while(fd == -1)
    {
        printf("REDUCE IT IS -1-->\n");

        fd = open(fifo_name, O_RDONLY);  
    }

    printf("reduce fd is : %d\n", fd);


    while(result != map_size)
    {
        // sleep(1);
        char input[BUFFER_SIZE];
        int x = read(fd, input, 80);
        if (x > 0)
        {
            printf("result[%d] --> %s\n",result, input);
            result ++;
        }  
        // printf("%d", x);

        // printf("result[%d] --> %s\n",result, input);
        // result ++;
    }

    // fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    // char input[BUFFER_SIZE];
    // char s[40];
    // char ss[40];

    // read(fd, s, 30);
    // read(fd, ss, 30);

    // printf("%s1 --> %s\n",fifo_name, input);
    // printf("%s2 --> %s\n",fifo_name, s);
    // printf("%s3 --> %s\n",fifo_name, ss);


    // // fd = open(fifo_name, O_RDONLY);
    // read(fd, input, BUFFER_SIZE);
    // printf("%s3 --> %s\n",fifo_name, input);
    // close(fd);


    printf("\nGanre:%s Count: %d\n", fifo_name, result);
    close(fd);
    exit(EXIT_SUCCESS);
}
