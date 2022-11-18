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


void read_genres_file(char* g)
{
    int fd = open(GENRE_FILE_PATH_FORMAT, O_RDONLY);
    int last_char = read(fd, g, BUFFER_SIZE);
    g[last_char] = '\0';
    close(fd);
}

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
    }
    else if (pid == 0) //child process
    {
        close(fd[WRITE_END]);
        char fd_read[BUFFER_SIZE];
        sprintf(fd_read, "%d", fd[READ_END]);
        char path[BUFFER_SIZE];
        sprintf(path, "./%s", process_name);
        execlp(path, process_name, fd_read, NULL);
        exit(EXIT_FAILURE);
    }
    return pid;
}

int create_process_map(int file_number, char* g)
{
    char write_msg[BUFFER_SIZE];
    sprintf(write_msg, LIBRARY_PATH_FORMAT, file_number);
    strcat(write_msg, ",");
    strcat(write_msg, g);
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
        sprintf(fifo_name, "./fifo/%s_fifo,%d", token, MAP_SIZE);
        create_process(REDUCE_PROCESS_NAME, fifo_name);
        token = strtok(NULL, ",");
    }
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
        // printf( " %s created\n", fifo_name ); //printing each token
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
        create_process_map(i, g);

    // while(wait(NULL) != -1)
    // {
    //     sleep(1);
    //     printf(".\n");
    // }

    create_reduce_processes(g);

    while(wait(NULL) != -1);
    printf("all processes finished successfully2\n");

    unlink_fifo(g);
 
    printf("all processes finished successfully3\n");
    exit(EXIT_SUCCESS);
}