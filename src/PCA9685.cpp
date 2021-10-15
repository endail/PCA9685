// MIT License
//
// Copyright (c) 2021 Daniel Robertson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "../include/PCA9685.h"
#include <cstdint>
#include <lgpio.h>
#include <stdexcept>

namespace PCA9685Lib {

PCA9685::PCA9685(const int device, const int address, const int flags) noexcept :
    _handle(-1),
    _device(device),
    _address(address),
    _flags(flags) {
}

PCA9685::~PCA9685() {
    this->disconnect();
}

void PCA9685::connect() {

    if(this->_handle >= 0) {
        return;
    }

    if((this->_handle = ::lgI2cOpen(
        this->_device,
        this->_address,
        this->_flags)) < 0) {
            throw std::runtime_error("failed to connect to device");
    }

}

void PCA9685::disconnect() {

    if(this->_handle < 0) {
        return;
    }

    if(::lgI2cClose(this->_handle) < 0) {
        throw std::runtime_error("failed to disconnect device");
    }

    this->_handle = -1;

}





};
