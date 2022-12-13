#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;

class A
{
private:
    int x;
public:
    A(int y) 
    {
        cout << "using first coonstructor" << endl;
    }
    A(int y, int z) 
    {
        cout << "using second coonstructor" << endl;
    }
    ~A() {}
};


int main(int argc, char const *argv[])
{
    A a(12);
    A b(1,14);
    
    string s = "salam";
    cout << s;
    int n = 3, m = 4;
    int x [n][m];

    return 0;
}
