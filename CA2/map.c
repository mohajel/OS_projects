// in the name of God

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "manual.h"

#define MAX_GENRES_SIZE 100
#define EQUAL 0
#define NOT_FOUND -1

struct Genre
{
    char name[BUFFER_SIZE];
    char fifo_name[BUFFER_SIZE];
    int count;
};

struct Genre genres[MAX_GENRES_SIZE];
int genres_size = 0;

void get_genres()
{
    char *genre_name = strtok(NULL, ",");
    int i = 0;
    while (genre_name != NULL)
    {
        strcpy(genres[i].name, genre_name);
        sprintf(genres[i].fifo_name, FIFO_PATH_FORMAT, genre_name);

        genre_name = strtok(NULL, ",");
        i++;
    }
    genres_size = i;
}

int find(char *genre)
{
    // BUG FIX:
    // handling /n
    int last = strlen(genre);
    if (genre[last - 1] == '\n')
        genre[last - 1] = '\0';

    for (int i = 0; i < genres_size; i++)
        if (strcmp(genre, genres[i].name) == EQUAL)
            return i;

    return NOT_FOUND;
}

void send(char *fifo_name, int data, char *file_path)
{
    int fd = open(fifo_name, O_WRONLY);

    if (fd == -1)
        printf("cant open %s to write\n", file_path);

    int x = write(fd, &data, sizeof(int));

    if (x == -1)
        printf("cant write in %s \n", file_path);

    close(fd);
}

void count_genres(char *file_path)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *token = strtok(line, ",");
        while (token != NULL)
        {
            // printf("tokenis:%s\n", token);
            int index = find(token);
            if (index != NOT_FOUND)
                genres[index].count++;
            token = strtok(NULL, ",");
        }
    }
    fclose(fp);
    if (line)
        free(line);
}

void print_status(char *file_path)
{
    printf("*****************************************\n");
    printf("%s status:\n", file_path);
    for (int i = 0; i < genres_size; i++)
    {
        printf("%s :..%s count: %d\n", file_path, genres[i].name, genres[i].count);
    }
    printf("*****************************************\n");
}

int main(int argc, char const *argv[])
{
    int fd = atoi(argv[1]);
    char read_msg[BUFFER_SIZE];
    read(fd, read_msg, BUFFER_SIZE);
    close(fd);
    char *file_path = strtok(read_msg, ",");

    printf("    ...inside map.c : %s\n", file_path);

    get_genres();

    count_genres(file_path);

    // print_status(file_path);

    for (int i = 0; i < genres_size; i++)
    {
        send(genres[i].fifo_name, genres[i].count, file_path);
        // printf("**********%s sending %d to %s \n",file_path, genres[i].count ,genres[i].fifo_name);
    }

    printf("    ...%s finished\n", file_path);
    exit(EXIT_SUCCESS);
}
