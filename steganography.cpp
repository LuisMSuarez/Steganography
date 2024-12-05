#include <iostream>   // std::cout
#include <fstream>    // std::*fstream
#include <filesystem> // std::filesystem::file_size
#include "steganography.h"

#define FILE_CHUNK_SIZE 1024
#define LSB_BYTE_MASK 0x01

using namespace std;
using namespace bmp;

steganographyLib::Steganography::Steganography() noexcept
{
    m_progressCallback = nullptr;
}

steganographyLib::Steganography::~Steganography() noexcept
{
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

    auto rawSourceFileSize = filesystem::file_size(sourceDataFilePath);
    if (rawSourceFileSize > UINT16_MAX)
    {
        throw runtime_error("Source file size is too large");
    }
    
    uint16_t sourceFileSize = static_cast<uint16_t>(rawSourceFileSize); 

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

    // verify that the bitmap can fit in the encoded file with the provided
    // bits per pixel density
    auto maxFileSizeBytes = (m_sourceBitmap.height() * m_sourceBitmap.width() * m_bitsPerPixel) / 8 ;
    auto encodedFileSizeBytes = rawSourceFileSize + sizeof(sourceFileSize);
    if (encodedFileSizeBytes > maxFileSizeBytes)
    {
        throw runtime_error("Data file is too large to fit in the bitmap.  Use a larger bitmap or a higher packing density.");
    }

    // perform embed operation
    // for performance reasons, we read the input in chunks instead of one byte at a time
    vector<char> buffer(FILE_CHUNK_SIZE);
    auto inputStreamExhausted = false;
    m_currentPixelIterator = m_sourceBitmap.begin();
    
    m_pixelBitEncodingPos = 0; // index to the bit (0 to 7) where data will be stored in the current pixel.
    m_currentPixelColor = PixelColor::R;
    m_pPixel = &m_currentPixelIterator->r;

    // embed the source file size in the first 16 bits of encoded data, so that
    // the extract operation knows when to stop decoding bytes
    vector<char> sourceFileSyzeBytes(sizeof(sourceFileSize));
    std::memcpy(sourceFileSyzeBytes.data(), &sourceFileSize, sizeof(sourceFileSize));
    encodeByte(sourceFileSyzeBytes[0]); // 8 least significant bytes of the file size
    encodeByte(sourceFileSyzeBytes[1]); // 8 most significant bytes of the file size

    while(!inputStreamExhausted  &&
          m_currentPixelIterator != m_sourceBitmap.end())
    {
        sourceDataFileStream.read(buffer.data(), buffer.size());
        if(m_progressCallback != nullptr)
        {
            m_progressCallback(0);
        }
        auto bytesRead = sourceDataFileStream.gcount();

        if (bytesRead > 0)
        {
            for (streamsize i = 0; i < bytesRead; i++) 
            {
                encodeByte(buffer[i]);
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

    // the first 16 bits of encoded data indicate the number of data bytes encoded in the file
    // so that the extract operation knows when to stop decoding bytes
    uint16_t dataFileSize;
    vector<char> sourceFileSyzeBytes(sizeof(dataFileSize));
    sourceFileSyzeBytes[0] = decodeByte(); // 8 least significant bytes of the file size
    sourceFileSyzeBytes[1] = decodeByte(); // 8 most significant bytes of the file size
    std::memcpy(&dataFileSize, sourceFileSyzeBytes.data(), sizeof(dataFileSize));

    // verify that the bitmap can hold at least 'dataFileSize' bytes, based on the number of pixels
    // in the image and the value provided for 'bitsPerPixel'
    // this protects against cases where the bitmap that has been provided is not a valid encoded file
    auto maxEncodedBytes = (m_sourceBitmap.width() * m_sourceBitmap.height() * bitsPerPixel)/8 - sizeof(dataFileSize);
    if (dataFileSize > maxEncodedBytes)
    {
        throw runtime_error("Could not decode bitmap at " 
            + sourceBitmapFilePath
            + " the bitmap may not be a valid encoded file or try selecting a higher value for 'bitsPerPixel'.");
    }

    while (dataFileSize &&
        m_currentPixelIterator != m_sourceBitmap.end())
    {
        auto dataByte = decodeByte();
        buffer[vectorPos++] = dataByte;
        dataFileSize--;

        if (vectorPos == FILE_CHUNK_SIZE)
        {
            destinationDataFileStream.write(buffer.data(), buffer.size());
            vectorPos = 0;
        }
    }

    // need to flush the data vector once more, as it may be partially filled.
    if (vectorPos > 0)
    {
        destinationDataFileStream.write(buffer.data(), vectorPos);
    }

    destinationDataFileStream.close();
}

void steganographyLib::Steganography::registerProgressCallback(ProgressCallback callbackFunction, int percentGrain) noexcept
{
    m_progressCallback = callbackFunction;
    m_progressCallbackPercentGrain = percentGrain;
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

    for (int dataByteBitPos = 0; dataByteBitPos < 8; dataByteBitPos++)
    {
        uint8_t mask = LSB_BYTE_MASK << m_pixelBitEncodingPos;
        if (*m_pPixel & mask)
        {
            dataByte |= LSB_BYTE_MASK << dataByteBitPos;
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

            if (m_currentPixelIterator == m_sourceBitmap.end())
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