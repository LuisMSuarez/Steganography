Steganography is the art of hiding or encoding a message within another message, image or file.
A bitmap (BMP) image is a perfect vessel for encoding another message.  A 24-bit bitmap is essentially a matrix of pixels, where each pixel is made up of Red, Green and Blue 8-bit values.

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
For reading/writing the bitmap, I used the [BitmapPlusPlus](https://github.com/baderouaich/BitmapPlusPlus) library (MIT license), which is conveniently packaged as a single C++ header file, which makes it easy to use in your own project.  This served two purposes: allow me to focus on my core logic and practice usage of open source libraries.

One of my key goals in this project was to have clean separation of concerns between the presentation layer (command-line interface via main() entry point) and the core library for encoding/decoding content.  I'm particularly pleased with the 'callback' pattern I introduced to allow the caller to subscribe to 'progress' notifications from the core library, to display progress to the user in any fashion, such as via command-line output or progress bar in UX applications, where the core library and the presentation layer remain cleanly decoupled, with no dependency on the inner workings of eachother.

The RGB pixels in a bitmap are a convenient container for embedding other content (text or any binary file).  Each pixel is a 24-bit value.  Note: there are also 32-bit pixels where an additional A or Alpha value is provided for transparency.  For this project I focused on 24-bit bitmaps, but the code can easily be extended to support 32-bit pixels.  The caller can decide the "bitsPerPixel" or encoding density,
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

Original bitmap (showing bytes in both decimal and binary):

| Pixel    | R | G | B |
| -------- | ------- | ------- |------- |
| 0  | 64 (1000 0000) | 224 (1110 0000)| 208 (1101 0000|
| 1  | 245 (1111 0101)| 222 (1101 1110)| 179 (1011 0011)|
| 2  | 250 (1111 1010)| 128 (1000 0000)| 114 (1110 0010)|

Suppose we want to encode the message "Hello world" into the above bitmap.
The first letter/byte in the message 'H' in ASCII has the following binary value: 0100 1000

Assuming we encode 3 bits into each RGB pixel, the resulting 'shifted' pixels would be as shown in the table below, where the convention is:
1. Input bits are encoded from LSB to MSB
2. Bits are encoded on the LSB of the pixels, to ensure minimal color 'shift'.

| Pixel    | R | G | B |
| -------- | ------- | ------- |------- |
| 0  | 64 (1000 0000) | 224 (1110 0000)| 208 (1101 0000|
| 1  | 245 (1111 0101)| 222 (1101 1110)| **178 (1011 0010)**|
| 2  | **251 (1111 1011)**| 128 (1000 0000)| 114 (1110 0010)|

Note how only 2 bytes (in bold) actually had to be shifted by a single digit, 6 other bytes required no shift at all as the LSB matched the value of the pixel being encoded.
The above process would then be repeated to encode a complete message.  Note that this not only works for text files in ASCII format, but for any binary file, as the encoder just reads the input file one byte at a time, without making any further assumptions

Below you can compare the original bitmap on the left, and the resulting bitmap on the right, where I encoded 2.3KB of "Lorem Ipsum" text. Using 6 bits per pixel of encoding density.
Note: markdown files on GitHub don't support uploading bitmaps, so I had to repackage them in the PNG format, which is also lossless.
The human eye cannot percieve any change between the two.

![sample](https://github.com/user-attachments/assets/b608b7dd-5fc1-4bb2-8106-50b8d778939a) ![destinationBitmap](https://github.com/user-attachments/assets/46572b13-e549-4c60-845d-3f65d2b5217e)

