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

#ifndef PCA9685_H_0D1184C8_B49E_455E_9343_0CE072650263
#define PCA9685_H_0D1184C8_B49E_455E_9343_0CE072650263

/**
 * Datasheet
 * https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf
 */

namespace PCA9685Lib {
class PCA9685 {

public:

    static const int DEFAULT_DEVICE = 0;
    static const int DEFAULT_ADDRESS = 0x40;
    static const int DEFAULT_FLAGS = 0;

    PCA9685(
        const int device = DEFAULT_DEVICE,
        const int address = DEFAULT_ADDRESS,
        const int flags = DEFAULT_FLAGS) noexcept;

    virtual ~PCA9685();
    void connect();
    void disconnect();

    


protected:
    int _handle;
    int _device;
    int _address;
    int _flags;

};
};
#endif
