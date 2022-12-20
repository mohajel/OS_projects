
#include "defs.h"
#include "bmp24.h"
#include "thread.h"
#include "reverse.h"

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