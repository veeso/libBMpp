/**
 *   libBMP24 - bmp.cpp
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

#include <bmp.hpp>

#include <cstring>

#ifdef BMP_DEBUG
#include <iostream>
#include <string>
#endif

#define BMP_ID 0x424D
#define HORIZONTAL_FLIP 'H'
#define VERTICAL_FLIP 'V'

using namespace bmp;

/**
 * @function Bmp
 * @description Bmp class constructor
**/

Bmp::Bmp() {
  header = nullptr;
}

/**
 * @function Bmp
 * @description Bmp class constructor
**/

Bmp::Bmp(std::vector<Pixel*> pixelArray, size_t width, size_t height) {
  //Set pixelArray
  this->pixelArray = pixelArray;
  //Create Header
  header = new Header();
  header->bmpId = BMP_ID;
  //FileSize must be set by child class
  header->width = width;
  header->height = height;
  //DataSize must be set by child class
  //bitsPerPixel must be set by child class
  header->printSizeW = 
  header->paletteSize = 0;
  header->importantColors = 0;
}

/**
 * @function ~Bmp
 * @description Bmp class destructor
**/

Bmp::~Bmp() {
  //Delete header
  if (header != nullptr) {
    delete header;
  }
  //Delete pixels
  for (auto& pixel : pixelArray) {
    delete pixel;
    pixel = nullptr;
  }
  pixelArray.clear();
  delete[] dibData;
}

/**
 * @function decodeBmp
 * @description decode Bmp data buffer filling header struct
 * @param uint8_t*
 * @param size_t
 * @returns bool
**/

bool Bmp::decodeBmp(uint8_t* bmpData, size_t dataSize) {

  //Header is too small
  if (dataSize < 54) {
    return false;
  }
  //Delete previous header if exists
  if (header != nullptr) {
    delete header;
  }

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

  //Save dibData
  size_t dibDataSize = header->dataOffset - 54;
  dibData = new uint8_t[dibDataSize];
  memcpy(dibData, bmpData + 54, dibDataSize);

  return true;

}

/**
 * @function encodeBmp
 * @description: encodes bitmap to buffer; only header is encoded in Bmp class
 * @param size_t*
 * @returns uint8_t*
**/

uint8_t* Bmp::encodeBmp(size_t* dataSize) {

  if (header == nullptr) {
    return nullptr;
  }

  //Get our fundamental parameters
  size_t nextMultipleOf4 = roundToMultiple(header->width * (header->bitsPerPixel / 8), 4);
  size_t paddingSize = nextMultipleOf4 - header->width * (header->bitsPerPixel / 8);
  size_t totalRowSize = (header->width * (header->bitsPerPixel / 8)) + paddingSize;
  //Recalc dataSize
  int pxDataSize = totalRowSize * header->height;
  //Recalc dataSize
  header->dataSize = pxDataSize;
  //We need to allocate the buffer now (dataOffset + dataSize)
  *dataSize = header->dataOffset + header->dataSize;
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
  bmpData[10] = header->dataOffset;
  bmpData[11] = header->dataOffset >> 8;
  bmpData[12] = header->dataOffset >> 16;
  bmpData[13] = header->dataOffset >> 24;
  //DibSize
  bmpData[14] = header->dibSize;
  bmpData[15] = header->dibSize >> 8;
  bmpData[16] = header->dibSize >> 16;
  bmpData[17] = header->dibSize >> 24;
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
  //Store to bmpData dbData
  memcpy(bmpData + 54, dibData, (header->dataOffset - 54));

  return bmpData;

}

/**
 * @function rotate
 * @description: rotates the image by a multiple of 90
 * @param int
 * @returns bool
**/

bool Bmp::rotate(int degrees) {

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

  std::vector<std::vector<Pixel*>> currentPixelMatrix;
  int counter = -1;
  //Store pixels in a matrix
  for (size_t row = 0; row < previousHeight; row++) {
    std::vector<Pixel*> rowVector;
    for (size_t column = 0; column < previousWidth; column++) {
      rowVector.push_back(pixelArray.at(++counter));
    }
    currentPixelMatrix.push_back(rowVector);
  }

  if (degrees == 270 || degrees == 90) {
    std::vector<std::vector<Pixel*>> rotatedPixelMatrix;
    //Create rotated matrix
    counter = -1;
    for (size_t row = 0; row < previousWidth; row++) {
      std::vector<Pixel*> rowVector;
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
 * @function flipHorizontal
 * @description flip image horizontally
 * @returns bool
**/

bool Bmp::flipHorizontal() {
  return this->flip(HORIZONTAL_FLIP);
}

/**
 * @function flipHorizontal
 * @description flip image horizontally
 * @returns bool
**/

bool Bmp::flipVertical() {
  return this->flip(VERTICAL_FLIP);
}

/**
 * @function resizeArea
 * @description: resize area of the image from the bottom left; image can't become bigger than before
 * @param size_t
 * @param size_t
 * @param size_t
 * @param size_t
 * @returns bool
**/

bool Bmp::resizeArea(size_t width, size_t height, size_t xOffset /* = 0 */, size_t yOffset /* = 0 */) {

  //Out image is bigger than current image
  if (width + xOffset > header->width || height + yOffset > header->height) {
    return false;
  }
  //Since pixels are stored bottom to top, we need to flip and then resize the matrix
  if (!flipVertical()) {
    return false;
  }
  //If offset is set cut starting from offset
  if (xOffset > 0 || yOffset > 0) {
    //Flip horizontally
    if (!flipHorizontal()) {
      return false;
    }
    //Resize passing offsets as width and height
    if (!resizeArea(header->width - xOffset, header->height - yOffset)) {
      return false;
    }
    //Reflip horizontally
    if (!flipHorizontal()) {
      return false;
    }
  }
  //Convert image to matrix
  std::vector<std::vector<Pixel*>> pixelMatrix;
  //Store pixels in a matrix
  size_t counter = 0;
  for (size_t row = 0; row < header->height; row++) {
    std::vector<Pixel*> rowVector;
    for (size_t column = 0; column < header->width; column++) {
      rowVector.push_back(pixelArray.at(counter));
      counter++;
    }
    pixelMatrix.push_back(rowVector);
  }
  //Cut image
  std::vector<std::vector<Pixel*>> resizedPixelMatrix = pixelMatrix;
  //Resize height
  resizedPixelMatrix.resize(height);
  //Resize each row
  for (size_t i = 0; i < height; i++) {
    resizedPixelMatrix.at(i).resize(width);
  }
  //Delete out of bounds pixels
  size_t count = 0;
  //Delete all rows elements which have been cut off
  for (size_t i = height; i < header->height; i++) {
    for (size_t j = 0; j < header->width; j++) {
      delete pixelMatrix.at(i).at(j);
      count++;
    }
  }
  //Delete cut columns
  for (size_t i = 0; i < height; i++) {
    for (size_t j = width; j < header->width; j++) {
      delete pixelMatrix.at(i).at(j);
      count++;
    }
  }

  //Convert matrix to array
  pixelArray.clear();
  for (size_t row = 0; row < height; row++) {
    for (size_t column = 0; column < width; column++) {
      pixelArray.push_back(resizedPixelMatrix.at(row).at(column));
    }
  }

  //Update BMP header
  header->width = width;
  header->height = height;
  //Re-flip image
  if (!flipVertical()) {
    return false;
  }
  return true;
}

/* Specchiato 4 inverso effect
    for (size_t row = 0; row < header->height; row++) {
      for (size_t column = 0; column < header->width / 2; column++) {
        std::swap(pixelMatrix.at(row).at(column), pixelMatrix.at(row).at(header->width - 1 - column));
        pixelArray.at(++counter) = pixelMatrix.at(row).at(column);
        pixelArray.at(pixelArray.size() - 1 - counter) = pixelMatrix.at(row).at(header->width - 1 - column);
      }
    }
*/

/**
 * @function getWidth
 * @description: returns the image width
 * @returns size_t
**/

size_t Bmp::getWidth() {
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

size_t Bmp::getHeight() {
  if (header != nullptr) {
    return header->height;
  } else {
    return 0;
  }
}

/**
 * @function flip
 * @description: flip image horizontally or vertically based on argument
 * @param char
 * @returns bool
**/

bool Bmp::flip(char flipType) {
  
  //Invalid flip operation
  if (flipType != HORIZONTAL_FLIP && flipType != VERTICAL_FLIP) {
    return false;
  }
  //Check if header exists
  if (header == nullptr) {
    return false;
  }

  //Apply vertical flip
  if (flipType == VERTICAL_FLIP) {
    //Apply horizontal flip
    std::vector<std::vector<Pixel*>> pixelMatrix;
    int counter = -1;
    //Store pixels in a matrix
    for (size_t row = 0; row < header->height; row++) {
      std::vector<Pixel*> rowVector;
      for (size_t column = 0; column < header->width; column++) {
        rowVector.push_back(pixelArray.at(++counter));
      }
      pixelMatrix.push_back(rowVector);
    }
    for (size_t row = 0; row < header->height / 2; row++) {
      std::swap(pixelMatrix.at(row), pixelMatrix.at(header->height - 1 - row));
    }
    //Convert rotated matrix to pixelArray
    counter = -1;
    for (size_t row = 0; row < header->height; row++) {
      for (size_t column = 0; column < header->width; column++) {
        pixelArray.at(++counter) = pixelMatrix.at(row).at(column);
      }
    }
  } else { 
    //Apply horizontal flip
    std::vector<std::vector<Pixel*>> pixelMatrix;
    int counter = -1;
    //Store pixels in a matrix
    for (size_t row = 0; row < header->height; row++) {
      std::vector<Pixel*> rowVector;
      for (size_t column = 0; column < header->width; column++) {
        rowVector.push_back(pixelArray.at(++counter));
      }
      pixelMatrix.push_back(rowVector);
    }
    //Flip horizontally
    counter = -1;
    for (size_t row = 0; row < header->height; row++) {
      //Save counter at new line
      int beginRowCounter = counter;
      for (size_t column = 0; column < header->width / 2; column++) {
        //Swap elements on a row
        std::swap(pixelMatrix.at(row).at(column), pixelMatrix.at(row).at(header->width - 1 - column));
        //Put left side element on left side of row in pixel array
        pixelArray.at(++counter) = pixelMatrix.at(row).at(column);
        //Put right side element on right side of row in pixel array
        pixelArray.at((header->width - column) + beginRowCounter) = pixelMatrix.at(row).at(header->width - 1 - column);
      }
      //Add to counter missing part to finish line
      counter += header->width / 2;
    }
  }
  return true;
}

/**
 * @function rountToMultiple
 * @description: rount provided integer to the nearest bigger multiple of 'multiple'
 * @param int
 * @param int
 * @returns int
**/

int Bmp::roundToMultiple(int toRound, int multiple) {
  if (multiple == 0)
    return toRound;

  int remainder = toRound % multiple;
  if (remainder == 0)
    return toRound;

  return toRound + multiple - remainder;
}
