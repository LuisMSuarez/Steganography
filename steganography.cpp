#include <iostream>  // std::cout
#include <fstream>   // std::*fstream
#include "steganography.h"
#include "bitmap.h"

using namespace std;
using namespace bmp;

steganographyLib::Steganography::Steganography()
{
    std::cout << "Steganography constructor invoked";
}

steganographyLib::Steganography::~Steganography()
{
    std::cout << "Steganography destructor invoked";
}

void steganographyLib::Steganography::embed(const std::string &originalBitmapFilePath, const std::string &sourceDataFilePath, const std::string &destinationBitmapDataFilePath, u_int8_t bitsPerPixel)
{
    cout << "embed called\n";
    auto sourceDataFileStream = ifstream(sourceDataFilePath, ios::binary);
    if (!sourceDataFileStream.is_open())
    {
        throw runtime_error("Could not open source data file at " 
            + sourceDataFilePath
            + " aborting operation");
    }

    auto sourceBitmap = Bitmap(originalBitmapFilePath);

    // perform embed operation
    // for performance reasons, we read the input in 1K chunks
    std::vector<char> buffer(1024);
    while(sourceDataFileStream.read(buffer.data(), buffer.size()))
    {
        auto bytesRead = sourceDataFileStream.gcount();
        // Process the chunk of data in buffer
        // Note that the last chunk might not be full
        for (std::streamsize i = 0; i < bytesRead; ++i) {
            std::cout << static_cast<int>(buffer[i]) << " ";
        }
    }
    sourceDataFileStream.close();

    sourceBitmap.save(destinationBitmapDataFilePath);
}

void steganographyLib::Steganography::extract(const std::string &source, const std::string &destination, u_int8_t bitsPerPixel)
{
    cout << "extract called";
}
