/**
 *   libBMPP - bmp16.hpp
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

#ifndef BMP16_HPP
#define BMP16_HPP

#include <pixels/wordpixel.hpp>
#include <bmp.hpp>

namespace bmp
{

class Bmp16 : public Bmp {

public:
  Bmp16();
  Bmp16(std::vector<bmp::Pixel*> pixelArray, size_t width, size_t height);
  ~Bmp16();
  //En/Decoding
  bool decodeBmp(uint8_t* bmpData, size_t dataSize);
  uint8_t* encodeBmp(size_t* dataSize);
  //Image operations
  bool setPixelAt(int index, uint16_t value);
  //Getters
  bmp::WordPixel* getPixelAt(int index);

};

} // namespace bmp

#endif
