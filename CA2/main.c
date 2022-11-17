// in the name of God

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define READ_END 0
#define WRITE_END 1

#define MAP_PROCESS_NAME "map.c"
#define REDUCE_PROCESS_NAME "reduce.c"

int number_of_genres = 0;

int create_process(char* process_name, char* write_msg)
{
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        fprintf(stderr, "pipe failed");
        return -1;
    }

    pid = fork();

    if (pid == -1)
    {
        fprintf(stderr, "fork failed");
        return -2;
    }

    else if (pid > 0 ) //parent process
    {
        close(fd[READ_END]);
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
        close(fd[WRITE_END]);
        write(1, "in parent", 10);

    }
    
    else if (pid == 0) //child process
    {
        char read_msg[BUFFER_SIZE];
        close(fd[WRITE_END]);
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        close(fd[READ_END]);
        char path[BUFFER_SIZE];
        sprintf(path, "./%s", process_name);
        execlp(path, process_name, read_msg, number_of_genres, NULL);
    }
    return pid;
}

int create_process_map(char* write_msg)
{
    return create_process(MAP_PROCESS_NAME, write_msg);
}

int create_process_reduce(char* write_msg)
{
    return create_process(REDUCE_PROCESS_NAME, write_msg);
}

int main()
{
    char x[BUFFER_SIZE] = "./child.out";
    char y[BUFFER_SIZE] = "argument";
    write(1,"hellooo\n", 9);

    create_process(x, y);
    write(1,"hello", 6);

    // for (size_t i = 0; i < 10000000000; i++)
    // {cd
    //     if (i % 1000000000 == 0)
    //     {
    //         printf(".");
    //     }
        
    // }
    sleep(5);

    // exit(EXIT_SUCCESS);
    return 0;
}