#include <iostream>
#include <unistd.h>
#include <fstream>
#include "defs.h"
#include "bmp24.h"

using namespace std;

Img initial_img;
Img converted_img;

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

  // apply_smoothing_filter(picture_input);
  // apply_sepia_filter(picture_input);
  // apply_washed_out_filter(picture_input);
  writeOutBmp24(initial_img.rows, initial_img.cols, initial_img.fileBuffer, FILE_OUT_SERIAL, initial_img.bufferSize, initial_img.data);
  // end = clock();
  // double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  // int time_exec = (int)(time_taken * 1000);
  // cout << "Time taken by serial program is : " << time_exec << endl;
  return 0;
}