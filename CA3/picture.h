#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Picture;

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

struct thread_info
{
    Picture *source;
    Picture *dest;
    int start_row;
    int end_row;
};

struct Pixel
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

class Picture
{

public:
    Picture(char *name)
    {
        cout << "in constructor" << endl;
        this->name = name;
        this->read(this->name);
    }

    bool read(char *name)
    {
        if (!this->fillAndAllocate(this->buffer, name, this->rows, this->cols, this->buffer_size))
        {
            cerr << "File read error" << endl;
            return false;
        }
        cout << "in read" << endl;

        this->alloc_img(this->cols, this->rows);
        cout << "in alooocimg" << endl;

        this->getPixlesFromBMP24(this->buffer_size, this->rows, this->cols, this->buffer);
        return true;
    }

    Pixel ** img;
    size_t rows;
    size_t cols;
    char *name;
    char *buffer;
    int buffer_size;

    bool fillAndAllocate(char *&buffer, const char *fileName, size_t &rows, size_t &cols, int &bufferSize)
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

    void getPixlesFromBMP24(int end, size_t rows, size_t cols, char *fileReadBuffer)
    {
        cout << "reaching here" << endl;
        size_t count = 1;
        int padding = cols % 4; // padding
        for (size_t i = 0; i < rows; i++)
        {
            count += padding;
            for (size_t j = cols - 1; j >= 0; j--)
                for (int k = 0; k < 3; k++)
                {
                    switch (k)
                    {
                    case 0:
                        img[i][j].red = fileReadBuffer[end - count];
                        break;
                    case 1:
                        img[i][j].green = fileReadBuffer[end - count];
                        break;
                    case 2:
                        img[i][j].blue = fileReadBuffer[end - count];
                        break;
                    }
                    count++;
                }
        }
    }

    void write(char *outfile_name)
    {
        writeOutBmp24(this->buffer, outfile_name, this->buffer_size);
    }

    bool writeOutBmp24(char *fileBuffer, const char *nameOfFileToCreate, int bufferSize)
    {
        std::ofstream write(nameOfFileToCreate);
        if (!write)
        {
            cout << "Failed to write " << nameOfFileToCreate << endl;
            return false;
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
                        fileBuffer[bufferSize - count] = img[i][j].red;
                        break;
                    case 1:
                        fileBuffer[bufferSize - count] = img[i][j].green;
                        break;
                    case 2:
                        fileBuffer[bufferSize - count] = img[i][j].blue;
                        break;
                    }
                    count++;
                }
        }
        write.write(fileBuffer, bufferSize);
        return true;
    }

    void alloc_img(size_t cols, size_t rows)
    {
        cout << "in aaalll" << endl;

        cout << "clos:" << cols << "rows:" << rows << endl;

        // img.resize(rows, vector<Pixel>(cols));

        img = new Pixel*[rows];
        for(int i = 0; i < rows; i++)
            img[i] = new Pixel[cols];
        cout << "in aaalll" << endl;

    }

    // void getPixlesFromBMP24()
    // {
    //     int end = this->bufferSize;
    //     int count = 1;
    //     int padding = this->cols % 4;
    //     for (int i = 0; i < rows; i++)
    //     {
    //         count += padding;
    //         for (int j = this->cols - 1; j >= 0; j--)
    //         for (int k = 0; k < 3; k++)
    //         {
    //             switch (k)
    //             {
    //             case 0:
    //             this->img[i][j].red = this->buffer[end - count];
    //             break;
    //             case 1:
    //             this->img[i][j].green = this->buffer[end - count];
    //             break;
    //             case 2:
    //             this->img[i][j].blue = this->buffer[end - count];
    //             break;
    //             }
    //             count ++;
    //         }
    //     }
    // }

    // void writeOutBmp24(const char *nameOfFileToCreate)
    // {
    //     std::ofstream write(nameOfFileToCreate);
    //     if (!write)
    //     {
    //         cout << "Failed to write " << nameOfFileToCreate << endl;
    //         return;
    //     }
    //     int count = 1;
    //     int padding = cols % 4;
    //     for (int i = 0; i < rows; i++)
    //     {
    //         count += padding;
    //         for (int j = cols - 1; j >= 0; j--)
    //         for (int k = 0; k < 3; k++)
    //         {
    //             switch (k)
    //             {
    //             case 0:
    //             this->buffer[this->bufferSize - count] = this->img[i][j].red;
    //             break;
    //             case 1:
    //             this->buffer[this->bufferSize - count] = this->img[i][j].green;
    //             break;
    //             case 2:
    //             this->buffer[this->bufferSize - count] = this->img[i][j].blue;
    //             break;
    //             }
    //             count++;
    //         }
    //     }
    //     write.write(this->buffer, this->bufferSize);
    // }

    // bool initilize_pic()
    // {
    //     cout << "helwwlo";

    //     std::ifstream file(this->name);
    //     if (!file)
    //         return false;
    //     cout << "hello" << endl;

    //     file.seekg(0, std::ios::end);
    //     std::streampos length = file.tellg();
    //     file.seekg(0, std::ios::beg);

    //     this->buffer = new char[length];
    //     file.read(&(this->buffer[0]), length);
    //     cout << "helwwlo";

    //     PBITMAPFILEHEADER file_header;
    //     PBITMAPINFOHEADER info_header;

    //     file_header = (PBITMAPFILEHEADER)(&(this->buffer[0]));
    //     info_header = (PBITMAPINFOHEADER)(&(this->buffer[0]) + sizeof(BITMAPFILEHEADER));
    //     this->rows = info_header->biHeight;
    //     this->cols = info_header->biWidth;
    //     this->bufferSize = file_header->bfSize;
    //     return true;
    // }

    // empty picture;
    // Picture(int cols, int rows)
    // {
    //     alloc_img(cols, rows);
    // }

    // Picture(const Picture& sample)
    // {
    //     this->x = new int;
    //     *(this->x) = *(sample.x);
    // }

    ~Picture() {}

    // private:
};
