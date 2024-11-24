#include <iostream>  // std::cout
#include <fstream>   // std::*fstream
#include "steganography.h"

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
    // Before manipulating files, verify that bitsPerPixel is a number between 3 and 24 and a multiple of 3.
    // this number represents how many bits of information from the input file we will pack into each 24-bit RGB pixel.
    if (bitsPerPixel < 3 ||
        bitsPerPixel > 24 ||
        bitsPerPixel % 3 > 0)
    {
        throw runtime_error("Invalid value for parameter bitsPerPixel. Must be a value between 3 and 24 and multiple of 3. Aborting embed operation.");
    }

    m_bitsPerPixel = bitsPerPixel;

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
    // for performance reasons, we read the input in 1K chunks instead of one byte at a time
    vector<char> buffer(1024);
    auto inputStreamExhausted = false;
    m_currentPixelIterator = sourceBitmap.begin();
    
    m_pixelBitEncodingPos = 0; // index to the bit (0 to 7) where data will be stored in the current pixel.
    m_currentPixelColor = PixelColor::R;
    m_pPixel = &m_currentPixelIterator->r;

    while(!inputStreamExhausted  &&
          m_currentPixelIterator != sourceBitmap.end())
    {
        sourceDataFileStream.read(buffer.data(), buffer.size());
        auto bytesRead = sourceDataFileStream.gcount();

        if (bytesRead > 0)
        {
            for (streamsize i = 0; i < bytesRead; i++) 
            {
                encodeByte(buffer[i]);
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

void steganographyLib::Steganography::encodeByte(const char inputByte)
{
    const u_int8_t LSB_BYTE_MASK = 0x01;

    // loop with an index to the bit (0 to 7) that is being encoded from the data file byte
    // encoding from least significant bit to most significant bit
    for (int inputByteBitEncodingPos = 0; inputByteBitEncodingPos < 8; inputByteBitEncodingPos++)
    {
        // isolate the bit we are trying to encode on the LSB position of a byte
        uint8_t inputByteBit = LSB_BYTE_MASK & (inputByte >> inputByteBitEncodingPos);
      
        // encode the bit at the encoding position
        int8_t mask;
        if (inputByteBit)
        {
            // we encode a '1' bit by performing OR bitwise operation on the pixel byte
            mask = inputByteBit << m_pixelBitEncodingPos;
            *m_pPixel |= mask;
        }
        else
        {
            // we encode a '0' bit by performing AND bitwise operation on the complement of the mask
            mask = LSB_BYTE_MASK << m_pixelBitEncodingPos;
            *m_pPixel &= ~mask;
        }

        m_pixelBitEncodingPos++;

        // check to see if we need to encode data in the next byte of the bitmap
        if (m_pixelBitEncodingPos == m_bitsPerPixel)
        {
            nextDestinationByte();
        }        
    }
}

void steganographyLib::Steganography::nextDestinationByte()
{
    switch(m_currentPixelColor)
    {
        case PixelColor::R:
            m_currentPixelColor = PixelColor::G;
            m_pPixel = &m_currentPixelIterator->g;
            break;
        case PixelColor::G:
            m_currentPixelColor = PixelColor::B;
            m_pPixel = &m_currentPixelIterator->b;
            break;
        case PixelColor::B:
            m_currentPixelIterator++;
            m_pPixel = &m_currentPixelIterator->r;
            break;
        default:
            throw runtime_error("Unexpected pixel color during encode operation");
    }

    m_pixelBitEncodingPos = 0;
}
