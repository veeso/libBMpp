/**
 *   libBMPP - bmpparser.cpp
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

#include <parser/bmpparser.hpp>

#include <bmpmonochrome.hpp>
#include <bmp8.hpp>
#include <bmp16.hpp>
#include <bmp24.hpp>
#include <bmp32.hpp>

#include <functional>

using namespace bmp;

/**
 * @function BmpParser
 * @description BmpParser class constructor
**/

BmpParser::BmpParser() {

}

/**
 * @function getBmp
 * @description decodes a bitmap and returns a pointer to the instance of its specialized type class
 * @param uint8_t*
 * @param size_t dataSize
 * @param size_t& bitsPerPixel
 * @returns Bmp*
**/

Bmp* BmpParser::getBmp(uint8_t* bmpData, size_t dataSize, size_t& bitsPerPixel) {
  Bmp* decBmp = new Bmp();
  if (!decBmp->decodeBmp(bmpData, dataSize)) {
    return nullptr;
  }
  //Get bits per pixel
  bitsPerPixel = decBmp->getBitsPerPixel();
  delete decBmp;
  switch (bitsPerPixel) {
    case 1: {
      Bmpmonochrome* outBmp = new Bmpmonochrome();
      outBmp->decodeBmp(bmpData, dataSize);
      return outBmp;
      break;
    }
    case 8: {
      Bmp8* outBmp = new Bmp8();
      outBmp->decodeBmp(bmpData, dataSize);
      return outBmp;
      break;
    }
    case 16: {
      Bmp16* outBmp = new Bmp16();
      outBmp->decodeBmp(bmpData, dataSize);
      return outBmp;
      break;
    }
    case 24: {
      Bmp24* outBmp = new Bmp24();
      outBmp->decodeBmp(bmpData, dataSize);
      return outBmp;
      break;
    }
    case 32: {
      Bmp32* outBmp = new Bmp32();
      outBmp->decodeBmp(bmpData, dataSize);
      return outBmp;
      break;
    }
    default:
      return nullptr;
  }
}
