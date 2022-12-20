// In the Name of God

#if !defined(__BMP24__)
#define __BMP24__

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include "defs.h"
#include "thread.h"

struct Pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Img
{
    int rows;
    int cols;
    char *fileBuffer;
    int bufferSize;
    Pixel **data;
};

extern Img initial_img;
extern Img converted_img;

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

bool fillAndAllocate(const char *fileName, Img &img);
void allocate_memory(int cols, int rows, Pixel **&pic);
void getPixlesFromBMP24(int start_row, int end_row);
void copy_img();
void read_img(Img &img);
void writeOutBmp24(Img &img, const char *output_file_name);

#endif // __BMP24__