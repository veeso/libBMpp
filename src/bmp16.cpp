/**
 *   libBMPP - Bmp16.hpp
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

#include <bmp16.hpp>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

using namespace bmp;

/**
 * @function Bmp16
 * @description Bmp16 class constructor
**/

Bmp16::Bmp16() : Bmp() {
  pixelArray.clear();
}

/**
 * @function Bmp
 * @description Bmp class constructor
**/

Bmp16::Bmp16(std::vector<Pixel*> pixelArray, size_t width, size_t height) : Bmp(pixelArray, width, height) {
  //Set bits per pixel
  header->bitsPerPixel = 8;
  //FileSize must be set by child class
  size_t nextMultipleOf4 = roundToMultiple(width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - (header->width * (header->bitsPerPixel / 8));
  size_t rowSize = paddingSize + ((width * header->bitsPerPixel / 8));
  size_t dataSize = rowSize * height;
  header->fileSize = 54 + dataSize;
  //DataSize must be set by child class
  header->dataSize = dataSize;
}

/**
 * @function ~Bmp16
 * @description Bmp16 class destructor
**/

Bmp16::~Bmp16() {
  
}


bool Bmp16::decodeBmp(uint8_t* bmpData, size_t dataSize) {

  //Call superclass decodeBmp to decode header
  if (!Bmp::decodeBmp(bmpData, dataSize)) {
    return false;
  }
  //Get data
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - (header->width * (header->bitsPerPixel / 8));
  size_t realRowSize = (header->width * (header->bitsPerPixel / 8));
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < header->fileSize - 1;) {
    //Store Pixels for each byte
    uint16_t value = bmpData[++dataPtr] << 8;
    value += bmpData[++dataPtr];
    //Create new pixel
    pixelArray.push_back(new WordPixel(value));
    rowPositionCounter += 2;
    //If row has been parsed, go to next line
    if (rowPositionCounter >= realRowSize) {
      rowPositionCounter = 0;
      dataPtr += paddingSize;
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

uint8_t* Bmp16::encodeBmp(size_t* dataSize) {
  //Get our fundamental parameters
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - header->width * (header->bitsPerPixel / 8);
  size_t realRowSize = (header->width * (header->bitsPerPixel / 8));
  //Fill header and get bmpData with fixed size
  uint8_t* bmpData = Bmp::encodeBmp(dataSize);
  //Return nullptr if needed
  if (bmpData == nullptr) {
    return nullptr;
  }
  
  //Fill data
  int pxIndex = -1;
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < *dataSize - 1;) {
    //Set pixel
    WordPixel* currPixel = reinterpret_cast<WordPixel*>(pixelArray.at(++pxIndex));
    uint16_t value = currPixel->getValue();
    bmpData[++dataPtr] = value >> 8;
    bmpData[++dataPtr] = value & 255;
    //Check if row has to be closed
    rowPositionCounter += 2;
    if (rowPositionCounter >= realRowSize) {
      //Fill row with padding and go on new line
      rowPositionCounter = 0;
      for (size_t i = 0; i < paddingSize; i++) {
        bmpData[++dataPtr] = 0;
      }
    }
  }
  return bmpData;
}

/**
 * @function resizeArea
 * @description resize area (does not scale image), both enlarging or scaling it
 * @param size_t width
 * @param size_t height
 * @param size_t xOffset (optional)
 * @param size_t yOffset (optional)
 * @returns bool
**/

bool Bmp16::resizeArea(size_t width, size_t height, size_t xOffset /* = 0*/, size_t yOffset /* = 0*/) {

  while (header->width != width || header->height != height) {
    size_t currWidth = header->width;
    size_t currHeight = header->height;
    //Resize image in order to match user requests; start with enlarging if necessary
    if (width > currWidth || height > currHeight) {
      //Pass higher size or current one to enlarge
      size_t enlargedWidth = (width > currWidth) ? width : currWidth;
      size_t enlargedHeight = (height > currHeight) ? height : currHeight;
      //Initialize Pixel lambda
      std::function<void(Pixel*&)> initializePixel = [](Pixel*& px) { px = new WordPixel(65535); };
      if (!enlargeArea(enlargedWidth, enlargedHeight, initializePixel, xOffset, yOffset)) {
        return false;
      }
      continue;
    }
    //Also, scale area if necessary
    if (width < currWidth || height << currHeight) {
      //Pass lower size or current one to scale
      size_t scaledWidth = (width < currWidth) ? width : currWidth;
      size_t scaledHeight = (height < currHeight) ? height : currHeight;
      //Scale image
      if (!scaleArea(scaledWidth, scaledHeight, xOffset, yOffset)) {
        return false;
      }
      continue;
    }
  }
  //Return OK
  return true;
}

/**
 * @function setPixelAt
 * @description: replace pixel in a certain position with the provided one
 * @param int
 * @param int
 * @param uint16_t
 * @returns bool
**/

bool Bmp16::setPixelAt(int row, int column, uint16_t value) {

  //Get index, considering that pixels are stored bottom to top
  int reversedRow = (header->height - 1 - row); // h - 1 - r
  int index = (header->width * reversedRow) + column;

  if (index >= static_cast<int>(pixelArray.size())) {
    return false;
  }
  WordPixel* reqPixel = reinterpret_cast<WordPixel*>(pixelArray.at(index));
  reqPixel->setPixel(value);
  return true;
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param int
 * @param int
 * @returns RGBPixel*
**/

WordPixel* Bmp16::getPixelAt(int row, int column) {

  //Get index, considering that pixels are stored bottom to top
  int reversedRow = (header->height - 1 - row); // h - 1 - r
  int index = (header->width * reversedRow) + column;

  if (index >= static_cast<int>(pixelArray.size())) {
    return nullptr;
  }
  return reinterpret_cast<WordPixel*>(pixelArray.at(index));
}
