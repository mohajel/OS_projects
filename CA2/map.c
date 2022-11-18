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
    char* genre_name = strtok(NULL, ",");
    int i = 0;
    while(genre_name != NULL) 
    {  
        strcpy(genres[i].name, genre_name);
        sprintf(genres[i].fifo_name, FIFO_PATH_FORMAT, genre_name);
         
        printf("... ... genres[%d] = %s \n", i, genres[i].name);
        genre_name = strtok(NULL, ",");
        i ++;
    }
    genres_size = i;
    printf(" ... ...genres_size: %d \n", genres_size);
}

int find(char* genre)
{
    for (int i = 0; i < genres_size; i++)
        if (strcmp(genre, genres[i].name) == EQUAL)
            return i;
        
    return NOT_FOUND;  
}

int main(int argc, char const *argv[])
{
    printf("    ...inside map.c with argv[1] = %s\n", argv[1]);
    int fd = atoi(argv[1]);
    char read_msg[BUFFER_SIZE];
    read(fd, read_msg, BUFFER_SIZE);
    close(fd);
    char* file_path = strtok(read_msg, ",");
    get_genres();

    // FILE * fp;
    // char * line = NULL;
    // size_t len = 0;
    // ssize_t read;

    // fp = fopen(argv[1], "r");
    // if (fp == NULL)
    //     exit(EXIT_FAILURE);

    // while ((read = getline(&line, &len, fp)) != -1) 
    // {
    //     printf("Retrieved line of length %zu:\n", read);
    //     char* token = strtok(line, ",");
    //     token = 
    // }

    // fclose(fp);
    // if (line)
    //     free(line);
    // exit(EXIT_SUCCESS);

    return 0;
}
