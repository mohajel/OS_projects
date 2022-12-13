//in the name of God
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>


int main(int argc, char const *argv[])
{
    printf("WRITING\n");
    int fd = open("fifooo", O_WRONLY);
    if (fd == -1)
    {
        printf("cant open -1 \n");
        return -1;
    }
    printf("connected to fifo O_WRONLY fd:%d \n", fd);

    write(fd, "salam\n", 6);

    char input[100];
    while(1)
    {
        scanf("%s", input);

        if (strcmp(input, "close") == 0)
        {
            printf("closing\n");
            close(fd);
            break;
            // printf("reopening\n");

            // fd = open("fifo", O_WRONLY | O_NONBLOCK);
            // if (fd == -1)
            // {
            //     printf("cant open -1 \n");
            //     return -1;
            // }
        }
        

        int x = write(fd, input, strlen(input));

        printf("writing returns:%d\n", x);
        // fsync(fd);
    }

    
    return 0;
}
// C program to implement one side of FIFO
// This side writes first, then reads
// #include <stdio.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <unistd.h>

// int main()
// {
    // int fd;

    // FIFO file path
    // char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    // mkfifo("myfifo", 0666);

    // char arr1[80], arr2[80];
    // while (1)
    // {
    //     // Open FIFO for write only
    //     fd = open("myfifo", O_WRONLY);

    //     // Take an input arr2ing from user.
    //     // 80 is maximum length
    //     fgets(arr2, 80, stdin);

    //     // Write the input arr2ing on FIFO
    //     // and close it
    //     write(fd, arr2, strlen(arr2)+1);
    //     close(fd);

    //     // Open FIFO for Read only
    //     fd = open("myfifo", O_RDONLY);

    //     // Read from FIFO
    //     read(fd, arr1, sizeof(arr1));

    //     // Print the read message
    //     printf("User2: %s\n", arr1);
    //     close(fd);
    // }
    // return 0;
// }