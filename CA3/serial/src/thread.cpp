#include <functional>
#include "defs.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "thread.h"

using namespace std;

void *runner(void *param)
{
    Thread_msg *t = (Thread_msg *)param;
    t->func(t->start_row, t->end_row);
    pthread_exit(0);
}

Thread::Thread(int n)
    : threds_num(n)
{
    this->used_threads = 0;
    pthread_attr_init(&attr);
    this->workers = new pthread_t(n);
}

void Thread::run(Thread_msg *msg)
{
    if (this->used_threads == this->threds_num)
    {
        cerr << "more threads than expected!" << endl;
        return;
    }
    pthread_create(&workers[used_threads], &attr, runner, (void *)msg);
    this->used_threads++;
}

bool Thread::wait()
{
    for (size_t i = 0; i < this->threds_num; i++)
        pthread_join(workers[i], NULL);
    return true;
}

Thread::~Thread()
{
    delete this->workers;
}