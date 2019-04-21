/**
 *   libBMPP - bmp24.hpp
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

#include <bmp24.hpp>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

#define MIN(a,b) ((a) < (b) ? (a) : (b))

using namespace bmp;

/**
 * @function Bmp24
 * @description Bmp24 class constructor
**/

Bmp24::Bmp24() : Bmp() {
  pixelArray.clear();
}

/**
 * @function Bmp
 * @description Bmp class constructor
**/

Bmp24::Bmp24(std::vector<Pixel*> pixelArray, size_t width, size_t height) : Bmp(pixelArray, width, height) {
  //Set bits per pixel
  header->bitsPerPixel = 24;
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
 * @function ~Bmp24
 * @description Bmp24 class destructor
**/

Bmp24::~Bmp24() {
  
}

/**
 * @function decodeBmp
 * @description decode Bmp data buffer converting it to header struct and RGBPixel array
 * @param uint8_t*
 * @param size_t
 * @returns bool
**/


bool Bmp24::decodeBmp(uint8_t* bmpData, size_t dataSize) {

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
    //Store Pixels for each byte NOTE: BMP is BGR
    uint8_t blue = bmpData[++dataPtr];
    uint8_t green = bmpData[++dataPtr];
    uint8_t red = bmpData[++dataPtr];
    //Create new pixel
    pixelArray.push_back(new RGBPixel(red, green, blue));
    rowPositionCounter += 3;
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

uint8_t* Bmp24::encodeBmp(size_t* dataSize) {  

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
    if (++pxIndex >= static_cast<int>(pixelArray.size())) {
      break;
    }
    //Set pixel
    RGBPixel* currPixel = reinterpret_cast<RGBPixel*>(pixelArray.at(pxIndex));
    bmpData[++dataPtr] = currPixel->getBlue();
    bmpData[++dataPtr] = currPixel->getGreen();
    bmpData[++dataPtr] = currPixel->getRed();
    //Check if row has to be closed
    rowPositionCounter += 3;
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
 * @function setPixelAt
 * @description: replace pixel in a certain position with the provided one
 * @param int
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
 * @returns bool
**/

bool Bmp24::setPixelAt(int index, uint8_t red, uint8_t green, uint8_t blue) {

  if (index >= static_cast<int>(pixelArray.size())) {
    return false;
  }
  RGBPixel* reqPixel = reinterpret_cast<RGBPixel*>(pixelArray.at(index));
  reqPixel->setPixel(red, green, blue);
  return true;
}

/**
 * @function toGreyScale
 * @description: convert bitmap to greyscaleArea; if greyLevels is set, provided amount of greys will be used
 * @param int
 * @returns bool
**/

bool Bmp24::toGreyScale(int greyLevels /*= 255*/) {

  //Convert each pixel to grey
  for (auto& pixel : pixelArray) {
    RGBPixel* rgbPixel = reinterpret_cast<RGBPixel*>(pixel);
    //Get average value (grey for each pixel)
    uint8_t red = rgbPixel->getRed();
    uint8_t green = rgbPixel->getGreen();
    uint8_t blue = rgbPixel->getBlue();
    uint8_t greyValue = (red + green + blue) / 3;
    //Apply greyLevels
    if (greyLevels < 255) {
      uint8_t appliedLevel = 255 / greyLevels;
      //Fix grey based on levels we have
      greyValue = greyValue - (greyValue % appliedLevel);
    }
    rgbPixel->setPixel(greyValue, greyValue, greyValue);
  }
  return true;
}

/**
 * @function toSepiaTone
 * @description: converts image to sepia tone
 * @returns bool
**/

bool Bmp24::toSepiaTone() {
  //Convert each pixel to sepia
  for (auto& pixel : pixelArray) {
    RGBPixel* rgbPixel = reinterpret_cast<RGBPixel*>(pixel);
    uint8_t red = MIN((rgbPixel->getRed() * 0.393) + (rgbPixel->getGreen() * 0.769) + (rgbPixel->getBlue() * 0.189), 255);
    uint8_t green = MIN((rgbPixel->getRed() * 0.349) + (rgbPixel->getGreen() * 0.686) + (rgbPixel->getBlue() * 0.168), 255);
    uint8_t blue = MIN((rgbPixel->getRed() * 0.272) + (rgbPixel->getGreen() * 0.534) + (rgbPixel->getBlue() * 0.131), 255);
    rgbPixel->setPixel(red, green, blue);
  }
  return true;
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param int
 * @returns RGBPixel*
**/

RGBPixel* Bmp24::getPixelAt(int index) {

  if (index >= static_cast<int>(pixelArray.size())) {
    return nullptr;
  }
  return reinterpret_cast<RGBPixel*>(pixelArray.at(index));
}
