#pragma once

#include <cstdint>    // std::int*_t
#include <functional> // std::function
#include <string>     // std::string

namespace steganographyLib
{
    /// @brief Callback function that will be invoked during the embed and extract methods to notify the caller of the progress of these operations.
    /// @param int The percentage of the operation that has been completed.
    typedef std::function<void(int)> ProgressCallback;

    /// @brief Interface for Steganography operations on a bitmap
    class ISteganography
    {
        public:
            /// @brief Destructor
            virtual ~ISteganography() = default;

            /// @brief Embeds information to a bitmap.
            /// @param originalBitmapFilePath Path to original bitmap that will be used to embed information into its pixels. This bitmap will be opened in readonly mode.
            /// @param sourceDataFilePath Path to file that contains information that we wish to embed into bitmap.
            /// @param destinationBitmapDataFilePath Path to bitmap file that will be the result of embedding into the original bitmap file.
            /// @param bitsPerPixel Resolution that determines how many bits from each RGB pixel (24 bits) will encode source data.  Must be a multiple of 3 between 3 and 24.
            virtual void embed(const std::string &originalBitmapFilePath, const std::string &sourceDataFilePath, const std::string &destinationBitmapDataFilePath, std::uint8_t bitsPerPixel) = 0;

            /// @brief Extracts information from a bitmap.
            /// @param sourceBitmapFilePath Path to file that contains information that we wish to extract from the bitmap.
            /// @param destinationDataFilePath Path to file that will be the result of extracting information from the original bitmap file.
            /// @param bitsPerPixel Resolution that determines how many bits from each RGB pixel (24 bits) encodes source data.  Must be a multiple of 3 between 3 and 24.
            virtual void extract(const std::string &sourceBitmapFilePath, const std::string &destinationDataFilePath, std::uint8_t bitsPerPixel) = 0;

            /// @brief Registers a callback function to be invoked during both the embed and extract methods.
            /// Allows the caller to be notified with the progress of these operations, such as for logging or to display a progress bar to the user.
            /// @param callbackFunction The callback function that will be invoked.
            /// @param percentGrain value between 1 to 100, indicating after how many percentage units of completed work (over a total of 100) will the callback be invoked.
            //  Example, if 1 is provided, 100 callbacks will be invoked.  If 50 is provided 2 callbacks will be invoked.
            virtual void registerProgressCallback(ProgressCallback callbackFunction, int percentGrain = 10) = 0;
    };
}
