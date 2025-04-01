#include <gtest/gtest.h>
#include <filesystem>
#include "../steganography.h"
#include "../program_wrapper.h"

using namespace SteganographyLib;

TEST(SteganographyTests, EmbedValidText_NoCallback) {
    Steganography steg;
    std::string sourceDataFilePath = "../../../data/sampleInput.txt";
    std::string originalBitmapFilePath = "../../../data/sample.bmp";
    std::string destinationDataFilePath = "embeddedBitmap_6bits.bmp";
    uint8_t bitsPerPixel = 6;

    // Ensure the source bitmap exists
    ASSERT_TRUE(std::filesystem::exists(originalBitmapFilePath));

    // Ensure the source data exists
    ASSERT_TRUE(std::filesystem::exists(sourceDataFilePath));

    // Call the extract method
    EXPECT_NO_THROW(steg.embed(originalBitmapFilePath, sourceDataFilePath, destinationDataFilePath, bitsPerPixel));

    // Verify the output file is created
    EXPECT_TRUE(std::filesystem::exists(destinationDataFilePath));

    // Clean up
    std::filesystem::remove(destinationDataFilePath);
}

static bool embedValidText_Callback_invoked = false;
void EmbedValidText_Callback_testCallback(int progressPercentage)
{
   embedValidText_Callback_invoked = true;
}

TEST(SteganographyTests, EmbedValidText_Callback) {
    Steganography steg;
    std::string sourceDataFilePath = "../../../data/sampleInput.txt";
    std::string originalBitmapFilePath = "../../../data/sample.bmp";
    std::string destinationDataFilePath = "embeddedBitmap_6bits.bmp";
    uint8_t bitsPerPixel = 6;

    // Ensure the source bitmap exists
    ASSERT_TRUE(std::filesystem::exists(originalBitmapFilePath));

    // Ensure the source data exists
    ASSERT_TRUE(std::filesystem::exists(sourceDataFilePath));

    // Register the callback
    steg.registerProgressCallback(EmbedValidText_Callback_testCallback, 10);

    // Call the extract method
    EXPECT_NO_THROW(steg.embed(originalBitmapFilePath, sourceDataFilePath, destinationDataFilePath, bitsPerPixel));

    // Verify the output file is created
    EXPECT_TRUE(std::filesystem::exists(destinationDataFilePath));

    // Verify that the callback was invoked
    EXPECT_TRUE(embedValidText_Callback_invoked);

    // Clean up
    std::filesystem::remove(destinationDataFilePath);
}

TEST(SteganographyTests, ExtractValidBitmap_NoCallback) {
    Steganography steg;
    std::string sourceBitmapFilePath = "../../../data/embedded_6bits.bmp";
    std::string destinationDataFilePath = "output_data.txt";
    uint8_t bitsPerPixel = 6;

    // Ensure the source bitmap exists
    ASSERT_TRUE(std::filesystem::exists(sourceBitmapFilePath));

    // Call the extract method
    EXPECT_NO_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, bitsPerPixel));

    // Verify the output file is created
    EXPECT_TRUE(std::filesystem::exists(destinationDataFilePath));

    // Clean up
    std::filesystem::remove(destinationDataFilePath);
}

static bool extractValidBitmap_Callback_invoked = false;
void ExtractValidBitmap_Callback_testCallback(int progressPercentage)
{
   extractValidBitmap_Callback_invoked = true;
}

TEST(SteganographyTests, ExtractValidBitmap_Callback) {
    Steganography steg;
    std::string sourceBitmapFilePath = "../../../data/embedded_6bits.bmp";
    std::string destinationDataFilePath = "output_data.txt";
    uint8_t bitsPerPixel = 6;

    // Ensure the source bitmap exists
    ASSERT_TRUE(std::filesystem::exists(sourceBitmapFilePath));

    // Register the callback
    steg.registerProgressCallback(ExtractValidBitmap_Callback_testCallback, 10);

    // Call the extract method
    EXPECT_NO_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, bitsPerPixel));

    // Verify the output file is created
    EXPECT_TRUE(std::filesystem::exists(destinationDataFilePath));

    // Verify that the callback was invoked
    EXPECT_TRUE(embedValidText_Callback_invoked);

    // Clean up
    std::filesystem::remove(destinationDataFilePath);
}

TEST(SteganographyTests, ExtractInvalidBitmapPath) {
    Steganography steg;
    std::string sourceBitmapFilePath = "nonexistent_bitmap.bmp";
    std::string destinationDataFilePath = "output_data.txt";
    uint8_t bitsPerPixel = 6;

    // Call the extract method and expect an exception
    EXPECT_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, bitsPerPixel), std::runtime_error);
}

TEST(SteganographyTests, ExtractInvalidBitsPerPixel) {
    Steganography steg;
    std::string sourceBitmapFilePath = "../../../data/embedded_6bits.bmp";
    std::string destinationDataFilePath = "output_data.txt";
    uint8_t invalidBitsPerPixel = 5; // Not a multiple of 3

    // Ensure the source bitmap exists
    ASSERT_TRUE(std::filesystem::exists(sourceBitmapFilePath));

    // Call the extract method and expect an exception
    EXPECT_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, invalidBitsPerPixel), std::runtime_error);
}
