#include <iostream>
#include "bitmap.h"
#include "steganography.h"

int main(int argc, char **argv)
{
    std::cout << "Loading bitmap\n";
    bmp::Bitmap mybitmap("sample.bmp");
    std::cout << "Image size:" << mybitmap.height() << "X" << mybitmap.width() << "\n";
    steganographyLib::Steganography mySteganography;
    return 0;
}
