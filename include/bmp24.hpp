/**
 *   libBMP24 - bmp24.hpp
 *   Developed by Christian Visintin
 * 
 * MIT License
 * Copyright (c) 2019 Christian Visintin
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#ifndef BMP24_HPP
#define BMP24_HPP

#include <pixels/rgbpixel.hpp>

#include <cstddef>
#include <vector>

#define BMP_ID 0x424D

namespace bmp {

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
  uint16_t bitsPerPixel = 24;
  uint32_t biRgb = 0;
  uint32_t dataSize;
  uint32_t printSizeW;
  uint32_t printSizeH;
  uint32_t paletteSize;
  uint32_t importantColors;
} Header;

class Bmp24 {

public:
  Bmp24();
  ~Bmp24();
  //En/Decoding
  bool decodeBmp(uint8_t* bmpData, size_t dataSize);
  uint8_t* encodeBmp(size_t* dataSize);
  //Image operations
  bool setPixelAt(int index, uint8_t red, uint8_t green, uint8_t blue);
  bool toGreyScale(int greyLevels = 255);
  bool rotate(int degrees);
  /*flip()
  */
  //Getters
  size_t getWidth();
  size_t getHeight();
  bmp::RGBPixel* getPixelAt(int index);

private:
  int roundToMultiple(int toRound, int multiple);
  Header* header;
  std::vector<bmp::RGBPixel*> pixelArray;

};

} // namespace bmp

#endif
