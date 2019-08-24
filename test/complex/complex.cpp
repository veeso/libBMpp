/**
 *   libBMpp - main.cpp
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

//This test includes a set of complex operation on the library.
//Its purpose is to find runtime errors, memory leaks and benchmarks

#include <iostream>
#include <chrono>
#include <cstdlib>

#include <bmp24.hpp>

#define TEST_TEXT_1 "Read source bmp file and copy it to out bmp file"
#define TEST_TEXT_2 "Copy (always with constructor) the BMP and then write it to out file"
#define TEST_TEXT_3 "Copy the source BMP and then invert 255 times and rotate 255 times by 90 degrees"
#define TEST_TEXT_4 "Copy the source BMP and then convert to grey scale 254 times"
#define TEST_TEXT_5 "Copy the source BMP and then resize image it dividing by two until the size is 16x16"
#define TEST_TEXT_6 "Resize the same image to its original size, with the same steps"
#define TEST_TEXT_7 "Copy the source BMP and then resize the area by two until the size is 16x16"
#define TEST_TEXT_8 "Copy the source and alternate pixels with red/green/blue each two pixels"
#define TEST_TEXT_9 "Create an empty image with size 2048x2048 and randomly set its pixel"

#define PROGRAM_NAME "complex"
#define USAGE PROGRAM_NAME "<bmpFile> <outBmpFile>\n\
This program doesn't require any other argument\n\
The program will execute a set of operations; for each one the execution time will be provided\n\
The program will execute the following operation on a BMP24:\n\
\t" TEST_TEXT_1 "\n\
\t" TEST_TEXT_2 "\n\
\t" TEST_TEXT_3 "\n\
\t" TEST_TEXT_4 "\n\
\t" TEST_TEXT_5 "\n\
\t" TEST_TEXT_6 "\n\
\t" TEST_TEXT_7 "\n\
\t" TEST_TEXT_8 "\n\
\t" TEST_TEXT_9 "\n\
"

int roundToPow2(int toRound) {
  int currPow = 2;
  int pow = currPow;
  while (currPow <= toRound) {
    pow = currPow;
    currPow *= 2;
  }
  return pow;
}

int roundToPrevPow2(int toRound) {
  int pow = roundToPow2(toRound);
  return pow / 2;
}

int roundToNextPow2(int toRound) {
  int pow = roundToPow2(toRound);
  return pow * 2;
}

int main(int argc, char* argv[]) {
  
  if (argc < 3) {
    std::cout << USAGE << std::endl;
    return 1;
  }

  const std::string srcFile = argv[1];
  const std::string outFile = argv[2];

  int rc = 0;
  std::string input;
  unsigned long long tStart;
  unsigned long long tEnd;
  bmp::Bmp24* bmp = nullptr;
  bmp::Bmp24* copyBmp = nullptr;
  size_t width;
  size_t height;
  size_t imgSize;
  std::cout << "Test 1: '" << TEST_TEXT_1 "' - Type any key to start";
  std::getline(std::cin, input);
  //@! Test 1
  //Get tStart
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  bmp = new bmp::Bmp24();
  if (!bmp->readBmp(srcFile)) {
    std::cout << "Could not source BMP. Test 1 aborted" << std::endl;
    rc = 1;
    goto cleanup;
  }
  //Write back
  if (!bmp->writeBmp(outFile)) {
    std::cout << "Could not write BMP file to " << outFile << std::endl;
    rc = 1;
    goto cleanup;
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 1 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 2: '" << TEST_TEXT_2 "' - Type any key to start";
  std::getline(std::cin, input);
  //@! Test 2
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  copyBmp = new bmp::Bmp24(bmp);
  //Write back
  if (!copyBmp->writeBmp(outFile)) {
    std::cout << "Could not write BMP file to " << outFile << std::endl;
    rc = 1;
    goto cleanup;
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  delete copyBmp;
  std::cout << "Test 2 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 3: 'Copy the source BMP and then invert 255 times and rotate 255 times by 90 degrees' - Type any key to start";
  std::getline(std::cin, input);
  //@! Test 3
  copyBmp = new bmp::Bmp24(bmp);
  //Invert 255 times
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  for (int i = 0; i < 255; i++) {
    if (!copyBmp->invert()) {
      std::cout << "Invert attempt " << i << " failed; execution aborted." << std::endl;
      rc = 1;
      goto cleanup;
    }
    if (!copyBmp->rotate(90)) {
      std::cout << "Rotate attempt " << i << " failed; execution aborted." << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  delete copyBmp;
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 3 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 4: '" << TEST_TEXT_4 "' - Type any key to start";
  std::getline(std::cin, input);
  //@! Test 4
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  copyBmp = new bmp::Bmp24(bmp);
  for (int i = 255; i > 1; i--) {
    if (!copyBmp->toGreyScale(i)) {
      std::cout << "toGreyScale failed at attempt " << i << "; execution aborted" << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  delete copyBmp;
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 4 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 5: '" << TEST_TEXT_5 "' - Type any key to start";
  std::getline(std::cin, input);
  //@! Test 5
  //Make width = height NOTE: before test starts
  width = bmp->getWidth();
  height = bmp->getHeight();
  if (width > height) {
    if (!bmp->resizeArea(height, height)) {
      std::cout << "Could not resize area to make width and height equal" << std::endl;
      rc = 1;
      goto cleanup;
    }
  } else if (width < height) {
    if (!bmp->resizeArea(width, width)) {
      std::cout << "Could not resize area to make width and height equal" << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  copyBmp = new bmp::Bmp24(bmp);
  //Resize untile size is 16x16
  imgSize = copyBmp->getWidth(); //height and width are the same
  while ((imgSize = roundToPrevPow2(imgSize)) >= 16) { //While at least 16x16
    if (!copyBmp->resizeImage(imgSize, imgSize)) {
      std::cout << "resizeImage failed at attempt " << imgSize << "x" << imgSize << ". Execution aborted." << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 5 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 6: '" << TEST_TEXT_6 "' - Type any key to start";
  std::getline(std::cin, input);
  //@! Test 6
  while ((imgSize = roundToNextPow2(imgSize)) < width) {
    if (!copyBmp->resizeImage(imgSize, imgSize)) {
      std::cout << "resizeImage failed at attempt " << imgSize << "x" << imgSize << ". Execution aborted." << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  //To original size
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  if (imgSize != width && imgSize != height) {
    size_t minSize = (width < height) ? width : height;
    if (!copyBmp->resizeImage(minSize, minSize)) {
      std::cout << "resizeImage failed at attempt " << minSize << "x" << minSize << ". Execution aborted." << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 6 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 7: '" << TEST_TEXT_7 "' - Type any key to start";
  std::getline(std::cin, input);
  delete copyBmp;
  //@! Test 7
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  copyBmp = new bmp::Bmp24(bmp);
  //Resize untile size is 16x16
  if (width > height) {
    if (!bmp->resizeArea(height, height)) {
      std::cout << "Could not resize area to make width and height equal" << std::endl;
      rc = 1;
      goto cleanup;
    }
  } else if (width < height) {
    if (!bmp->resizeArea(width, width)) {
      std::cout << "Could not resize area to make width and height equal" << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  imgSize = copyBmp->getWidth(); //height and width are the same
  while ((imgSize = roundToPrevPow2(imgSize)) >= 16) { //While at least 16x16
    if (!copyBmp->resizeArea(imgSize, imgSize)) {
      std::cout << "resizeImage failed at attempt " << imgSize << "x" << imgSize << ". Execution aborted." << std::endl;
      rc = 1;
      goto cleanup;
    }
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 7 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 8: '" << TEST_TEXT_8 "' - Type any key to start";
  std::getline(std::cin, input);
  delete copyBmp;
  //@! Test 8
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  copyBmp = new bmp::Bmp24(bmp);
  for (size_t i = 0; i < height; i++) {
    uint8_t currColor[3] = {255, 0, 0};
    for (size_t j = 0; j < width; j += 2) {
      if (!copyBmp->setPixelAt(i, j, currColor[0], currColor[1], currColor[2])) {
        std::cout << "setPixelAt (" << i << "," << j << ") failed. Execution aborted" << std::endl;
        rc = 1;
        goto cleanup;
      }
      //Swap colors
      uint8_t tmp = currColor[2];
      currColor[2] = currColor[1];
      currColor[1] = currColor[0];
      currColor[0] = tmp;
    }
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 8 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;
  std::cout << "Test 9: '" << TEST_TEXT_9 "' - Type any key to start";
  std::getline(std::cin, input);
  delete copyBmp;
  copyBmp = nullptr;
  delete bmp;
  tStart = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  srand(tStart);
  bmp = new bmp::Bmp24(2048, 2048, 255, 255, 255);
  for (size_t i = 0; i < bmp->getHeight(); i++) {
    for (size_t j = 0; j < bmp->getWidth(); j++) {
      //Get rand color
      uint8_t red = rand() % 256;
      uint8_t green = rand() % 256;
      uint8_t blue = rand() % 256;
      if (!bmp->setPixelAt(i, j, red, green, blue)) {
        std::cout << "setPixelAt (" << i << "," << j << ") failed. Execution aborted" << std::endl;
        rc = 1;
        goto cleanup;
      }
    }
  }
  tEnd = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  std::cout << "Test 9 OK - Execution time: " << tEnd - tStart << "ns" << std::endl;

cleanup:
  if (bmp != nullptr)
    delete bmp;
  if (copyBmp != nullptr)
    delete copyBmp;
  return rc;
}
