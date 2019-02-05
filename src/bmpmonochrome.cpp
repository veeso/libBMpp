/**
 *   libBMP24 - bmpmonochrome.hpp
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

#include <bmpmonochrome.hpp>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

using namespace bmp;

/**
 * @function Bmpmonochrome
 * @description Bmpmonochrome class constructor
**/

Bmpmonochrome::Bmpmonochrome() : Bmp() {
  pixelArray.clear();
}

/**
 * @function ~Bmpmonochrome
 * @description Bmpmonochrome class destructor
**/

Bmpmonochrome::~Bmpmonochrome() {
  //Delete pixels
  for (auto& pixel : pixelArray) {
    BWPixel* bwPixel = reinterpret_cast<BWPixel*>(pixel);
    delete bwPixel;
    pixel = nullptr;
  }
  pixelArray.clear();
}

/**
 * @function decodeBmp
 * @description decode Bmp data buffer converting it to header struct and RGBPixel array
 * @param uint8_t*
 * @param size_t
 * @returns bool
**/

bool Bmpmonochrome::decodeBmp(uint8_t* bmpData, size_t dataSize) {

  //Call superclass decodeBmp to decode header
  if (!Bmp::decodeBmp(bmpData, dataSize)) {
    return false;
  }
  //Get data
  size_t totalArea = header->width * header->height;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < header->fileSize - 1; dataPtr++) {
    uint8_t currentByte = bmpData[dataPtr];
    for (size_t i = 0; pixelArray.size() <= totalArea && i < 8; i++) {
      //Iterate over bits in byte to get color
      uint8_t shiftValue = 8 - i;
      uint8_t colorValue = (currentByte >> shiftValue) & 1;
      pixelArray.push_back(new BWPixel(colorValue));
    }
  }
  return true;

}

/**
 * @function encodeBmp
 * @description: encodes bitmap to buffer
 * @param size_t*
 * @returns uint8_t*
**/

uint8_t* Bmpmonochrome::encodeBmp(size_t* dataSize) {
  //Fill header and get bmpData with fixed size
  uint8_t* bmpData = Bmp::encodeBmp(dataSize);
  //Return nullptr if needed
  if (bmpData == nullptr) {
    return nullptr;
  }
  size_t byteCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < *dataSize - 1;) {
    //Get byte to write
    uint8_t colorValue = 0;
    for (size_t i = 0; i < 8; i++, byteCounter++) {
      BWPixel* currPixel = reinterpret_cast<BWPixel*>(pixelArray.at(byteCounter));
      if (currPixel == nullptr) {
        break;
      }
      colorValue = colorValue << (8 - i);
    }
    bmpData[dataPtr] = colorValue;
  }
  return bmpData;
}

/**
 * @function setPixelAt
 * @description: replace pixel in a certain position with the provided one
 * @param int
 * @param uint8_t
 * @returns bool
**/

bool Bmpmonochrome::setPixelAt(int index, uint8_t value) {

  if (index >= static_cast<int>(pixelArray.size())) {
    return false;
  }
  BWPixel* reqPixel = reinterpret_cast<BWPixel*>(pixelArray.at(index));
  reqPixel->setPixel(value);
  return true;
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param int
 * @returns RGBPixel*
**/

BWPixel* Bmpmonochrome::getPixelAt(int index) {

  if (index >= static_cast<int>(pixelArray.size())) {
    return nullptr;
  }
  return reinterpret_cast<BWPixel*>(pixelArray.at(index));
}
