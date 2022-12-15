#include <iostream>
#include <pthread.h>
#include "thread.h"

using namespace std; 

void * runner (void * param)
{
    Thread_msg *t = (Thread_msg *) param;

    t->func(t->start_row, t->end_row);
    pthread_exit(0);
}

void f(int x ,int y)
{
    cout << x + y << endl;
}

void g(int x ,int y)
{
    cout << x - y << endl;
}

int main(int argc, char const *argv[])
{
    pthread_t workers[2];

    pthread_attr_t attr;

    Thread_msg t1 = {1 , 11, f};
    Thread_msg t2 = {1 , 11, g};



    int x[] = {1,2,3,4,5,6,7,8,9,10};

    pthread_attr_init(&attr);

    int f = 34;
    int g = 5;

    pthread_create(&workers[0], &attr, runner, (void*)& t1);
    pthread_create(&workers[1], &attr, runner, (void*)& t2);

    for (size_t i = 0; i < 2; i++)
    {
        pthread_join(workers[i], NULL);
    }


    
    return 0;
}
