/**
 *   libBMPP - bwpixel.cpp
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

#include <pixels/bwpixel.hpp>

using namespace bmp;

/**
 * @function BWPixel
 * @description: BWPixel class constructor
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
**/

BWPixel::BWPixel(uint8_t value) {
  this->value = value > 0 ? 1 : 0;
}

/**
 * @function setPixel
 * @description: Set pixel RGB value
 * @param uint8_t
 * @param uint8_t
 * @param uint8_t
**/

void BWPixel::setPixel(uint8_t value) {
  this->value = value > 0 ? 1 : 0;
}

/**
 * @function getValue
 * @description returns color (1:0) value
 * @returns uint8_t
**/

uint8_t BWPixel::getValue() {
  return value;
}
