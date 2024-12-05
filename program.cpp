#include <iostream>
#include "steganography.h"

using namespace std;

 void progressCallback(int progressPercentage)
 {
    cout << "*";
 }
 
int main(int argc, char* argv[])
{
    const int errorCodeInvalidArguments = 1;
    steganographyLib::Steganography steg;
    steg.registerProgressCallback(progressCallback);
    string usage = "steganography embed bitmapPath sourceData destinationBitmap bitsPerPixel |\nsteganographyextract bitmapPath destinationFile bitsPerPixel";
        
    // Command line parsing
    if (argc < 2)
    {
        cout << "Invalid argument count\n" << usage;
        exit(errorCodeInvalidArguments);
    }
    if (string(argv[1]).compare("embed") == 0)
    {
        if (argc != 6)
        {
            cout << "Invalid argument count for embed operation\n" << usage;
            exit(errorCodeInvalidArguments);
        }
        int bitsPerPixel = strtol(argv[5], NULL, 10);
        steg.embed(argv[2],argv[3], argv[4], bitsPerPixel);
    }
    else if (string(argv[1]).compare("extract") == 0)
    {
        if (argc != 5)
        {
            cout << "Invalid argument count for extract operation\n" << usage;
            exit(errorCodeInvalidArguments);
        }
        int bitsPerPixel = strtol(argv[4], NULL, 10);
        steg.extract(argv[2],argv[3], bitsPerPixel);
    }
    else
    {
        cout << "Invalid operation'" << argv[1] << "'.\n" << usage;
        exit(1);
    }

    return 0;
}