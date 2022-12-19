#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include "defs.h"
#include "bmp24.h"
#include "thread.h"
#include "dimond.h"
#include "emboss.h"
#include "reverse.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (EXEC_TYPE == PARALLEL)
        cout << "PARALLEL:" << endl;
     if (EXEC_TYPE == SERIAL)
        cout << "SERIAL:" << endl;

    char* fileName = new char[MAX_FILE_NAME_SIZE];
    if (argc == 2)
        fileName = argv[1];
    else
        strcpy(fileName, "input.bmp");

    clock_t start, end;
    start = clock();

    fillAndAllocate(fileName, initial_img);
    read_img(initial_img);
    copy_img();

    apply_reverse();
    apply_checkered();
    apply_dimond();

    writeOutBmp24(initial_img, FILE_OUT_SERIAL);

    end = clock();
    int time_exec = (int)(double(end - start) / double(CLOCKS_PER_SEC) * 1000);
    cout << "Execution Time:" << time_exec << endl;
    return 0;
}