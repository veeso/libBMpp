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

#include <bmp24.hpp>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

using namespace bmp;

/**
 * @function Bmp24
 * @description Bmp24 class constructor
**/

Bmp24::Bmp24() {
  header = nullptr;
}

/**
 * @function ~Bmp24
 * @description Bmp24 class destructor
**/

Bmp24::~Bmp24() {
  //Delete header
  if (header != nullptr) {
    delete[] header;
  }
  //Delete pixels
  for (auto& pixel : pixelArray) {
    delete pixel;
  }
}

/**
 * @function decodeBmp
 * @description decode Bmp data buffer converting it to header struct and RGBPixel array
 * @param uint8_t*
 * @param size_t
 * @returns bool
**/


bool Bmp24::decodeBmp(uint8_t* bmpData, size_t dataSize) {

  //Header is too small
  if (dataSize < 54) {
    return false;
  }
  //Create header
  header = new Header();
  //Get header
  //BMP ID 'BM'
  header->bmpId = (bmpData[0] << 8) + bmpData[1];
  if (header->bmpId != BMP_ID) {
    return false;
  }

  //Filesize
  header->fileSize = bmpData[5];
  header->fileSize = header->fileSize << 8;
  header->fileSize += bmpData[4];
  header->fileSize = header->fileSize << 8;
  header->fileSize += bmpData[3];
  header->fileSize = header->fileSize << 8;
  header->fileSize += bmpData[2];
  //Data offset
  header->dataOffset = bmpData[13];
  header->dataOffset = header->dataOffset << 8;
  header->dataOffset += bmpData[12];
  header->dataOffset = header->dataOffset << 8;
  header->dataOffset += bmpData[11];
  header->dataOffset = header->dataOffset << 8;
  header->dataOffset += bmpData[10];
  //DibSIZE
  header->dibSize = bmpData[17];
  header->dibSize = header->dibSize << 8;
  header->dibSize += bmpData[16];
  header->dibSize = header->dibSize << 8;
  header->dibSize += bmpData[15];
  header->dibSize = header->dibSize << 8;
  header->dibSize += bmpData[14];
  //Width
  header->width = bmpData[21];
  header->width = header->width << 8;
  header->width += bmpData[20];
  header->width = header->width << 8;
  header->width += bmpData[19];
  header->width = header->width << 8;
  header->width += bmpData[18];
  //Height
  header->height = bmpData[25];
  header->height = header->height << 8;
  header->height += bmpData[24];
  header->height = header->height << 8;
  header->height += bmpData[23];
  header->height = header->height << 8;
  header->height += bmpData[22];
  //Color Planes
  header->colorPlanes = bmpData[27];
  header->colorPlanes = header->colorPlanes << 8;
  header->colorPlanes += bmpData[26];
  //Bits per pixel
  header->bitsPerPixel = bmpData[29];
  header->bitsPerPixel = header->bitsPerPixel << 8;
  header->bitsPerPixel += bmpData[28];
  //biRgb
  header->biRgb = bmpData[33];
  header->biRgb = header->biRgb << 8;
  header->biRgb += bmpData[32];
  header->biRgb = header->biRgb << 8;
  header->biRgb += bmpData[31];
  header->biRgb = header->biRgb << 8;
  header->biRgb += bmpData[30];
  //DataSize
  header->dataSize = bmpData[37];
  header->dataSize = header->dataSize << 8;
  header->dataSize += bmpData[36];
  header->dataSize = header->dataSize << 8;
  header->dataSize += bmpData[35];
  header->dataSize = header->dataSize << 8;
  header->dataSize += bmpData[34];
  //Print size W
  header->printSizeW = bmpData[41];
  header->printSizeW = header->printSizeW << 8;
  header->printSizeW += bmpData[40];
  header->printSizeW = header->printSizeW << 8;
  header->printSizeW += bmpData[39];
  header->printSizeW = header->printSizeW << 8;
  header->printSizeW += bmpData[38];
  //Print size H
  header->printSizeH = bmpData[45];
  header->printSizeH = header->printSizeH << 8;
  header->printSizeH += bmpData[44];
  header->printSizeH = header->printSizeH << 8;
  header->printSizeH += bmpData[43];
  header->printSizeH = header->printSizeH << 8;
  header->printSizeH += bmpData[42];
  //Palette Size
  header->paletteSize = bmpData[49];
  header->paletteSize = header->paletteSize << 8;
  header->paletteSize += bmpData[48];
  header->paletteSize = header->paletteSize << 8;
  header->paletteSize += bmpData[47];
  header->paletteSize = header->paletteSize << 8;
  header->paletteSize += bmpData[46];
  //Important Colors
  header->importantColors = bmpData[53];
  header->importantColors = header->importantColors << 8;
  header->importantColors += bmpData[52];
  header->importantColors = header->importantColors << 8;
  header->importantColors += bmpData[51];
  header->importantColors = header->importantColors << 8;
  header->importantColors += bmpData[50];

  //Get data
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - (header->width * (header->bitsPerPixel / 8));
  size_t realRowSize = (header->width * (header->bitsPerPixel / 8));
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < dataSize;) {
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

  if (header == nullptr) {
    return 0;
  }

  //Get our fundamental parameters
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - header->width * (header->bitsPerPixel / 8);
  size_t realRowSize = (header->width * (header->bitsPerPixel / 8));
  size_t totalRowSize = (header->width * (header->bitsPerPixel / 8)) + paddingSize;
  //Recalc dataSize
  int pxDataSize = totalRowSize * header->height;
  //Recalc dataSize
  header->dataSize = pxDataSize;
  //We need to allocate the buffer now (headerSize [54] + dataSize)
  *dataSize = 54 + header->dataSize;
  uint8_t* bmpData = new uint8_t[*dataSize];
  //Fill header
  bmpData[0] = 'B';
  bmpData[1] = 'M';
  //FileSize
  bmpData[2] = *dataSize;
  bmpData[3] = *dataSize >> 8;
  bmpData[4] = *dataSize >> 16;
  bmpData[5] = *dataSize >> 24;
  //Reserved
  bmpData[6] = 0;
  bmpData[7] = 0;
  bmpData[8] = 0;
  bmpData[9] = 0;
  //Data offset
  bmpData[10] = 54;
  bmpData[11] = 0;
  bmpData[12] = 0;
  bmpData[13] = 0;
  //DibSize
  bmpData[14] = 0x28;
  bmpData[15] = 0;
  bmpData[16] = 0;
  bmpData[17] = 0;
  //Width
  bmpData[18] = header->width;
  bmpData[19] = header->width >> 8;
  bmpData[20] = header->width >> 16;
  bmpData[21] = header->width >> 24;
  //Height
  bmpData[22] = header->height;
  bmpData[23] = header->height >> 8;
  bmpData[24] = header->height >> 16;
  bmpData[25] = header->height >> 24;
  //Color planes
  bmpData[26] = header->colorPlanes;
  bmpData[27] = header->colorPlanes >> 8;
  //Bits per pixel
  bmpData[28] = header->bitsPerPixel;
  bmpData[29] = header->bitsPerPixel >> 8;
  //biRgb
  bmpData[30] = header->biRgb;
  bmpData[31] = header->biRgb >> 8;
  bmpData[32] = header->biRgb >> 16;
  bmpData[33] = header->biRgb >> 24;
  //DataSize
  bmpData[34] = header->dataSize;
  bmpData[35] = header->dataSize >> 8;
  bmpData[36] = header->dataSize >> 16;
  bmpData[37] = header->dataSize >> 24;
  //Print size w
  bmpData[38] = header->printSizeW;
  bmpData[39] = header->printSizeW >> 8;
  bmpData[40] = header->printSizeW >> 16;
  bmpData[41] = header->printSizeW >> 24;
  //Print size H
  bmpData[42] = header->printSizeH;
  bmpData[43] = header->printSizeH >> 8;
  bmpData[44] = header->printSizeH >> 16;
  bmpData[45] = header->printSizeH >> 24;
  //Palette
  bmpData[46] = 0;
  bmpData[47] = 0;
  bmpData[48] = 0;
  bmpData[49] = 0;
  //Important colors 0
  bmpData[50] = 0;
  bmpData[51] = 0;
  bmpData[52] = 0;
  bmpData[53] = 0;

  //Fill data
  int pxIndex = -1;
  size_t rowPositionCounter = 0;
  for (size_t dataPtr = header->dataOffset - 1; dataPtr < *dataSize;) {
    if (++pxIndex >= pixelArray.size()) {
      break;
    }
    //Set pixel
    RGBPixel* currPixel = pixelArray.at(pxIndex);
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

  if (index >= pixelArray.size()) {
    return false;
  }
  RGBPixel* reqPixel = pixelArray.at(index);
  reqPixel->setPixel(red, green, blue);
  return true;
}

/**
 * @function toGreyScale
 * @description: convert bitmap to greyscale; if greyLevels is set, provided amount of greys will be used
 * @param int
 * @returns bool
**/

bool Bmp24::toGreyScale(int greyLevels /*= 255*/) {

  //Convert each pixel to grey
  for (auto& pixel : pixelArray) {
    //Get average value (grey for each pixel)
    uint8_t red = pixel->getRed();
    uint8_t green = pixel->getGreen();
    uint8_t blue = pixel->getBlue();
    uint8_t greyValue = (red + green + blue) / 3;
    //Apply greyLevels
    if (greyLevels < 255) {
      uint8_t appliedLevel = 255 / greyLevels;
      //Fix grey based on levels we have
      greyValue = greyValue - (greyValue % appliedLevel);
    }
    pixel->setPixel(greyValue, greyValue, greyValue);
  }
  return true;
}

/**
 * @function rotate
 * @description: rotates the image by a multiple of 90
 * @param int
 * @returns bool
**/

bool Bmp24::rotate(int degrees) {

  if (degrees % 90 != 0) {
    return false;
  }

  if (header == nullptr) {
    return false;
  }

  uint32_t previousWidth = header->width;
  uint32_t previousHeight = header->height;

  if (degrees == 90 || degrees == 270) {
    //Exchange header attributes
    //Update width and height
    header->height = previousWidth;
    header->width = previousHeight;
    //Rotate also printH/W
    uint32_t transitionAttr = header->printSizeH;
    header->printSizeH = header->printSizeW;
    header->printSizeW = transitionAttr;
  }

  std::vector<std::vector<RGBPixel*>> currentPixelMatrix;
  int counter = -1;
  //Store pixels in a matrix
  for (size_t row = 0; row < previousHeight; row++) {
    std::vector<RGBPixel*> rowVector;
    for (size_t column = 0; column < previousWidth; column++) {
      rowVector.push_back(pixelArray.at(++counter));
    }
    currentPixelMatrix.push_back(rowVector);
  }

  if (degrees == 270 || degrees == 90) {
    std::vector<std::vector<RGBPixel*>> rotatedPixelMatrix;
    //Create rotated matrix
    counter = -1;
    for (size_t row = 0; row < previousWidth; row++) {
      std::vector<RGBPixel*> rowVector;
      for (size_t column = 0; column < previousHeight; column++) {
        rowVector.push_back(pixelArray.at(++counter));
      }
      rotatedPixelMatrix.push_back(rowVector);
    }
    if (degrees == 270) {
      //Rotate 270 degrees
      for (size_t i = 0; i < previousHeight; i++) {
        for (size_t j = 0; j < previousWidth; j++) {
          rotatedPixelMatrix.at(j).at(previousHeight - 1 - i) = currentPixelMatrix.at(i).at(j);
        }
      }
    } else {
      //Rotate 90 degrees
      for (size_t i = 0; i < previousHeight; i++) {
        for (size_t j = 0; j < previousWidth; j++) {
         rotatedPixelMatrix.at(previousWidth - 1 - j).at(i) = currentPixelMatrix.at(i).at(j);
        }
      }
    }
    //Convert rotated matrix to pixelArray
    counter = -1;
    for (size_t row = 0; row < previousWidth; row++) {
      for (size_t column = 0; column < previousHeight; column++) {
        pixelArray.at(++counter) = rotatedPixelMatrix.at(row).at(column);
      }
    }
  } else if (degrees == 180) {
    //Swap elements to rotate image
    for (size_t i = 0; i < previousHeight; i++) {
      for (size_t j = 0, k = previousWidth - 1; j < k; j++, k--) {
        std::swap(currentPixelMatrix.at(i).at(j), currentPixelMatrix.at(i).at(k));
      }
    }
    for (size_t j = 0; j < previousWidth; j++) {
      for (size_t i = 0, k = previousHeight - 1; i < k; i++, k--) {
        std::swap(currentPixelMatrix.at(i).at(j), currentPixelMatrix.at(k).at(j));
      }
    }
    //Convert rotated matrix to pixelArray
    counter = -1;
    for (size_t row = 0; row < previousHeight; row++) {
      for (size_t column = 0; column < previousWidth; column++) {
        pixelArray.at(++counter) = currentPixelMatrix.at(row).at(column);
      }
    }
  }
  return true;
}

/**
 * @function getWidth
 * @description: returns the image width
 * @returns size_t
**/

size_t Bmp24::getWidth() {
  if (header != nullptr) {
    return header->width;
  } else {
    return 0;
  }
}

/**
 * @function getHeight
 * @description: returns the image height
 * @returns size_t
**/

size_t Bmp24::getHeight() {
  if (header != nullptr) {
    return header->height;
  } else {
    return 0;
  }
}

/**
 * @function getPixelAt
 * @description return pointer to pixel in the provided position
 * @param int
 * @returns RGBPixel*
**/

RGBPixel* Bmp24::getPixelAt(int index) {

  if (index >= pixelArray.size()) {
    return nullptr;
  }
  return pixelArray.at(index);
}

/**
 * @function rountToMultiple
 * @description: rount provided integer to the nearest bigger multiple of 'multiple'
 * @param int
 * @param int
 * @returns int
**/

int Bmp24::roundToMultiple(int toRound, int multiple) {
  if (multiple == 0)
    return toRound;

  int remainder = toRound % multiple;
  if (remainder == 0)
    return toRound;

  return toRound + multiple - remainder;
}
