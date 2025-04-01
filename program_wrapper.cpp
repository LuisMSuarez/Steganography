#include "program_wrapper.h"
#include <iostream>
#include "steganography.h"

using namespace std;

// Examples of callbacks we can plug in to display progress of the encoding/extracting operations
// this demonstrates good decoupling between the steganography core library and the presentation layer.
// In a UX application, a progress bar can be plugged in using this same method.
void asteriskProgressCallback(int progressPercentage)
{
    cout << "*";
}

void percentageProgressCallback(int progressPercentage)
{
    cout << "Percent complete: " << progressPercentage << "\n";
}

int SteganographyLib::mainWrapper(int argc, char* argv[])
{
    const string usage = "steganography embed bitmapPath sourceData destinationBitmap bitsPerPixel |\nsteganography extract bitmapPath destinationFile bitsPerPixel";

    auto returnCode = SteganographyLib::SUCCESS;

    // Apply dependency inversion principle by taking dependency on abstractions, not concretions.
    SteganographyLib::ISteganography *steg = new SteganographyLib::Steganography();

    steg->registerProgressCallback(percentageProgressCallback, /* percentGrain */ 10);

    // Command line parsing
    if (argc < 2)
    {
        cout << "Invalid argument count\n" << usage;
        returnCode = ERROR_CODE_INVALID_ARGUMENTS;
    }
    else if (string(argv[1]).compare("embed") == 0)
    {
        if (argc != 6)
        {
            cout << "Invalid argument count for embed operation\n" << usage;
            returnCode = ERROR_CODE_INVALID_ARGUMENTS;
        }
        else
        {
            int bitsPerPixel = strtol(argv[5], NULL, 10);
            steg->embed(argv[2],argv[3], argv[4], bitsPerPixel);
        }
    }
    else if (string(argv[1]).compare("extract") == 0)
    {
        if (argc != 5)
        {
            cout << "Invalid argument count for extract operation\n" << usage;
            returnCode = ERROR_CODE_INVALID_ARGUMENTS;
        }
        else
        {
            int bitsPerPixel = strtol(argv[4], NULL, 10);
            steg->extract(argv[2],argv[3], bitsPerPixel);
        }
    }
    else
    {
        cout << "Invalid operation'" << argv[1] << "'.\n" << usage;
        returnCode = ERROR_CODE_INVALID_OPERATION;
    }

    if (steg != nullptr)
    {
        delete steg;
    }

    return returnCode;
}
