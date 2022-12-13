#include <iostream>
#include <unistd.h>
#include <fstream>
#include<threads.h>
#include <time.h>
#define FILE_OUT_SERIAL "output.bmp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


int const MAX_ROW = 1000, MAX_COL = 1000;

unsigned char picture_input[MAX_ROW][MAX_COL][3];
unsigned char picture_out[MAX_ROW][MAX_COL][3];
bool ok[MAX_ROW][MAX_COL][3];

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

int const SIZE_ROWS_READ = 8, SIZE_ROWS_FILTER = 100;

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

struct MessageSmoothing 
{
  int color;
  unsigned char**** picture_input;
  unsigned char**** picture_out;
};

struct ThreadMessage 
{
  int row;
  int end_row;
  unsigned char**** picture_input;
  unsigned char**** picture_out;
  char** fileReadBuffer;
  int bufferSize;
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

void* getting_pixels_row(void* arg_inp)
{
  ThreadMessage* message = (ThreadMessage*)arg_inp;
  int start_row = message->row;
  int end_row = message->end_row;
  int extra = cols % 4;
  int count = start_row * 3 * cols + extra * start_row + 1;
  char** fileReadBuffer = message->fileReadBuffer;
  int end = message->bufferSize;
  for(int i = start_row; i <= end_row; i++)
  {
    for (int j = cols - 1; j >= 0; j--)
    {
      for (int k = 0; k < 3; k++)
      {
        picture_input[i][j][k] = (*fileReadBuffer)[end - count];
        count++;
      }
    }
  }
  pthread_exit(NULL);
}

void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer)
{
  int count = 1;
  int extra = cols % 4;
  ThreadMessage* messages = new ThreadMessage[rows / SIZE_ROWS_FILTER + 1];
  pthread_t* threads_all = new pthread_t[rows / SIZE_ROWS_FILTER + 1];

  for (int i = 0; i < rows; i += SIZE_ROWS_FILTER)
  {
    messages[i / SIZE_ROWS_FILTER].row = i;
    messages[i / SIZE_ROWS_FILTER].fileReadBuffer = &fileReadBuffer;
    messages[i / SIZE_ROWS_FILTER].bufferSize = end;
    messages[i / SIZE_ROWS_FILTER].end_row = i + SIZE_ROWS_FILTER - 1;
    if(messages[i / SIZE_ROWS_FILTER].end_row >= rows)
      messages[i / SIZE_ROWS_FILTER].end_row = rows - 1;
    bool error = pthread_create(&threads_all[i / SIZE_ROWS_FILTER], NULL, getting_pixels_row, (void*)&messages[i / SIZE_ROWS_FILTER]);
    if(error)
    {
      cout << "Error occured in pthread_create" << endl;
      exit(-1);
    }
  }
  for (int i = 0; i < rows; i+=SIZE_ROWS_FILTER)
  {
    bool error = pthread_join(threads_all[i / SIZE_ROWS_FILTER], NULL);
    if(error)
    {
      cout << "Error occured in pthread_join" << endl;
      exit(-1);
    }
  }
  delete [] threads_all;
  delete [] messages;
}

unsigned char get_mean(int x, int y, int color)
{
  int ans = 0;
  for(int i = -1; i <= 1; i++)
  {
    for(int j = -1; j <= 1; j++)
    {
      ans += picture_input[x + i][y + j][color];
    }
  }
  ans /= 9;
  unsigned char ret;
  ret = (unsigned char)ans;
  return ans;
}

void spaia_one_pixel(int i, int j)
{
  int red_temp = (int)picture_out[i][j][0] * 0.393 + (int)picture_out[i][j][1] * 0.769 + (int)picture_out[i][j][2] * 0.189;
  int green_temp = (int)picture_out[i][j][0] * 0.349 + (int)picture_out[i][j][1] * 0.686 + (int)picture_out[i][j][2] * 0.168;
  int blue_temp = (int)picture_out[i][j][0] * 0.272 + (int)picture_out[i][j][1] * 0.534 + (int)picture_out[i][j][2] * 0.131;
  if(red_temp > 255)
    red_temp = 255;
  if(green_temp > 255)
      green_temp = 255;
  if(blue_temp > 255)
      blue_temp = 255;
  picture_out[i][j][0] = red_temp;
  picture_out[i][j][1] = green_temp;
  picture_out[i][j][2] = blue_temp;
}


void* apply_smoothing_color(void* arg_inp)
{
  long color = (long)arg_inp;
  int ans = 0;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      if(i != 0 && j != 0 && i != rows - 1 && j != cols - 1)
       picture_out[i][j][color] = get_mean(i, j, color);
      else 
        picture_out[i][j][color] = picture_input[i][j][color];
      ok[i][j][color] = 1;
      if(ok[i][j][0] && ok[i][j][1] && ok[i][j][2])
        spaia_one_pixel(i, j);
    }
  }
  pthread_exit(NULL);
}

void apply_smoothing_filter()
{
  pthread_t* threads_all = new pthread_t[3];
  for(long i = 0; i < 3; i++)
  {
      bool error = pthread_create(&threads_all[i], NULL, apply_smoothing_color, (void*)i);
      if(error)
      {
        cout << "Error occured in pthread_create" << endl;
        exit(-1);
      }
  } 
  for (int i = 0; i < 3; i++)
  {
    bool error = pthread_join(threads_all[i], NULL);
    if(error)
    {
      cout << "Error occured in pthread_join" << endl;
      exit(-1);
    }
  }
  delete [] threads_all;
}

void* apply_sepia_filter(void* arg_inp)
{
  ThreadMessage* message = (ThreadMessage*)arg_inp;
  int start_row = message->row;
  int end_row = message->end_row;;
  for(int i = start_row; i <= end_row; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      int red_temp = (int)picture_input[i][j][0] * 0.393 + (int)picture_input[i][j][1] * 0.769 + picture_input[i][j][2] * 0.189;
      int green_temp = (int)picture_input[i][j][0] * 0.349 + (int)picture_input[i][j][1] * 0.686 + picture_input[i][j][2] * 0.168;
      int blue_temp = (int)picture_input[i][j][0] * 0.272 + (int)picture_input[i][j][1] * 0.534 + picture_input[i][j][2] * 0.131;
      if(red_temp > 255)
        red_temp = 255;
      if(green_temp > 255)
        green_temp = 255;
      if(blue_temp > 255)
        blue_temp = 255;
      picture_input[i][j][0] = red_temp;
      picture_input[i][j][1] = green_temp;
      picture_input[i][j][2] = blue_temp;
    }
  }
  pthread_exit(NULL);
}

void apply_all_sepia_filter()
{
  ThreadMessage* messages = new ThreadMessage[rows / SIZE_ROWS_READ + 1];
  pthread_t* threads_all = new pthread_t[rows / SIZE_ROWS_READ + 1];

  for (int i = 0; i < rows; i += SIZE_ROWS_READ)
  {
    messages[i / SIZE_ROWS_READ].row = i;
    messages[i / SIZE_ROWS_READ].end_row = i + SIZE_ROWS_READ - 1;
    if(messages[i / SIZE_ROWS_READ].end_row >= rows)
      messages[i / SIZE_ROWS_READ].end_row = rows - 1;
    bool error = pthread_create(&threads_all[i / SIZE_ROWS_READ], NULL, apply_sepia_filter, (void*)&messages[i / SIZE_ROWS_READ]);
    if(error)
    {
      cout << "Error occured in pthread_create" << endl;
      exit(-1);
    }
  }
  for (int i = 0; i < rows; i+=SIZE_ROWS_READ)
  {
    bool error = pthread_join(threads_all[i / SIZE_ROWS_READ], NULL);
    if(error)
    {
      cout << "Error occured in pthread_join" << endl;
      exit(-1);
    }
  }
  delete [] threads_all;
  delete [] messages;
}

unsigned char find_mean_all(int color)
{
  int sum = 0;
  unsigned char ans;
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
        sum += (int)picture_out[i][j][color];
    }
  }
  int all = rows * cols;
  ans = (unsigned char) (sum / all);
  return ans;
}

void apply_washed_out_filter(int color)
{
  unsigned char ret = find_mean_all(color);
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < cols; j++)
    {
      picture_out[i][j][color] = picture_out[i][j][color] * 0.4 + ret * 0.6;
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

void make_white(int start, int end, int row_now, int color)
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
      picture_out[co_x][co_y][color] = 255;
    }
  }
}

void add_cross(int color)
{
  int end = cols - 1;
  int start = 0;
  for(int i = 0; i < rows; i++)
  {
    make_white(start, end, i, color);
    end--;
    start++;
  }
}

void writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
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
        fileBuffer[bufferSize - count] = picture_out[i][j][k];
        count++;
      }
  }
  write.write(fileBuffer, bufferSize);
}

void* last_two_filter(void* arg_inp)
{
  long color = (long)arg_inp;
  apply_washed_out_filter(color);
  add_cross(color);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  clock_t start, end;
  start = clock();
  char *fileBuffer;
  int bufferSize;
  char *fileName = "input.bmp";
  if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize))
  {
    cout << "File read error" << endl;
    return 1;
  }
  getPixlesFromBMP24(bufferSize, rows, cols, fileBuffer);
  apply_smoothing_filter();
  pthread_t* threads_all = new pthread_t[3];
  for(long i = 0; i < 3; i++)
  {
      bool error = pthread_create(&threads_all[i], NULL, last_two_filter, (void*)i);
      if(error)
      {
        cout << "Error occured in pthread_create" << endl;
        exit(-1);
      }
  } 
  for (int i = 0; i < 3; i++)
  {
    bool error = pthread_join(threads_all[i], NULL);
    if(error)
    {
      cout << "Error occured in pthread_join" << endl;
      exit(-1);
    }
  }
  writeOutBmp24(fileBuffer, FILE_OUT_SERIAL, bufferSize);

  end = clock();
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  int time_exec = (int)(time_taken * 1000);
  cout << "Time taken by parallel program is : " << time_exec << endl;
  return 0;
}