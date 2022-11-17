// in the name of God

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define READ_END 0
#define WRITE_END 1
#define PERMISSION 0777

#define MAP_PROCESS_NAME "map.c"
#define REDUCE_PROCESS_NAME "reduce.c"

#define GENRE_FILE_PATH_FORMAT "./library/genres.csv"
#define FIFO_PATH_FORMAT "./fifo/%s_fifo"
#define LIBRARY_PATH_FORMAT "./library/part%s.csv"

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

int create_process_map(int file_number)
{
    char* write_msg[BUFFER_SIZE];
    sprintf(write_msg, "%d", file_number);
    return create_process(MAP_PROCESS_NAME, write_msg);
}

int create_process_reduce(char* write_msg)
{
    return create_process(REDUCE_PROCESS_NAME, write_msg);
}


void create_fifo()
{
    //read GENRE_FILE_NAME and create fifo
    //update number_of_genres too

    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, PERMISSION);
}

int main()
{
    create_fifo();

    for (int i = 1; i < "ls -1 | wc -l"; i++) //make this better
        create_process_map(i);

    for (int i = 0; i < number_of_genres; i++)// ,,,
        create_process_reduce();
    
    wait();

    // closefifo();
    return 0;
}