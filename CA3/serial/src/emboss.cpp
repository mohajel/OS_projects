#include "defs.h"
#include "bmp24.h"
#include "thread.h"
#include "emboss.h"

using namespace std;

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

    if (red_pixel < 0)
        return 0;
    if (red_pixel > 255)
        return 255;

    return red_pixel;
}

int get_blue_conv(int row_index, int col_index)
{
    int blue_pixel = 0;

    for (int i = row_index - 1; i <= row_index + 1; i++)
        for (int j = col_index - 1; j <= col_index + 1; j++)
            if (is_inside_img(i, j))
                blue_pixel += (initial_img.data[i][j]).blue * kernel[i - (row_index - 1)][j - (col_index - 1)];

    if (blue_pixel < 0)
        return 0;
    if (blue_pixel > 255)
        return 255;

    return blue_pixel;
}

int get_green_conv(int row_index, int col_index)
{
    int green_pixel = 0;

    for (int i = row_index - 1; i <= row_index + 1; i++)
        for (int j = col_index - 1; j <= col_index + 1; j++)
            if (is_inside_img(i, j))
                green_pixel += (initial_img.data[i][j]).green * kernel[i - (row_index - 1)][j - (col_index - 1)];

    if (green_pixel < 0)
        return 0;
    if (green_pixel > 255)
        return 255;

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
