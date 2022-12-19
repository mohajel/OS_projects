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
    clock_t start, end;
    start = clock();

    Pixel **picture_input;
    char *fileBuffer;
    int bufferSize;

    char* fileName = new char[32];
    if (argc == 2)
        fileName = argv[1];
    else
        strcpy(fileName, "input.bmp");

    fillAndAllocate(fileName, initial_img);
    read_img(initial_img);
    copy_img();

    apply_reverse();
    apply_checkered();
    apply_dimond();

    writeOutBmp24(initial_img.rows, initial_img.cols, initial_img.fileBuffer, FILE_OUT_SERIAL, initial_img.bufferSize, initial_img.data);
    end = clock();
    int time_exec = (int)(double(end - start) / double(CLOCKS_PER_SEC) * 1000);
    cout << "Execution Time:" << time_exec << endl;
    return 0;
}