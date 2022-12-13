#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;


typedef int LONG;
// typedef Pixel ** IMG;
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

class Picture
{

public:
Picture(string name, int y) 
{ 
    this->name = name;
    x = new int;
    *x = y;
}

// empty picture;
Picture(int cols, int rows)
{ 
    this->cols = cols;
    this->rows = rows
}

Picture(const Picture& sample)
{
    this->x = new int;
    *(this->x) = *(sample.x);
}

~Picture() {}

// private:

int * x;
Pixel ** img;
int rows;
int cols;
string name;

};
