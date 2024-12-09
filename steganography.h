#pragma once

#include <vector>     // std::vector
#include <cstdint>    // std::int*_t
#include <functional> // std::function
#include "bitmap.h"

namespace steganographyLib
{
    enum PixelColor
    {
        R,
        G,
        B
    };

    typedef std::function<void(int)> ProgressCallback;

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

            /// @brief Registers a callback function to be invoked during both the embed and extract methods.
            /// Allows the caller to be notified with the progress of these operations, such as for logging or to display a progress bar to the user.
            /// @param callbackFunction The callback function that will be invoked.
            /// @param percentGrain value between 1 to 100, indicating after how many percentage units of completed work (over a total of 100) will the callback be invoked.
            //  Example, if 1 is provided, 100 callbacks will be invoked.  If 50 is provided 2 callbacks will be invoked.
            void registerProgressCallback(ProgressCallback callbackFunction, int percentGrain = 10);
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
            ProgressCallback m_progressCallback;
            int m_progressCallbackPercentGrain;
    };
}