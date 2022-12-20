// In the Name of God

#if !defined(__REVERSE__)
#define __REVERSE__

#include "defs.h"
#include "bmp24.h"
#include "thread.h"

void rvereseArray(Pixel *arr, int end);
void apply_reverse_partly(int start_row, int end_row);
void apply_reverse();

#endif // __REVERSE__