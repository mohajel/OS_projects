#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include "defs.h"
#include "bmp24.h"
#include "thread.h"

using namespace std;

void rvereseArray(Pixel *arr, int end)
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
        int step = rows / THREADS_NUM;

        for (size_t i = 0; i < THREADS_NUM; i++)
            t.run(new Thread_msg{(int)(step * i), (int)(step * (i + 1)), apply_reverse_partly});

        t.wait();
    };
    cout << "reverseImg:" << TICK_UNICODE << endl;

}

bool is_inside_img(int row_index, int col_index)
{
    if (row_index == -1 || col_index == -1 ||
        row_index >= initial_img.rows || col_index >= initial_img.cols)
        return false;
    return true;
}

int get_red_conv(int row_index, int col_index)
{
    int red_pixel = 0;

    for (int i = row_index - 1; i <= row_index + 1; i++)
        for (int j = col_index - 1; j <= col_index + 1; j++)
            if (is_inside_img(i, j))
                red_pixel += (initial_img.data[i][j]).red * kernel[i - (row_index - 1)][j - (col_index - 1)];

    if (red_pixel < 0) return 0;
    if (red_pixel > 255) return 255;

    return red_pixel;
}

int get_blue_conv(int row_index, int col_index)
{
    int blue_pixel = 0;

    for (int i = row_index - 1; i <= row_index + 1; i++)
        for (int j = col_index - 1; j <= col_index + 1; j++)
            if (is_inside_img(i, j))
                blue_pixel += (initial_img.data[i][j]).blue * kernel[i - (row_index - 1)][j - (col_index - 1)];

    if (blue_pixel < 0) return 0;
    if (blue_pixel > 255) return 255;

    return blue_pixel;
}

int get_green_conv(int row_index, int col_index)
{
    int green_pixel = 0;

    for (int i = row_index - 1; i <= row_index + 1; i++)
        for (int j = col_index - 1; j <= col_index + 1; j++)
            if (is_inside_img(i, j))
                green_pixel += (initial_img.data[i][j]).green * kernel[i - (row_index - 1)][j - (col_index - 1)];

    if (green_pixel < 0) return 0;
    if (green_pixel > 255) return 255;

    return green_pixel;
}

Pixel get_conv(int row_index, int col_index)
{
    Pixel p;

    p.red = (unsigned char)(get_red_conv(row_index, col_index));
    p.green = (unsigned char)(get_green_conv(row_index, col_index));
    p.blue = (unsigned char)(get_blue_conv(row_index, col_index));

    return p;
}

void apply_checkered_partly(int start_row, int end_row)
{
    int cols = initial_img.cols;

    for (size_t i = start_row; i < end_row; i++)
        for (size_t j = 1; j < cols - 1; j++)
            converted_img.data[i][j] = get_conv(i, j);
}

void apply_checkered()
{
    int rows = initial_img.rows;
    if (EXEC_TYPE == SERIAL)
        apply_checkered_partly(0, rows);

    else if (EXEC_TYPE == PARALLEL)
    {
        Thread t(THREADS_NUM);
        int step = rows / THREADS_NUM;

        for (size_t i = 0; i < THREADS_NUM; i++)
            t.run(new Thread_msg{(int)(step * i), (int)(step * (i + 1)), apply_checkered_partly});

        t.wait();
    };

    Pixel **temp = initial_img.data;
    initial_img.data = converted_img.data;
    converted_img.data = temp;

    cout << "embossImg:" << TICK_UNICODE << endl;

}

void apply_dimond_one_line(int i)
{
    Pixel white_pixel = {255, 255,  255};
    int m = initial_img.cols / 2; 
    int n = initial_img.rows / 2;

    int j1, j2;

    if (i < n)
    { 
        j1 = m - m/n * i;
        j2 = m + m/n * i;
    }
    else
    { 
        j1 = m * i / n - m;
        j2 = 3 * m - m * i /n;
    }

    initial_img.data[i][j1] = white_pixel;
    initial_img.data[i][j2] = white_pixel;
}

void apply_dimond_partly(int start_row, int end_row)
{
    for (size_t i = start_row; i < end_row; i++)
        apply_dimond_one_line(i);
}

void apply_dimond()
{
    int rows = initial_img.rows;
    if (EXEC_TYPE == SERIAL)
        apply_dimond_partly(0, rows);

    else if (EXEC_TYPE == PARALLEL)
    {
        Thread t(THREADS_NUM);
        int step = rows / THREADS_NUM;

        for (size_t i = 0; i < THREADS_NUM; i++)
            t.run(new Thread_msg{(int)(step * i), (int)(step * (i + 1)), apply_dimond_partly});

        t.wait();
    };
    cout << "drawDimond:" << TICK_UNICODE << endl;

}

int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();
    Pixel **picture_input;
    char *fileBuffer;
    int bufferSize;

    char* fileName = new char[32];
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

    writeOutBmp24(initial_img.rows, initial_img.cols, initial_img.fileBuffer, FILE_OUT_SERIAL, initial_img.bufferSize, initial_img.data);
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    int time_exec = (int)(time_taken * 1000);
    cout << "Execution Time:" << time_exec << endl;
    return 0;
}