// In the Name of God

#if !defined(__THREAD__)
#define __THREAD__

#include <functional>
#include "defs.h"
#include <iostream>
#include <unistd.h>
#include <fstream>

using namespace std;


struct Thread_msg
{
    int start_row;
    int end_row;
    void (*func)(int, int);
};

void *runner (void * param)
{
    Thread_msg *t = (Thread_msg *) param;
    t->func(t->start_row, t->end_row);
    pthread_exit(0);
}

class Thread
{
private:
    int threds_num;
    int used_threads;
    pthread_attr_t attr;
    pthread_t* workers;

public:
    Thread(int n)
        :
        threds_num(n)
    {
        this->used_threads = 0;
        pthread_attr_init(&attr);
        this->workers = new pthread_t(n);
    }
    
    // void run(function<void(int, int)> func, int start_row, int end_row)
    void run(Thread_msg *msg)
    {
        if (this->used_threads == this->threds_num)
        {
            cerr << "more threads than expected!" << endl;
            return;
        }
        pthread_create(&workers[used_threads], &attr, runner, (void*) msg);
        this->used_threads ++;
    }

    bool wait()
    {
        for (size_t i = 0; i < this->threds_num; i++)
            pthread_join(workers[i], NULL);
        return true;
    }

    ~Thread()
    {
        delete this->workers;
    }
};

#endif // __THREAD__
