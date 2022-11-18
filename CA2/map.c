// in the name of God

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "manual.h"

int main(int argc, char const *argv[])
{
    printf("    ...inside map.c with argv[1] = %s\n", argv[1]);
    return 0;
}
