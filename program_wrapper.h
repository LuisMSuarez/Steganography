#pragma once

namespace SteganographyLib
{
    const int SUCCESS = 0;
    const int ERROR_CODE_INVALID_ARGUMENTS = 1;
    const int ERROR_CODE_INVALID_OPERATION = 2;

    int mainWrapper(int argc, char* argv[]);
}
