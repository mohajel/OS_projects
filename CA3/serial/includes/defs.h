// In the Name of God

#if !defined(__DEFS__)
#define __DEFS__


#pragma pack(1)
#define RED 0
#define GREEN 0
#define BLUE 0

#define TICK_UNICODE "\xE2\x9C\x93"
#define MAX_FILE_NAME_SIZE 1024

#define SERIAL 0
#define PARALLEL 1
#define FILE_OUT_SERIAL "outputImg.bmp"
#define THREADS_NUM 6

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

const bool EXEC_TYPE = SERIAL;

const int kernel[3][3] = {{-2, -1, 0},
                        {-1,  1, 1},
                        { 0,  1, 2}};

#endif // __DEFS__


