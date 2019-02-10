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
#include <sstream>
#include <string>

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}


int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cout << "Usage: bmpTest <bmpfile> <outBmpFile> [operation] [cmdArg1,cmdArg2,...,cmdArgN]" << std::endl;
    std::cout << "Operations are:" << std::endl;
    std::cout << "0: print pixels" << std::endl;
    std::cout << "1: rotate()" << std::endl;
    std::cout << "2: flip('V')" << std::endl;
    std::cout << "3: flip('H')" << std::endl;
    std::cout << "4: resizeArea(arg1, arg2)" << std::endl;
    std::cout << "5: toGreyScale(arg1)" << std::endl;
    std::cout << "6: toSepiaTone()" << std::endl;
    return 1;
  }

  std::string bmpFilename = argv[1];
  std::string outFilename = argv[2];
  int command = -1;
  std::vector<std::string> commandArgs;
  if (argc >= 4) {
    command = std::stoi(std::string(argv[3]));
  }
  if (argc >= 5) {
    commandArgs = split(std::string(argv[4]), ',');
  }

  do {

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
  case 1: {
    int commandArg = std::stoi(commandArgs.at(0));
    std::cout << "Applying: rotate(" << commandArg << ")\n";
    myBmp->rotate(commandArg);
    break;
  }
  case 2:
    std::cout << "Applying: flip('V')\n";
    myBmp->flipVertical();
    break;
  case 3:
    std::cout << "Applying: flip('H')\n";
    myBmp->flipHorizontal();
    break;
  case 4: {
    size_t width = std::stoi(commandArgs.at(0));
    size_t height = std::stoi(commandArgs.at(1));
    size_t xOffset = 0;
    size_t yOffset = 0;
    if (commandArgs.size() >= 4) {
      xOffset = std::stoi(commandArgs.at(2));
      yOffset = std::stoi(commandArgs.at(3));
    }
    std::cout << "Applying: resizeArea(" << width << "," << height << "," << xOffset << "," << yOffset << ");\n";
    myBmp->resizeArea(width, height, xOffset, yOffset);
    break;
  }
  case 5: {
    int commandArg = std::stoi(commandArgs.at(0));
    std::cout << "Applying: toGreyScale(" << commandArg << ")\n";
    myBmp->toGreyScale(commandArg);
    break;
  }
  case 6: {
    std::cout << "Applying: toSepiaTone()\n";
    myBmp->toSepiaTone();
    break;
  }
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

  //Ask for next commands
  std::cout << "Usage: bmpTest <bmpfile> <outBmpFile> [operation] [cmdArg1;cmdArg2;...;cmdArgN]" << std::endl;
  std::cout << "Operations are:" << std::endl;
  std::cout << "0: print pixels" << std::endl;
  std::cout << "1: rotate()" << std::endl;
  std::cout << "2: flip('V')" << std::endl;
  std::cout << "3: flip('H')" << std::endl;
  std::cout << "4: resizeArea(arg1, arg2)" << std::endl;
  std::cout << "5: toGreyScale(arg1)" << std::endl;
  std::cout << "6: toSepiaTone()" << std::endl;
  std::cout << "bmpFile (QUIT to exit): ";
  std::cin >> bmpFilename;
  if (bmpFilename == "QUIT") {
    break;
  }
  std::cout << "outBmpFile: ";
  std::cin >> outFilename;
  std::string operationStr;
  std::cout << "Operation: ";
  std::cin >> operationStr;
  command = std::stoi(operationStr);
  std::string operationArg;
  std::cout << "OperationArgs: ";
  std::cin >> operationArg;
  commandArgs = split(operationArg, ',');

  } while (true);

  return 0;
}
