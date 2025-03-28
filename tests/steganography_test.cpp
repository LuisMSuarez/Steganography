#include <gtest/gtest.h>
#include <filesystem>
#include "../steganography.h"

using namespace steganographyLib;

TEST(SteganographyTest, EmbedValidText) {
    Steganography steg;
    std::string sourceDataFilePath = "../../data/sampleInput.txt";
    std::string originalBitmapFilePath = "../../data/sample.bmp";
    std::string destinationDataFilePath = "../../data/destinationBitmapTest.bmp";
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

TEST(SteganographyTest, ExtractValidBitmap) {
    Steganography steg;
    std::string sourceBitmapFilePath = "test_data/valid_bitmap.bmp";
    std::string destinationDataFilePath = "test_data/output_data.txt";
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

TEST(SteganographyTest, ExtractInvalidBitmapPath) {
    Steganography steg;
    std::string sourceBitmapFilePath = "test_data/nonexistent_bitmap.bmp";
    std::string destinationDataFilePath = "test_data/output_data.txt";
    uint8_t bitsPerPixel = 6;

    // Call the extract method and expect an exception
    EXPECT_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, bitsPerPixel), std::runtime_error);
}

TEST(SteganographyTest, ExtractInvalidBitsPerPixel) {
    Steganography steg;
    std::string sourceBitmapFilePath = "test_data/valid_bitmap.bmp";
    std::string destinationDataFilePath = "test_data/output_data.txt";
    uint8_t invalidBitsPerPixel = 5; // Not a multiple of 3

    // Ensure the source bitmap exists
    ASSERT_TRUE(std::filesystem::exists(sourceBitmapFilePath));

    // Call the extract method and expect an exception
    EXPECT_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, invalidBitsPerPixel), std::runtime_error);
}

TEST(SteganographyTest, ExtractCorruptedBitmap) {
    Steganography steg;
    std::string sourceBitmapFilePath = "test_data/corrupted_bitmap.bmp";
    std::string destinationDataFilePath = "test_data/output_data.txt";
    uint8_t bitsPerPixel = 6;

    // Ensure the corrupted bitmap exists
    ASSERT_TRUE(std::filesystem::exists(sourceBitmapFilePath));

    // Call the extract method and expect an exception
    EXPECT_THROW(steg.extract(sourceBitmapFilePath, destinationDataFilePath, bitsPerPixel), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}