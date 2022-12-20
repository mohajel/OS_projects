// In the Name of God

#if !defined(__EMBOSS__)
#define __EMBOSS__

#include "defs.h"
#include "bmp24.h"
#include "thread.h"

bool is_inside_img(int row_index, int col_index);
int get_red_conv(int row_index, int col_index);
int get_blue_conv(int row_index, int col_index);
int get_green_conv(int row_index, int col_index);
Pixel get_conv(int row_index, int col_index);
void apply_checkered_partly(int start_row, int end_row);
void apply_checkered();

#endif // __EMBOSS__
