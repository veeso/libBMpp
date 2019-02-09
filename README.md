# LibBMPP

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![HitCount](http://hits.dwyl.io/ChristianVisintin/libBMPP.svg)](http://hits.dwyl.io/ChristianVisintin/libBMPP) [![Stars](https://img.shields.io/github/stars/ChristianVisintin/libBMPP.svg)](https://github.com/ChristianVisintin/libBMPP) [![Issues](https://img.shields.io/github/issues/ChristianVisintin/libBMPP.svg)](https://github.com/ChristianVisintin/libBMPP) [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/ChristianVisintin/libBMPP/issues) [![MemoryLeaks](https://img.shields.io/badge/Memory%20Leaks-None-brightgreen.svg)](https://img.shields.io/badge/Memory%20Leaks-None-brightgreen.svg)

Lib **Bitmap Plus Plus**  
Developed by *Christian Visintin*

Version 0.0.1 (Under development)

---

## What is libBMPP

libBMPP is a library for working with Bitmaps in modern C++, which allows you to manipulate bitmaps images.

---

## Build

libBMPP build is achievable through autotools, issuing the following commands:

```sh
./autogen.sh
./configure
make
make install
```

---

## Documentation

### Header

```cpp
typedef struct Header {
  uint16_t bmpId;
  uint32_t fileSize;
  uint32_t unused = 0;
  uint32_t dataOffset = 54;
  //DIB
  uint32_t dibSize = 40;
  uint32_t width;
  uint32_t height;
  uint16_t colorPlanes = 1;
  uint16_t bitsPerPixel;
  uint32_t biRgb = 0;
  uint32_t dataSize;
  uint32_t printSizeW;
  uint32_t printSizeH;
  uint32_t paletteSize;
  uint32_t importantColors;
} Header;
```

Header is the bitmap header storage structure and contains all the data which are usually stored into the bitmap header.

### Bmp

Bmp class is the main class of libBMPP. It is the superclass of every kind of Bitmap and takes care of handling pixels independently from the bits per pixel which is currently used. Bmp class also encodes and decodes the header and prepare the buffer for the raster data.
Every kind of Bmp (Bmp24 etc) must inherits from Bmp class.

#### header

```cpp
bmp::Header* header;
```

header represents the bitmap header, which is stored into a Header structure.

#### Constructor

```cpp
Bmp();
```

Bmp constructor initializes header to nullptr

#### Destructor

```cpp
~Bmp();
```

Bmp destructor deletes header structure.

#### decodeBmp

```cpp
bool decodeBmp(uint8_t* bmpData, size_t dataSize);
```

Given the buffer read from bitmap file, this method decodes the bitmap header and store its attributes in the struct. The raster data parsing is not provided by this method and must be implemented by the classes which extends Bmp.

If was unable to parse the bmpData or was invalid returns false.

#### encodeBmp

```cpp
uint8_t* encodeBmp(size_t* dataSize);
```

Starting from the header struct, this method creates the image buffer and returns a pointer to the buffer allocated. This method takes care only of allocating enough space to put the raster data and filling the first 54 bytes with the header. The encoding of the raster data is a task for the class which extends Bmp.

If was unable to allocate the buffer, returns nullptr

#### rotate

```cpp
bool rotate(int degrees);
```

Rotates the BMP image of 'degrees', which must be multiple of 90 (90/180/270/360).

If was unable to rotate the image, returns false

#### flipVertical

```cpp
bool flipVertical();
```

Flip the image vertically

#### flipHorizontal

```cpp
bool flipHorizontal();
```

Flip the image vertically

#### resizeArea

Resize the image area, an offset can be specified. The image can't become bigger than it was

```cpp
bool resizeArea(size_t width, size_t height, size_t xOffset = 0, size_t yOffset = 0);
```

#### getWidth

```cpp
size_t getWidth();
```

Returns the image width in pixels

#### getHeight

```cpp
size_t getHeight();
```

Returns the image height in pixels

### Bmp24

Bmp24 is a class which extends Bmp class and describes a 24 bits for pixel Bitmap.

In addition to Bmp methods, it provides the following methods:

#### setPixelAt

```cpp
bool setPixelAt(int index, uint8_t red, uint8_t green, uint8_t blue);
```

set the RGB value of the pixel in index position.  

Returns false if the requested pixel, does not exist.

#### toGreyScale

```cpp
bool toGreyScale(int greyLevels = 255);
```

Converts a color image to greyScale. Is it possible to specify the amount of grey levels (2 - 255).

#### getPixelAt

```cpp
bmp::RGBPixel* getPixelAt(int index);
```

Returns the pointer to the RGBPixel in provided position. If the requested pixel does not exist, returns nullptr

---

## Changelog

TODO

---

## License

MIT License  

Copyright (c) 2019 Christian Visintin  

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
