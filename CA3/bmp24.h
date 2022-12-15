#include <iostream>
#include <unistd.h>
#include <fstream>

#define FILE_OUT_SERIAL "outputtt.bmp"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

#pragma pack(1)



typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

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

// Img 


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


//   fillAndAllocate(initial_img.fileBuffer, fileName, initial_img.rows, initial_img.cols, initial_img.bufferSize);
// 
// bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
bool fillAndAllocate(const char *fileName, Img & img)
{
    char* &buffer = img.fileBuffer;
    int& rows = img.rows;
    int& cols = img.cols;
    int& bufferSize = img.bufferSize;

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

void allocate_memory(int cols, int rows, Pixel **& pic)
{
    pic = new Pixel *[cols];
    for (int i = 0; i < cols; i++)
        pic[i] = new Pixel[rows];
}



// void getPixlesFromBMP24(int end, int start_rows, int end_rows, int cols, char *fileReadBuffer, Pixel **&pic)

//   getPixlesFromBMP24(initial_img.bufferSize, initial_img.rows, initial_img.cols, initial_img.fileBuffer,  initial_img.data);

// void getPixlesFromBMP24(int end, int rows, int cols, char *fileReadBuffer, Pixel **&pic)
void getPixlesFromBMP24(Img& img, int start_row, int end_row)
{
    int end = img.bufferSize;
    int rows = img.rows;
    int cols = img.cols;
    char* fileReadBuffer =img.fileBuffer;
    Pixel**& pic = img.data;
    int padding = cols % 4;
    int count = start_row * 3 * cols + padding * start_row + 1;
    
    for (int i = start_row; i < end_row; i++)
    {
        count += padding;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++)
            {
                switch (k)
                {
                case 0:
                    pic[i][j].red = fileReadBuffer[end - count];
                    break;
                case 1:
                    pic[i][j].green = fileReadBuffer[end - count];
                    break;
                case 2:
                    pic[i][j].blue = fileReadBuffer[end - count];
                    break;
                }
                count++;
            }
    }
}

void read_img(Img& img)
{
    Pixel**& pic = img.data;
    int rows = img.rows;
    int cols = img.cols;
    allocate_memory(cols, rows, pic);
    getPixlesFromBMP24(img, rows/2, rows);
}

void writeOutBmp24(int rows, int cols, char *fileBuffer, const char *nameOfFileToCreate, int bufferSize, Pixel **pic)
{
    std::ofstream write(nameOfFileToCreate);
    if (!write)
    {
        cout << "Failed to write " << nameOfFileToCreate << endl;
        return;
    }
    int count = 1;
    int padding = cols % 4;
    for (int i = 0; i < rows; i++)
    {
        count += padding;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++)
            {
                switch (k)
                {
                case 0:
                    fileBuffer[bufferSize - count] = pic[i][j].red;
                    break;
                case 1:
                    fileBuffer[bufferSize - count] = pic[i][j].green;
                    break;
                case 2:
                    fileBuffer[bufferSize - count] = pic[i][j].blue;
                    break;
                }
                count++;
            }
    }
    write.write(fileBuffer, bufferSize);
}
