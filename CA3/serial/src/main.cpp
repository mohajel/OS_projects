// In the Name of God

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include "defs.h"
#include "bmp24.h"
#include "thread.h"
#include "dimond.h"
#include "emboss.h"
#include "reverse.h"

using namespace std;

int main(int argc, char *argv[])
{
    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    if (EXEC_TYPE == PARALLEL)
        cout << "PARALLEL:" << endl;
    if (EXEC_TYPE == SERIAL)
        cout << "SERIAL:" << endl;

    char *fileName = new char[MAX_FILE_NAME_SIZE];
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

    writeOutBmp24(initial_img, FILE_OUT_SERIAL);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "[ms]" << std::endl;
    return 0;
}