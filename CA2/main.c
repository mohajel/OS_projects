// in the name of God

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "manual.h"

#define MAP_SIZE 3

#define MAP_PROCESS_NAME "map"
#define REDUCE_PROCESS_NAME "reduce"


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
        // write(1, "in parent", 10);
    }
    else if (pid == 0) //child process
    {
        char read_msg[BUFFER_SIZE];
        close(fd[WRITE_END]);
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        close(fd[READ_END]);
        char path[BUFFER_SIZE];
        sprintf(path, "./%s", process_name);
        execlp(path, process_name, read_msg, NULL);
        exit(EXIT_FAILURE);
    }
    return pid;
}

int create_process_map(int file_number)
{
    char write_msg[BUFFER_SIZE];
    sprintf(write_msg, LIBRARY_PATH_FORMAT, file_number);
    return create_process(MAP_PROCESS_NAME, write_msg);
}

void create_reduce_processes(char* g)
{

    char temp[BUFFER_SIZE];

    strcpy(temp, g);

    char* token = strtok(temp, ",");

    while(token != NULL ) 
    {
        char fifo_name[BUFFER_SIZE];
        sprintf(fifo_name, FIFO_PATH_FORMAT, token);
        create_process(REDUCE_PROCESS_NAME, fifo_name);
        token = strtok(NULL, ",");
    }

    return;
}

void create_fifo(const char* g)
{
    char temp[BUFFER_SIZE];
    strcpy(temp, g);

    char* token = strtok(temp, ",");

    while( token != NULL ) 
    {
        char fifo_name[BUFFER_SIZE];
        sprintf(fifo_name, FIFO_PATH_FORMAT, token);
        mkfifo(fifo_name, PERMISSION);
        printf( " %s created\n", fifo_name ); //printing each token
        token = strtok(NULL, ",");
    }
}

void unlink_fifo(const char* g)
{
    char temp[BUFFER_SIZE];
    strcpy(temp, g);

    char* token = strtok(temp, ",");

    while( token != NULL ) 
    {
        char fifo_name[BUFFER_SIZE];
        sprintf(fifo_name, FIFO_PATH_FORMAT, token);
        unlink(fifo_name);
        printf( " %s unlinked\n", fifo_name ); //printing each token
        token = strtok(NULL, ",");
    }
}

int main()
{
    char g[BUFFER_SIZE];
    read_genres_file(g);

    create_fifo(g);

    while(wait(NULL) != -1);

    for (int i = 1; i <= MAP_SIZE; i++) //make this better
        create_process_map(i);

    create_reduce_processes(g);

    while(wait(NULL) != -1);
    
    unlink_fifo(g);

    printf("all processes finished successfully\n");
    exit(EXIT_SUCCESS);
}