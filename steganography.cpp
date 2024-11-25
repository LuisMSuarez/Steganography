#include <iostream>  // std::cout
#include <fstream>   // std::*fstream
#include "steganography.h"

#define FILE_CHUNK_SIZE 1024
#define LSB_BYTE_MASK 0x01

using namespace std;
using namespace bmp;

steganographyLib::Steganography::Steganography() noexcept
{
    std::cout << "Steganography constructor invoked";
}

steganographyLib::Steganography::~Steganography() noexcept
{
    std::cout << "Steganography destructor invoked";
}

void steganographyLib::Steganography::embed(const std::string &originalBitmapFilePath, const std::string &sourceDataFilePath, const std::string &destinationBitmapDataFilePath, u_int8_t bitsPerPixel)
{
    setBitsPerPixel(bitsPerPixel);

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

    try
    {
        m_sourceBitmap.load(originalBitmapFilePath);
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
    // for performance reasons, we read the input in chunks instead of one byte at a time
    vector<char> buffer(FILE_CHUNK_SIZE);
    auto inputStreamExhausted = false;
    m_currentPixelIterator = m_sourceBitmap.begin();
    
    m_pixelBitEncodingPos = 0; // index to the bit (0 to 7) where data will be stored in the current pixel.
    m_currentPixelColor = PixelColor::R;
    m_pPixel = &m_currentPixelIterator->r;

    while(!inputStreamExhausted  &&
          m_currentPixelIterator != m_sourceBitmap.end())
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
    m_sourceBitmap.save(destinationBitmapDataFilePath);    
}

void steganographyLib::Steganography::extract(const std::string &sourceBitmapFilePath, const std::string &destinationDataFilePath, u_int8_t bitsPerPixel)
{
    setBitsPerPixel(bitsPerPixel);

    try
    {
        m_sourceBitmap.load(sourceBitmapFilePath);
    }
    catch(const bmp::Exception& e)
    {
        // Repackage exception from underlying library for uniformity.
        throw runtime_error("Could not open original bitmap file at "
            + sourceBitmapFilePath
            + " aborting extract operation."
            + e.what());
    }

    // Open and verify destinationDataFilePath
    auto destinationDataFileStream = ofstream(destinationDataFilePath, ios::binary);
    if (!destinationDataFileStream || 
        !destinationDataFileStream.is_open() ||
        destinationDataFileStream.bad())
    {
        throw runtime_error("Could not open destination data file at " 
            + destinationDataFilePath
            + " aborting extract operation.");
    }

    // perform extract operation
    // for performance reasons, we write the output in chunks instead of one byte at a time
    vector<char> buffer(FILE_CHUNK_SIZE);
    m_currentPixelIterator = m_sourceBitmap.begin();    
    m_pixelBitEncodingPos = 0; // index to the bit (0 to 7) where data is encoded in the current pixel.
    m_currentPixelColor = PixelColor::R;
    m_pPixel = &m_currentPixelIterator->r;
    int vectorPos = 0;

    while (m_currentPixelIterator != m_sourceBitmap.end())
    {
        auto dataByte = decodeByte();
        buffer[vectorPos++] = dataByte;

        if (vectorPos == FILE_CHUNK_SIZE)
        {
            destinationDataFileStream.write(buffer.data(), buffer.size());
            vectorPos = 0;
        }
    }

    // need to flush the data vector
    if (vectorPos > 0)
    {
        destinationDataFileStream.write(buffer.data(), vectorPos);
    }

    destinationDataFileStream.close();
}

void steganographyLib::Steganography::encodeByte(const char inputByte)
{
    // loop with an index to the bit (0 to 7) that is being encoded from the data file byte
    // encoding from least significant bit to most significant bit
    for (int inputByteBitEncodingPos = 0; inputByteBitEncodingPos < 8; inputByteBitEncodingPos++)
    {
        // isolate the bit we are trying to encode on the LSB position of a byte
        uint8_t inputByteBit = LSB_BYTE_MASK & (inputByte >> inputByteBitEncodingPos);
      
        // encode the bit at the encoding position
        // we encode bits starting at the least significant bit positions to ensure we shift
        // the color of the bit as little as possible.
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
            nextBitmapByte();
        }        
    }
}

uint8_t steganographyLib::Steganography::decodeByte()
{
    uint8_t dataByte = 0x00;

    for (int inputByteBitEncodingPos = 0; inputByteBitEncodingPos < 8; inputByteBitEncodingPos++)
    {
        uint8_t mask = LSB_BYTE_MASK << inputByteBitEncodingPos;
        if (*m_pPixel & mask)
        {
            dataByte |= LSB_BYTE_MASK << m_pixelBitEncodingPos;
        }
        m_pixelBitEncodingPos++;

        if (m_pixelBitEncodingPos == m_bitsPerPixel)
        {
            nextBitmapByte();
        }
    }

    return dataByte;
}

void steganographyLib::Steganography::nextBitmapByte()
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

            if ( m_currentPixelIterator == m_sourceBitmap.end())
            {
                throw runtime_error("end of source bitmap reached");
            }
            
            m_pPixel = &m_currentPixelIterator->r;
            break;
        default:
            throw runtime_error("Unexpected pixel color during encode/decode operation");
    }

    m_pixelBitEncodingPos = 0;
}

void steganographyLib::Steganography::setBitsPerPixel(int bitsPerPixel)
{
    // Before manipulating files, verify that bitsPerPixel is a number between 3 and 24 and a multiple of 3.
    // this number represents how many bits of information from the input file we will pack into each 24-bit RGB pixel.
    if (bitsPerPixel < 3 ||
        bitsPerPixel > 24 ||
        bitsPerPixel % 3 > 0)
    {
        throw runtime_error("Invalid value for parameter bitsPerPixel. Must be a value between 3 and 24 and multiple of 3. Aborting operation.");
    }

    m_bitsPerPixel = bitsPerPixel;
}