#include <iostream>
#include "steganography.h"

steganographyLib::Steganography::Steganography()
{
    std::cout << "Steganography constructor invoked";
}

steganographyLib::Steganography::~Steganography()
{
}

void steganographyLib::Steganography::embedInformation(const std::string &originalBitmapFilePath, const std::string &sourceDataFilePath, const std::string &destinationBitmapDataFilePath, u_int8_t bitsPerPixel)
{
}

void steganographyLib::Steganography::extractInformation(const std::string &source, const std::string &destination, u_int8_t bitsPerPixel)
{
}
