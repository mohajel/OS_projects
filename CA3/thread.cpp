#include <iostream>
#include <pthread.h>

using namespace std; 

void * runner (void * param)
{
    // cout << param;
    int * x = (int *) param;
    cout << (*x) << "hello";
    *x = 55;
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    pthread_t workers[2];

    pthread_attr_t attr;

    int x[] = {1,2,3,4,5,6,7,8,9,10};

    pthread_attr_init(&attr);

    int f = 34;
    int g = 5;

    pthread_create(&workers[0], &attr, runner, (void*)& f);
    pthread_create(&workers[1], &attr, runner, (void*)&f);

    for (size_t i = 0; i < 2; i++)
    {
        pthread_join(workers[i], NULL);
    }


    
    return 0;
}
