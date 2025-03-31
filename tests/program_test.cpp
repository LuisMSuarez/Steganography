#include <gtest/gtest.h>
#include <filesystem>
#include "../program_wrapper.h"

using namespace SteganographyLib;

int add(int a, int b) {
    return a + b;
}

extern int SteganographyLib::mainWrapper(int argc, char* argv[]);

TEST(SteganographyTests, mnTest)
{
    std::string sourceDataFilePath = "../../../data/sampleInput.txt";
    char* argv[] = {(char*)"steganography", (char*)"invalidAction", (char*)"somefile.txt"};
    //ASSERT_EQ(SteganographyLib::ERROR_CODE_INVALID_OPERATION, add(2, 3)); // Test passes
    // only works if first arg is 0!!!
    ASSERT_EQ(SteganographyLib::ERROR_CODE_INVALID_OPERATION, SteganographyLib::mainWrapper(3, argv));
    //SteganographyLib::ERROR_CODE_INVALID_OPERATION
}
