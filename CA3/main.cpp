// In the Name of God

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>

#include "picture.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string img_name = "input.bmp";

    Picture input_picture(img_name, 14);

    Picture p2 = input_picture;

    *(p2.x) = 16;

    cout << *(input_picture.x) << endl;
    cout << *(p2.x) << endl;

    return 0;
}


