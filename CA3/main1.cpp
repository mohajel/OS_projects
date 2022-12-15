#include <iostream>
#include <unistd.h>
#include <fstream>

#define FILE_OUT_SERIAL "fuck.bmp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

#pragma pack(1)
// #pragma once

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER
{
  WORD bfType;
  DWORD bfSize;
  WORD bfReserved1;
  WORD bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

struct Pixel 
{
  unsigned char red;
  unsigned char green;
  unsigned char blue; 
};

int rows;
int cols;

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
  std::ifstream file(fileName);

  if (file)
  {
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new char[length];
    file.read(&buffer[0], length);

    PBITMAPFILEHEADER file_header;
    PBITMAPINFOHEADER info_header;

    file_header = (PBITMAPFILEHEADER)(&buffer[0]);
    info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
    rows = info_header->biHeight;
    cols = info_header->biWidth;
    bufferSize = file_header->bfSize;
    return 1;
  }
  else
  {
    cout << "File" << fileName << " doesn't exist!" << endl;
    return 0;
  }
}

void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer,  Pixel** &picture_input)
{
  pthread_t* thread_all = new pthread_t[rows];
  int count = 1;
  int extra = cols % 4;//padding 
  picture_input = new Pixel*[cols];
  for(int i = 0; i < cols; i++)
  {
    picture_input[i] = new Pixel[rows];
  }
  for (int i = 0; i < rows; i++) 
  {
    count += extra;
    for (int j = cols - 1; j >= 0; j--)
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          picture_input[i][j].red = fileReadBuffer[end - count];
          break;
        case 1:
          picture_input[i][j].green = fileReadBuffer[end - count];
          break;
        case 2:
          picture_input[i][j].blue = fileReadBuffer[end - count];
          break;
        }
        count ++;
      }
  }
}

Pixel get_mean(Pixel** &picture_input, int x, int y)
{
  Pixel ans;
  int green_temp = 0;
  int red_temp = 0;
  int blue_temp = 0;
  int cnt = 0;
  for(int i = -1; i <= 1; i++)
  {
    for(int j = -1; j <= 1; j++)
    {
      if(x + i < 0 || x + i >= rows)
        continue;
      if(y + j < 0 || y + j >= cols)
        continue;
      int cor_x = x + i;
      int cor_y = y + j;
      cnt++;
      green_temp += (picture_input[cor_x][cor_y].green);
      red_temp += (picture_input[cor_x][cor_y].red);
      blue_temp += (picture_input[cor_x][cor_y].blue);
    }
  }
  green_temp /= cnt;
  red_temp /= cnt;
  blue_temp /= cnt;
  ans.green = (unsigned char)green_temp;
  ans.red = (unsigned char)red_temp;
  ans.blue = (unsigned char)blue_temp;
  return ans;
}

void apply_smoothing_filter(Pixel** &picture_input)
{
  Pixel** picture_out;
  picture_out = new Pixel*[cols];
  for(int i = 0; i < cols; i++)
  {
    picture_out[i] = new Pixel[rows];
  }
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      if(i == 0 || j == 0 || j == cols - 1 || i == rows - 1)
        continue;
      picture_out[i][j] = get_mean(picture_input, i, j);
    }
  }
  for(int j = 0; j < cols; j++)
  {
    delete [] picture_input[j];    
  }
  delete [] picture_input;
  picture_input = picture_out;
}

void apply_sepia_filter(Pixel** &picture_input)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      int red_temp = (int)picture_input[i][j].red * 0.393 + (int)picture_input[i][j].green * 0.769 + picture_input[i][j].blue * 0.189;
      int green_temp = (int)picture_input[i][j].red * 0.349 + (int)picture_input[i][j].green * 0.686 + picture_input[i][j].blue * 0.168;
      int blue_temp = (int)picture_input[i][j].red * 0.272 + (int)picture_input[i][j].green * 0.534 + picture_input[i][j].blue * 0.131;
      if(red_temp > 255)
        red_temp = 255;
      if(green_temp > 255)
        green_temp = 255;
      if(blue_temp > 255)
        blue_temp = 255;
      picture_input[i][j].red = red_temp;
      picture_input[i][j].green = green_temp;
      picture_input[i][j].blue = blue_temp;
    }
  }
}

Pixel find_mean_all(Pixel** picture_input)
{
  int sum_green = 0;
  int sum_red = 0;
  int sum_blue = 0;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      sum_green += (int)picture_input[i][j].green;
      sum_blue += (int)picture_input[i][j].blue;
      sum_red += (int)picture_input[i][j].red;
    }
  }
  Pixel ans;
  int all = rows * cols;
  ans.green = sum_green /  all;
  ans.red = sum_red / all;
  ans.blue = sum_blue / all;
  return ans;
}

void apply_washed_out_filter(Pixel** &picture_input)
{
  Pixel mean_all = find_mean_all(picture_input);
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      picture_input[i][j].green = picture_input[i][j].green * 0.4 + mean_all.green * 0.6;
      picture_input[i][j].red = picture_input[i][j].red * 0.4 + mean_all.red * 0.6;
      picture_input[i][j].blue = picture_input[i][j].blue * 0.4 + mean_all.blue * 0.6;
    }
  }
}

bool is_in_table(int x, int y)
{
  if(x < 0 || x >= rows || y < 0 || y >= cols)
  {
    return 0;
  }
  return 1;
}

void make_white(Pixel** &picture_input, int start, int end, int row_now)
{
  int both[2] = {start, end};
  for(int i = 0; i < 2; i++)
  {
    for(int j = -1; j <= 1; j++)
    {
      int co_x = row_now;
      int co_y = both[i] + j;
      if(!is_in_table(co_x, co_y))
        continue;
      picture_input[co_x][co_y].red = 255;
      picture_input[co_x][co_y].green = 255;
      picture_input[co_x][co_y].blue = 255;
    }
  }
}

void add_cross(Pixel** &picture_input)
{
  int end = cols - 1;
  int start = 0;
  for(int i = 0; i < rows; i++)
  {
    make_white(picture_input, start, end, i);
    end--;
    start++;
  }
}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize, Pixel** picture_input)
{
  std::ofstream write(nameOfFileToCreate);
  if (!write)
  {
    cout << "Failed to write " << nameOfFileToCreate << endl;
    return;
  }
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    count += extra;
    for (int j = cols - 1; j >= 0; j--)
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          fileBuffer[bufferSize - count] = picture_input[i][j].red;
          break;
        case 1:
          fileBuffer[bufferSize - count] = picture_input[i][j].green;
          break;
        case 2:
          fileBuffer[bufferSize - count] = picture_input[i][j].blue;
          break;
        }
        count++;
      }
  }
  write.write(fileBuffer, bufferSize);
}

int main(int argc, char *argv[])
{
  clock_t start, end;
  start = clock();
  Pixel** picture_input;
  char *fileBuffer;
  int bufferSize;
  char *fileName = "input.bmp";
  if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
  {
    cout << "File read error" << endl;
    return 1;
  }
  getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer,  picture_input);
  // apply_smoothing_filter(picture_input);
  // apply_sepia_filter(picture_input);
  // apply_washed_out_filter(picture_input);
  // add_cross(picture_input);
  writeOutBmp24(fileBuffer, FILE_OUT_SERIAL, bufferSize, picture_input);
  // end = clock();
  // double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  // int time_exec = (int)(time_taken * 1000);
  // cout << "Time taken by serial program is : " << time_exec << endl;
  return 0;
}