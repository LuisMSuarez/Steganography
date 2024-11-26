Steganography is the art of hiding or encoding a message within another message, image or file.
A bitmap (BMP) image is a perfect vessel for encoding another message.  A bitmap is essentially a matrix of pixels, where each pixel is made up of Red, Green and Blue 8-bit values.

Examples:

| Color    | R | G | B |
| -------- | ------- | ------- |------- |
| White  | 255 | 255 | 255 |
| Black | 0 | 0 | 0 |
| Blue | 0 | 0 | 255 | 
| Pink | 255 | 192 | 203 |
| Turquoise | 64 | 224 | 208 |

For this project, I leveraged the [Microsoft BMP file format](https://en.wikipedia.org/wiki/BMP_file_format) which is an uncompressed raster image.
I also wanted to code this project using C++ in a Linux environment.  For my setup, I used [OpenSuse Tumbleweed](https://get.opensuse.org/tumbleweed/) and [VSCode IDE](https://code.visualstudio.com/), with the g++ compiler.
I used the [BitmapPlusPlus](https://github.com/baderouaich/BitmapPlusPlus) library (MIT license), which is conveniently packaged as a single C++ header file, which is easy to use in your own project.

The RGB pixels in a bitmap are a convenient container for embedding other content (text or any binary file).  Each pixel is a 24-bit value.  The caller can decide the "bitsPerPixel" or encoding density,
which determines how many bits from the 24 bit pixel will be used to embed content.  Selecting a small value, such as 3, means that only the least significant bit from each RGB component
will be shifted to store content, where the human eye cannot percieve any difference from the original image.  The theoretical limit for encoding density is 24, which means that
content will be encoded across the entire 24 bits, however this will produce perceptible changes.

Putting everything together, the way secret message transmission could work would be:
1. Both the sender and receiver to have a copy of the program.
2. The sender picks a bitmap of their choice.
3. The sender encodes the message in the bitmap.
4. The sender transmits the bitmap to the receiver, or alternatively use a passive method to transmit the file such as publish it on a website.
5. The receiver decodes the message from the bitmap.  All they need is the bitmap itself and knowledge of the packing density that was used (between 3 and 24).

Of course, this method could be discovered by a middle-man.  It is not intended to replace file encryption, but can definitely be used to transmit an encrypted message that has been encoded into an image.

Below is an example showing how a single letter could be encoded to the first 3 pixels of a bitmap

Orignal bitmap (showing bytes in both decimal and binary):

| Pixel    | R | G | B |
| -------- | ------- | ------- |------- |
| 0  | 64 (1000 0000) | 224 (1110 0000)| 208 (1101 0000|
| 1  | 245 (1111 0101)| 222 (1101 1110)| 179 (1011 0011)|
| 2  | 250 (1111 1010)| 128 (1000 0000)| 114 (1110 0010)|

The letter 'H' in ASCII has the following binary value: 0100 1000

Assuming we encode 3 bits into each RGB pixel, the resulting 'shifted' pixels would be as shown in the table below, where the convention is:
1. Input bits are encoded from LSB to MSB
2. Bits are encoded on the LSB of the pixels, to ensure minimal color 'shift'.

| Pixel    | R | G | B |
| -------- | ------- | ------- |------- |
| 0  | 64 (1000 0000) | 224 (1110 0000)| 208 (1101 0000|
| 1  | 245 (1111 0101)| 222 (1101 1110)| **178 (1011 0010)**|
| 2  | **251 (1111 1011)**| 128 (1000 0000)| 114 (1110 0010)|

Note how only 2 bytes actually had to be shifted by a single digit, 6 other bytes required no shift at all as the LSB matched the value of the pixel being encoded.
