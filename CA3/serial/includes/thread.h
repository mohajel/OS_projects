// In the Name of God

#if !defined(__THREAD__)
#define __THREAD__

#include <functional>
#include "defs.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

struct Thread_msg
{
    int start_row;
    int end_row;
    void (*func)(int, int);
};

void *runner(void *param);

class Thread
{
private:
    int threds_num;
    int used_threads;
    pthread_attr_t attr;
    pthread_t *workers;

public:
    Thread(int n);
    ~Thread();
    void run(Thread_msg *msg);
    bool wait();
};

#endif // __THREAD__
