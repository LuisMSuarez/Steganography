#include <gtest/gtest.h>
#include "../program_wrapper.h"

using namespace SteganographyLib;

extern int SteganographyLib::mainWrapper(int argc, char* argv[]);

TEST(ProgramTests, EmbedTest)
{
    char* argv[] = {(char*)"steganography", (char*)"embed", (char*)"../../../data/sample.bmp", (char*)"../../../data/sampleInput.txt", (char*)"ProgramTests_EmbedTestsBitmap_6bits.bmp", (char*)"6"};
    EXPECT_EQ(SteganographyLib::SUCCESS, SteganographyLib::mainWrapper(6, argv));
}

TEST(ProgramTests, InvalidArgumentsTest)
{
    char* argv[] = {(char*)"steganography"};
    EXPECT_EQ(SteganographyLib::ERROR_CODE_INVALID_ARGUMENTS, SteganographyLib::mainWrapper(1, argv));
}

TEST(ProgramTests, InvalidOperationTest)
{
    char* argv[] = {(char*)"steganography", (char*)"foo", (char*)"../../../data/sample.bmp", (char*)"../../../data/sampleInput.txt", (char*)"ProgramTests_EmbedTestsBitmap_6bits.bmp", (char*)"6"};
    ASSERT_EQ(SteganographyLib::ERROR_CODE_INVALID_OPERATION, SteganographyLib::mainWrapper(6, argv));
}
