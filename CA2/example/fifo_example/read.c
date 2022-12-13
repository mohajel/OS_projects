//in the name of God

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{

    printf("READING\n");

    int fd = open("fifooo", O_RDONLY);
    if (fd == -1)
    {
        printf("cant open -1 \n");
        return -1;
    }
    printf("connected to fifo O_RDONLY fd:%d \n", fd);

    int count = 0;
    while(1)
    {
        char input[100];
        char buf[512];

        count ++;
        if (count % 12 == 11)
            scanf("%s", input);
        
        int x = read(fd, buf, strlen(buf));
        printf("reading returns:%d , buf: %s\n", x, buf);
    }
    return 0;
}

// C program to implement one side of FIFO
// This side reads first, then reads
// #include <stdio.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <unistd.h>

// int main()
// {
//     int fd1;

//     // FIFO file path
//     // char * myfifo = "/tmp/myfifo";

//     // Creating the named file(FIFO)
//     // mkfifo(<pathname>,<permission>)
//     // mkfifo(myfifo, 0666);

//     char str1[80], str2[80];
//     while (1)
//     {
//         // First open in read only and read
//         fd1 = open("myfifo",O_RDONLY);
//         read(fd1, str1, 80);

//         // Print the read string and close
//         printf("User1: %s\n", str1);
//         close(fd1);

//         // Now open in write mode and write
//         // string taken from user.
//         fd1 = open("myfifo",O_WRONLY);
//         fgets(str2, 80, stdin);
//         write(fd1, str2, strlen(str2)+1);
//         close(fd1);
//     }
//     return 0;
// }
