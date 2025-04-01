#include <gtest/gtest.h>
#include "../program_wrapper.h"

using namespace SteganographyLib;

TEST(ProgramTests, Embed)
{
    char* argv[] = {(char*)"steganography", (char*)"embed", (char*)"../../../data/sample.bmp", (char*)"../../../data/sampleInput.txt", (char*)"ProgramTests_EmbedTestsBitmap_6bits.bmp", (char*)"6"};
    EXPECT_EQ(SteganographyLib::SUCCESS, SteganographyLib::mainWrapper(6, argv));
}

TEST(ProgramTests, EmbedInvalidArguments)
{
    char* argv[] = {(char*)"steganography", (char*)"embed", (char*)"../../../data/sample.bmp", (char*)"../../../data/sampleInput.txt", (char*)"ProgramTests_EmbedTestsBitmap_6bits.bmp", (char*)"6"};
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(2, argv));
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(3, argv));
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(4, argv));
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(5, argv));
}

TEST(ProgramTests, Extract)
{
    char* argv[] = {(char*)"steganography", (char*)"extract", (char*)"../../../data/embedded_6bits.bmp", (char*)"output_data.txt", (char*)"6"};
    EXPECT_EQ(SteganographyLib::SUCCESS, SteganographyLib::mainWrapper(5, argv));
}

TEST(ProgramTests, ExtractInvalidArguments)
{
    char* argv[] = {(char*)"steganography", (char*)"extract", (char*)"../../../data/embedded_6bits.bmp", (char*)"output_data.txt", (char*)"6"};
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(2, argv));
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(3, argv));
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(4, argv));
}

TEST(ProgramTests, InvalidArguments)
{
    char* argv[] = {(char*)"steganography"};
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(1, argv));
}

TEST(ProgramTests, InvalidOperation)
{
    char* argv[] = {(char*)"steganography", (char*)"foo"};
    ASSERT_EQ(SteganographyLib::ERROR_CODE_INVALID_OPERATION, SteganographyLib::mainWrapper(2, argv));
}
