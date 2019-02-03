/**
 *   libBMP24 - main.cpp
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

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cout << "Usage: bmpTest <bmpfile> <outBmpFile> [operation] [operationArg]" << std::endl;
    std::cout << "Operations are:" << std::endl;
    std::cout << "0: print pixels" << std::endl;
    std::cout << "1: toGreyScale()" << std::endl;
    std::cout << "2: rotate()" << std::endl;
    return 1;
  }

  std::string bmpFilename = argv[1];
  std::string outFilename = argv[2];
  int command = -1;
  int commandArg = 0;
  if (argc >= 4) {
    command = std::stoi(std::string(argv[3]));
  }
  if (argc >= 5) {
    commandArg = std::stoi(std::string(argv[4]));
  }

  std::ifstream bmpFile;
  bmpFile.open(bmpFilename, std::ios::binary | std::ios::ate);
  std::streamsize size = bmpFile.tellg();
  bmpFile.seekg(0, std::ios::beg);

  char* dataBuffer = new char[size];
  if (!bmpFile.read(dataBuffer, size)) {
    std::cout << "Could not read file " << bmpFilename << std::endl;
    delete[] dataBuffer;
    return 1;
  }

  bmp::Bmp24* myBmp = new bmp::Bmp24();
  //Decode bitmap
  if (!myBmp->decodeBmp(reinterpret_cast<uint8_t*>(dataBuffer), size)) {
    std::cout << "Could not decode bitmap" << std::endl;
    delete myBmp;
    return 1;
  }

  std::cout << "Bitmap decoded successfully (size: " << size << ")" << std::endl;

  std::cout << "Bitmap size(width: " << myBmp->getWidth() << "; height: " << myBmp->getHeight() << ")" << std::endl;

  //Print RGB for each pixel

  switch (command) {
  case 0: {
    size_t width = myBmp->getWidth();
    size_t height = myBmp->getHeight();
    size_t pixelAmount = width * height;
    for (size_t pixel = 0; pixel < pixelAmount; pixel++) {
      bmp::RGBPixel* currPixel = myBmp->getPixelAt(pixel);
      if (currPixel == nullptr) {
        std::cout << "Out of range\n";
        continue;
      }
      std::cout << "Pixel[" << pixel << "]: (" << std::to_string(currPixel->getRed()) << "," << std::to_string(currPixel->getGreen()) << "," << std::to_string(currPixel->getBlue()) << ");" << std::endl;
    }
    break;
  }
  case 1:
    std::cout << "Applying: toGreyScale(" << commandArg << ")\n";
    myBmp->toGreyScale(commandArg);
    break;
  case 2:
    std::cout << "Applying: rotate(" << commandArg << ")\n";
    myBmp->rotate(commandArg);
  default:
    break;
  }

  //re-Encode BMP
  size_t outDataSize;
  uint8_t* newBmp = myBmp->encodeBmp(&outDataSize);
  //Write file
  std::ofstream outFile;
  outFile.open(outFilename);
  if (!outFile.is_open()) {
    std::cout << "Could not open file " << outFilename << std::endl;
    delete[] newBmp;
    delete[] dataBuffer;
    delete myBmp;
    return 1;
  }
  for (size_t i = 0; i < outDataSize; i++) {
    outFile << newBmp[i];
  }
  std::cout << "\nOutDataSize: " << outDataSize << std::endl;
  outFile.close();

  delete[] newBmp;
  delete[] dataBuffer;
  delete myBmp;
  return 0;
}
