// in the name of God

#define BUFFER_SIZE 1024
#define READ_END 0
#define WRITE_END 1
#define PERMISSION 0777

#define GENRE_FILE_PATH_FORMAT "./library/genres.csv"
#define FIFO_PATH_FORMAT "./fifo/%s_fifo"
#define LIBRARY_PATH_FORMAT "./library/part%d.csv"

void read_genres_file(char* g)
{
    int fd = open(GENRE_FILE_PATH_FORMAT, O_RDONLY);
    int last_char = read(fd, g, BUFFER_SIZE);
    g[last_char] = '\0';
    close(fd);
}
