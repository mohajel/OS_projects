#include <iostream>
#include <unistd.h>
#include <fstream>
#include "defs.h"
#include "bmp24.h"
#include "thread.h"

using namespace std;

void rvereseArray(Pixel * arr, int end)
{
    int start = 0;
    while (start < end)
    {
        Pixel temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}    


void apply_reverse_partly(int start_row, int end_row)
{
    for (size_t i = start_row; i < end_row; i++)
        rvereseArray(initial_img.data[i], initial_img.cols); 
}

void apply_reverse()
{
    int rows = initial_img.rows;
    if (EXEC_TYPE == SERIAL)
        apply_reverse_partly(0, rows);

    else if (EXEC_TYPE == PARALLEL)
    {
        Thread t(THREADS_NUM);
        int step = rows/THREADS_NUM;

        for (size_t i = 0; i < THREADS_NUM; i++)
            t.run(new Thread_msg{(int)(step * i), (int)(step * (i + 1)), apply_reverse_partly});

        t.wait();
        cout << "waiting finished for reverse" << endl;
    }; 
}




int main(int argc, char *argv[])
{
  clock_t start, end;
  start = clock();
  Pixel** picture_input;
  char *fileBuffer;
  int bufferSize;
  char fileName[30] = "input.bmp";

  fillAndAllocate(fileName, initial_img);
  read_img(initial_img);
  copy_img();

  apply_reverse();

  writeOutBmp24(initial_img.rows, initial_img.cols, initial_img.fileBuffer, FILE_OUT_SERIAL, initial_img.bufferSize, initial_img.data);
  end = clock();
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  int time_exec = (int)(time_taken * 1000);
  cout << "Time taken by " << EXEC_TYPE << " program is : " << time_exec << endl;
  return 0;
}