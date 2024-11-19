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

    // Before manipulating files, verify that bitsPerPixel is a number between 3 and 24 and a multiple of 3.
    // this number represents how many bits of information from the input file we will pack into each 24-bit RGB pixel.
    if (bitsPerPixel < 3 ||
        bitsPerPixel > 24 ||
        bitsPerPixel % 3 > 0)
    {
        throw runtime_error("Invalid value for parameter bitsPerPixel. Must be a value between 3 and 24 and multiple of 3. Aborting embed operation.");
    }

    // Open and verify sourceDataFilePath
    auto sourceDataFileStream = ifstream(sourceDataFilePath, ios::binary);
    if (!sourceDataFileStream || 
        !sourceDataFileStream.is_open() ||
        sourceDataFileStream.bad())
    {
        throw runtime_error("Could not open source data file at " 
            + sourceDataFilePath
            + " aborting embed operation.");
    }

    Bitmap sourceBitmap;
    try
    {
        sourceBitmap.load(originalBitmapFilePath);
    }
    catch(const bmp::Exception& e)
    {
        // Repackage exception from underlying library for uniformity.
        throw runtime_error("Could not open original bitmap file at "
            + originalBitmapFilePath
            + " aborting embed operation."
            + e.what());
    }

    // perform embed operation
    // for performance reasons, we read the input in 1K chunks
    vector<char> buffer(1024);
    auto inputStreamExhausted = false;
    auto currentPixelIterator = sourceBitmap.begin();
    auto pixelsExhausted = currentPixelIterator != sourceBitmap.end();
    int inputByteBitEncodingPos = 0; // index to the bit (0 to 7) that is being encoded from the data file byte
    int pixelBitEncodingPos = 0; // index to the bit (0 to 7) where data will be stored in the current pixel.
    while(!inputStreamExhausted  &&
          !pixelsExhausted)
    {
        sourceDataFileStream.read(buffer.data(), buffer.size());
        auto bytesRead = sourceDataFileStream.gcount();

        if (bytesRead > 0)
        {
            for (streamsize i = 0; i < bytesRead; i++) 
            {
                encodeByte(buffer[i], currentPixelIterator, inputByteBitEncodingPos, pixelBitEncodingPos);
                cout << buffer[i] << " ";
            }
        }

        if (bytesRead < buffer.size())
        {
            inputStreamExhausted = true;
        }
    }
    sourceDataFileStream.close();
    sourceBitmap.save(destinationBitmapDataFilePath);
}

void steganographyLib::Steganography::extract(const std::string &source, const std::string &destination, u_int8_t bitsPerPixel)
{
    cout << "extract called";
}

void steganographyLib::Steganography::encodeByte(const char inputByte, std::vector<Pixel>::iterator &currentPixelIterator, int &inputByteBitEncodingPos, int &pixelBitEncodingPos)
{
    cout << "encodeByte called";
}
