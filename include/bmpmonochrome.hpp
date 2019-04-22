/**
 *   libBMPP - bmpmonochrome.hpp
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

#include <pixels/bwpixel.hpp>
#include <bmp.hpp>

namespace bmp
{

class Bmpmonochrome : public Bmp {

public:
  Bmpmonochrome();
  Bmpmonochrome(std::vector<bmp::Pixel*> pixelArray, size_t width, size_t height);
  ~Bmpmonochrome();
  //En/Decoding
  bool decodeBmp(uint8_t* bmpData, size_t dataSize);
  uint8_t* encodeBmp(size_t* dataSize);
  //Image operations
  bool resizeArea(size_t width, size_t height, size_t xOffset = 0, size_t yOffset = 0);
  bool resizeImage(size_t width, size_t height);
  bool setPixelAt(int row, int column, uint8_t value);
  bmp::BWPixel* getPixelAt(int row, int column);

};

} // namespace bmp
