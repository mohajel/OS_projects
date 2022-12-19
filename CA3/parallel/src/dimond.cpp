#include "defs.h"
#include "bmp24.h"
#include "thread.h"
#include "dimond.h"

void apply_dimond_one_line(int i)
{
    Pixel white_pixel = {255, 255, 255};
    int m = initial_img.cols / 2;
    int n = initial_img.rows / 2;

    int j1, j2;

    if (i < n)
    {
        j1 = m - m * i / n;
        j2 = m + m * i / n;
    }
    else
    {
        j1 = m * i / n - m;
        j2 = 3 * m - m * i / n;
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
