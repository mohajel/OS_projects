#include <iostream>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include "defs.h"
#include "thread.h"
#include "bmp24.h"

Img initial_img;
Img converted_img;

bool fillAndAllocate(const char *fileName, Img &img)
{
    char *&buffer = img.fileBuffer;
    int &rows = img.rows;
    int &cols = img.cols;
    int &bufferSize = img.bufferSize;

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
        cout << "File " << fileName << " doesn't exist!" << endl;
        exit(EXIT_FAILURE);
    }
}

void allocate_memory(int cols, int rows, Pixel **&pic)
{
    pic = new Pixel *[rows];
    for (int i = 0; i < rows; i++)
        pic[i] = new Pixel[cols];
}

void getPixlesFromBMP24(int start_row, int end_row)
{
    Img &img = initial_img;
    int end = img.bufferSize;
    int rows = img.rows;
    int cols = img.cols;
    char *fileReadBuffer = img.fileBuffer;
    Pixel **&pic = img.data;
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

void copy_img()
{
    converted_img.rows = initial_img.rows;
    converted_img.cols = initial_img.cols;
    allocate_memory(converted_img.cols, converted_img.rows, converted_img.data);
    converted_img.fileBuffer = initial_img.fileBuffer;
    converted_img.bufferSize = initial_img.bufferSize;
}

void read_img(Img &img)
{
    Pixel **&pic = img.data;
    int rows = img.rows;
    int cols = img.cols;
    allocate_memory(cols, rows, pic);

    if (EXEC_TYPE == SERIAL)
        getPixlesFromBMP24(0, rows);

    else if (EXEC_TYPE == PARALLEL)
    {
        Thread t(THREADS_NUM);
        int step = rows / THREADS_NUM;

        for (size_t i = 0; i < THREADS_NUM; i++)
            t.run(new Thread_msg{(int)(step * i), (int)(step * (i + 1)), getPixlesFromBMP24});

        t.wait();
        cout << "readImg:" << TICK_UNICODE << endl;
    };
}

void writeOutBmp24(Img &img, const char *output_file_name)
{
    int rows = img.rows;
    int cols = img.cols;
    Pixel **&pic = img.data;
    char *fileBuffer = img.fileBuffer;
    int bufferSize = img.bufferSize;

    std::ofstream write(output_file_name);
    if (!write)
    {
        cout << "Failed to write " << output_file_name << endl;
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
    cout << "writeImg:" << TICK_UNICODE << endl;
}
