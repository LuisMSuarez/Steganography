#pragma once

#include <vector>    // std::vector
#include <cstdint>   // std::int*_t
#include "bitmap.h"

namespace steganographyLib
{
    enum PixelColor 
    {
        R,
        G,
        B
    };

    class Steganography
    {
        public:
            Steganography() noexcept;
            ~Steganography() noexcept;
            
            /// @brief Embeds information to a bitmap.
            /// @param originalBitmapFilePath Path to original bitmap that will be used to embed information into its pixels. This bitmap will be opened in readonly mode.
            /// @param sourceDataFilePath Path to file that contains information that we wish to embed into bitmap.
            /// @param destinationBitmapDataFilePath Path to bitmap file that will be the result of embedding into the original bitmap file.
            /// @param bitsPerPixel Resolution that determines how many bits from each RGB pixel (24 bits) will encode source data.  Must be a multiple of 3 between 3 and 24. 
            void embed(const std::string &originalBitmapFilePath, const std::string &sourceDataFilePath, const std::string &destinationBitmapDataFilePath, u_int8_t bitsPerPixel);
            
            /// @brief Extracts information from a bitmap.
            /// @param sourceBitmapFilePath Path to file that contains information that we wish to extract from the bitmap.
            /// @param destinationDataFilePath Path to file that will be the result of extracting information from the original bitmap file.
            /// @param bitsPerPixel Resolution that determines how many bits from each RGB pixel (24 bits) encodes source data.  Must be a multiple of 3 between 3 and 24. 
            void extract(const std::string &sourceBitmapFilePath, const std::string &destinationDataFilePath, u_int8_t bitsPerPixel);
        private:
            void encodeByte(const char inputByte);
            uint8_t decodeByte();
            void nextBitmapByte();
            void setBitsPerPixel(int bitsPerPixel);
            
            // member variables
            u_int8_t m_bitsPerPixel;
            std::vector<bmp::Pixel>::iterator m_currentPixelIterator;
            int m_pixelBitEncodingPos;
            bmp::Bitmap m_sourceBitmap;
            PixelColor m_currentPixelColor;
            uint8_t* m_pPixel;          
    };
}