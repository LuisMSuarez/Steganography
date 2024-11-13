#include <iostream>
#include "bitmap.h"

int main(int argc, char **argv)
{
    std::cout << "Loading bitmap\n";
    bmp::Bitmap mybitmap("sample.bmp");
    std::cout << "Image size:" << mybitmap.height() << "X" << mybitmap.width() << "\n";
    return 0;
}
