#pragma once

namespace steganographyLib
{
    class Steganography
    {
        public:
            Steganography(int x, int y);
            ~Steganography();
            void embedInformation();
            void extractInformation();
        private:
    };
}