/**
 *   libBMpp - bmpmonochrome.hpp
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

#include <fstream>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

namespace bmp {

/**
 * @function Bmpmonochrome
 * @description Bmpmonochrome class constructor
**/

Bmpmonochrome::Bmpmonochrome() : Bmp() {
  pixelArray.clear();
}

/**
 * @function Bmpmonochrome
 * @description Bmp class constructor
**/

Bmpmonochrome::Bmpmonochrome(size_t width, size_t height, uint8_t defaultColor) : Bmp(width, height) {
  //Set bits per pixel
  header->bitsPerPixel = 1;
  //FileSize must be set by child class
  size_t nextMultipleOf4 = roundToMultiple(width, 4);
  size_t paddingSize = nextMultipleOf4 - (header->width);
  size_t rowSize = paddingSize + (width);
  size_t dataSize = rowSize * height;
  header->fileSize = 54 + dataSize;
  //DataSize must be set by child class
  header->dataSize = dataSize;
  //Create empty image
  size_t arraySize = header->width * header->height;
  pixelArray.reserve(arraySize);
  for (size_t i = 0; i < arraySize; i++) {
    pixelArray.push_back(new BWPixel(defaultColor));
  }
}

/**
 * @function Bmpmonochrome
 * @description Bmpmonochrome class copy constructor
 * @param const Bmp& bmp
 */

Bmpmonochrome::Bmpmonochrome(const Bmpmonochrome& bmp) : Bmp(bmp) {
  //Copy pixel array to new bmp
  size_t arraySize = bmp.pixelArray.size();
  for (size_t i = 0; i < arraySize; i++) {
    BWPixel* copyPixel = reinterpret_cast<BWPixel*>(bmp.pixelArray.at(i));
    pixelArray.push_back(new BWPixel(copyPixel->getValue()));
  }
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
  size_t nextMultipleOf4 = roundToMultiple(header->width, 4);
  size_t paddingSize = nextMultipleOf4 - (header->width);
  size_t realRowSize = (header->width);
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < header->fileSize - 1; dataPtr++) {
    uint8_t currentByte = bmpData[dataPtr];
    for (size_t i = 0; pixelArray.size() <= totalArea && i < 8; i++) {
      //Iterate over bits in byte to get color
      uint8_t shiftValue = 8 - i;
      uint8_t colorValue = (currentByte >> shiftValue) & 1;
      pixelArray.push_back(new BWPixel(colorValue));
    }
    rowPositionCounter++;
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

uint8_t* Bmpmonochrome::encodeBmp(size_t& dataSize) {
  size_t nextMultipleOf4 = roundToMultiple(header->width, 4);
  size_t paddingSize = nextMultipleOf4 - header->width;
  size_t realRowSize = (header->width);
  //Fill header and get bmpData with fixed size
  uint8_t* bmpData = Bmp::encodeBmp(dataSize);
  //Return nullptr if needed
  if (bmpData == nullptr) {
    return nullptr;
  }
  size_t byteCounter = 0;
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < dataSize - 1;) {
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
    //Check if row has to be closed
    rowPositionCounter++;
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
 * @function readBmp
 * @description read a BMP file and decodes it. The decoded Bmp becomes the object
 * @param const std::string& bmpFile
 * @returns bool
 */

bool Bmpmonochrome::readBmp(const std::string& bmpFile) {
  std::ifstream iFile;
  iFile.open(bmpFile, std::ios::binary | std::ios::ate);
  if (!iFile.is_open()) {
    return false;
  }
  std::streamsize size = iFile.tellg();
  iFile.seekg(0, std::ios::beg);
  char* dataBuffer = new char[size];
  if (!iFile.read(dataBuffer, size)) {
    delete[] dataBuffer;
    return false;
  }
  iFile.close();
  //Decode
  bool rc = decodeBmp(reinterpret_cast<uint8_t*>(dataBuffer), size);
  delete[] dataBuffer;
  return rc;
}

/**
 * @function writeBmp
 * @description encode BMP write the buffer to a file.
 * @param const std::string& bmpFile
 * @returns bool
 */

bool Bmpmonochrome::writeBmp(const std::string& bmpFile) {
  size_t outDataSize;
  uint8_t* outBuf = encodeBmp(outDataSize);
  if (outBuf == nullptr) {
    return false;
  }
  //Write file
  std::ofstream outFile;
  outFile.open(bmpFile);
  if (!outFile.is_open()) {
    delete[] outBuf;
    return false;
  }
  for (size_t i = 0; i < outDataSize; i++) {
    outFile << outBuf[i];
  }
  outFile.close();
  return true;
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

bool Bmpmonochrome::resizeArea(size_t width, size_t height, size_t xOffset /* = 0*/, size_t yOffset /* = 0*/) {

  while (header->width != width || header->height != height) {
    size_t currWidth = header->width;
    size_t currHeight = header->height;
    //Resize image in order to match user requests; start with enlarging if necessary
    if (width > currWidth || height > currHeight) {
      //Pass higher size or current one to enlarge
      size_t enlargedWidth = (width > currWidth) ? width : currWidth;
      size_t enlargedHeight = (height > currHeight) ? height : currHeight;
      //Initialize Pixel lambda
      std::function<void(Pixel*&)> initializePixel = [](Pixel*& px) { px = new BWPixel(1); };
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
 * @function resizeImage
 * @description resize image (not only area) applying bilinear image scaling
 * @param size_t
 * @param size_t
 * @returns bool
**/

bool Bmpmonochrome::resizeImage(size_t width, size_t height) {
  //Apply resizing
  size_t prevWidth = header->width;
  size_t prevHeight = header->height;
  std::vector<Pixel*> resizedArray; //New pixel vector
  float xRatio = static_cast<float>(prevWidth - 1) / width;
  float yRatio = static_cast<float>(prevHeight - 1) / height;
  //Working variables
  BWPixel *px1, *px2, *px3, *px4;
  int x, y, index;
  float xDiff, yDiff;
  uint8_t value;
  for (size_t row = 0; row < height; row++) {
    for (size_t column = 0; column < width; column++) {
      x = static_cast<int>(xRatio * column);
      y = static_cast<int>(yRatio * row);
      xDiff = (xRatio * column) - x;
      yDiff = (yRatio * row) - y;
      index = ((y * prevWidth) + x);
      px1 = reinterpret_cast<BWPixel*>(pixelArray.at(index));
      px2 = reinterpret_cast<BWPixel*>(pixelArray.at(index + 1));
      px3 = reinterpret_cast<BWPixel*>(pixelArray.at(index + prevWidth));
      px4 = reinterpret_cast<BWPixel*>(pixelArray.at(index + prevWidth + 1));
      //Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
      value = (px1->getValue() * (1 - xDiff) * (1 - yDiff)) + (px2->getValue() * (xDiff) * (1 - yDiff)) + (px3->getValue() * (yDiff) * (1 - xDiff)) + (px4->getValue() * (xDiff * yDiff));
      //Instance new pixel
      Pixel* resizedPixel = new BWPixel(value);
      //Push pixel into array
      resizedArray.push_back(resizedPixel);
    }
  }
  //Delete all pixels in previous pixel array
  for (auto& pixel : pixelArray) {
    BWPixel* px = reinterpret_cast<BWPixel*>(pixel);
    delete px;
  }
  pixelArray = resizedArray;
  //Change header parameters
  return Bmp::resizeImage(width, height);
}

/**
 * @function setPixelAt
 * @description: replace pixel in a certain position with the provided one
 * @param size_t
 * @param size_t
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
 * @returns bool
**/

bool Bmpmonochrome::setPixelAt(size_t row, size_t column, uint8_t value) {
  //Get index, considering that pixels are stored bottom to top
  size_t reversedRow = (header->height - 1 - row); // h - 1 - r
  size_t index = (header->width * reversedRow) + column;
  return setPixelAt(index, value);
}

/**
 * @function setPixelAt
 * @description: replace pixel in a certain position with the provided one
 * @param size_t
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
 * @returns bool
**/

bool Bmpmonochrome::setPixelAt(size_t index, uint8_t value) {
  if (index >= pixelArray.size()) {
    return false;
  }
  BWPixel* reqPixel = reinterpret_cast<BWPixel*>(pixelArray.at(index));
  reqPixel->setPixel(value);
  return true;
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param size_t
 * @param size_t
 * @returns BWPixel*
**/

BWPixel* Bmpmonochrome::getPixelAt(size_t row, size_t column) {

  //Get index, considering that pixels are stored bottom to top
  int reversedRow = (header->height - 1 - row); // h - 1 - r
  int index = (header->width * reversedRow) + column;

  if (index >= pixelArray.size()) {
    return nullptr;
  }
  return getPixelAt(index);
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param size_t
 * @returns BWPixel*
**/

BWPixel* Bmpmonochrome::getPixelAt(size_t index) {

  if (index >= pixelArray.size()) {
    return nullptr;
  }
  return reinterpret_cast<BWPixel*>(pixelArray.at(index));
}

}
