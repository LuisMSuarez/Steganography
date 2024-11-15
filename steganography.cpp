#include <iostream>
#include "steganography.h"

using namespace std;

steganographyLib::Steganography::Steganography()
{
    std::cout << "Steganography constructor invoked";
}

steganographyLib::Steganography::~Steganography()
{
}

void steganographyLib::Steganography::embed(const std::string &originalBitmapFilePath, const std::string &sourceDataFilePath, const std::string &destinationBitmapDataFilePath, u_int8_t bitsPerPixel)
{
    cout << "embed called\n";
}

void steganographyLib::Steganography::extract(const std::string &source, const std::string &destination, u_int8_t bitsPerPixel)
{
    cout << "extract called";
}
