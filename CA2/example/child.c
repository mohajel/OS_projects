//in the name of God

#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("running child processsss");
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s \n", i, argv[i]);
    }
    printf("exit child");
    exit(EXIT_SUCCESS);
    return 0;
}
