#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;

class A
{

public:
    int x;

    A(int y) 
    {
        x = y;
        cout << "using first coonstructor" << endl;
    }
    A(int y, int z) 
    {
        cout << "using second coonstructor" << endl;
    }
    A(A& other)
    {
        cout << "wow" << endl;
        this-> x = other.x;
    }
    ~A() {}
};


int main(int argc, char const *argv[])
{
    A * p1 = new A(12);
    A * p2 = new A(*p1);

int kernel[3][3] = {{-2, 1, 0},
                    {-1, 1, 1},
                    { 0, 1, 2}};
    p2->x = 14;
    cout << kernel[0][0] << endl;


    // A a(12);
    // A b(1,14);
    
    // string s = "salam";
    // cout << s;
    // int n = 3, m = 4;
    // int x [n][m];

    return 0;
}
