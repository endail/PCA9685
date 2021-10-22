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
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <lgpio.h>
#include <stdexcept>
#include <thread>

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

Channel PCA9685::getChannel(const std::uint8_t channel) {

    const auto reg = _getLedRegister(channel);

    const auto onH = ::lgI2cReadByteData(this->_handle, reg.getOnH());
    const auto onL = ::lgI2cReadByteData(this->_handle, reg.getOnL());
    const auto offH = ::lgI2cReadByteData(this->_handle, reg.getOffH());
    const auto offL = ::lgI2cReadByteData(this->_handle, reg.getOffL());

    if(onH < 0 || onL < 0 || offH < 0 || offL < 0) {
        throw std::runtime_error("failed to read channel data");
    }

    return Channel(
        static_cast<std::uint8_t>(onH),
        static_cast<std::uint8_t>(onL),
        static_cast<std::uint8_t>(offH),
        static_cast<std::uint8_t>(offL));

}

Channel PCA9685::getAllChannels() {

    const auto onH = ::lgI2cReadByteData(this->_handle, ALL_LED_BASE_REGISTER);
    const auto onL = ::lgI2cReadByteData(this->_handle, ALL_LED_BASE_REGISTER + 1);
    const auto offH = ::lgI2cReadByteData(this->_handle, ALL_LED_BASE_REGISTER + 2);
    const auto offL = ::lgI2cReadByteData(this->_handle, ALL_LED_BASE_REGISTER + 3);

    if(onH < 0 || onL < 0 || offH < 0 || offL < 0) {
        throw std::runtime_error("failed to read channel data");
    }

    return Channel(
        static_cast<std::uint8_t>(onH),
        static_cast<std::uint8_t>(onL),
        static_cast<std::uint8_t>(offH),
        static_cast<std::uint8_t>(offL));

}

unsigned int PCA9685::getFrequency() {
    return 0;
    //https://github.com/adafruit/Adafruit_CircuitPython_PCA9685/blob/2ee578ab813da74d0947741a22d92d2ab8ebe62d/adafruit_pca9685.py#L134
}

void PCA9685::setChannel(const std::uint8_t channel, const unsigned short on, const unsigned short off) {
    ::lgI2cWriteByteData(this->_handle, LED0_ON_L + 4 * channel, on & 0xff);
    ::lgI2cWriteByteData(this->_handle, LED0_ON_H + 4 * channel, on >> 8);
    ::lgI2cWriteByteData(this->_handle, LED0_OFF_L + 4 * channel, off & 0xff);
    ::lgI2cWriteByteData(this->_handle, LED0_OFF_H + 4 * channel, off >> 8);
}

void setAllChannels(const unsigned short on, const unsigned short off) {
    ::lgI2cWriteByteData(this->_handle, ALL_LED_BASE_REGISTER, on & 0xff);
    ::lgI2cWriteByteData(this->_handle, ALL_LED_BASE_REGISTER + 1, on >> 8);
    ::lgI2cWriteByteData(this->_handle, ALL_LED_BASE_REGISTER + 2, off & 0xff);
    ::lgI2cWriteByteData(this->_handle, ALL_LED_BASE_REGISTER + 3, off >> 8);
}

void setFrequency(const unsigned int hz) {

    //Writes to PRE_SCALE register are blocked when SLEEP bit is logic 0 (MODE 1).
    //-> cannot change frequency while in normal power mode
    //must sleep first, change frequency, then unsleep
    //ie. https://github.com/adafruit/Adafruit_Python_PCA9685/blob/master/Adafruit_PCA9685/PCA9685.py#L94-L100
    //The PRE_SCALE register can only be set when the SLEEP bit of MODE1 register is set to
    //logic 1.

    const auto prescale = _prescale_value(OSCILLATOR_HZ, hz);
    auto m1 = this->_getModeRegister1();

    m1.setSLEEP(PowerMode::LOW_POWER);
    this->_setModeRegister1(m1);

    const auto code = ::lgI2cWriteByteData(
        this->_handle,
        PRE_SCALE_REGISTER,
        prescale);

    if(code < 0) {
        throw std::runtime_error("failed to set prescale value");
    }

    m1.setSLEEP(PowerMode::NORMAL);
    this->_setModeRegister1(m1);

    std::this_thread::sleep_for(std::chrono::microseconds(500));

    m1.setRESTART();
    this->_setModeRegister1(m1);

}

static void PCA9685::reset_all(const int device) {

    //datasheet pg. 28

    const auto handle = ::lgI2cOpen(
        device,
        SWRST_ADDRESS,
        SWRST_I2C_FLAGS);

    const auto code = ::lgI2cWriteByte(
        handle,
        SWRST_BYTE);

    ::lgI2cClose(handle);

    if(code != 0) {
        throw std::runtime_error("PCA9685 did not ack reset");
    }

}

std::uint8_t PCA9685::_readReg(const std::uint8_t reg) {

    const auto b = ::lgI2cReadByteData(this->_handle, reg);

    if(b < 0) {
        throw std::runtime_error("failed to read register");
    }

    return b;

}

void PCA9685::_writeReg(const std::uint8_t reg, const std::uint8_t val) {

    const auto code = ::lgI2cWriteByteData(this->_handle, reg, val);

    if(code < 0) {
        throw std::runtime_error("failed to write register");
    }

}

std::uint8_t PCA9685::_prescale_value(const unsigned int osc_clock, const unsigned int update_rate) noexcept {
    
    //datasheet pg. 25
    const int uncapped = std::round(
        static_cast<double>(osc_clock) / (4096 * update_rate)) - 1);
    
    return static_cast<std::uint8_t>(
        std::max(std::min(prescale, MAX_PRESCALE), MIN_PRESCALE));

}

LedRegister PCA9685::_getLedRegister(const std::uint8_t channel) const noexcept {
    return LedRegister(LED_BASE_REGISTER + (channel * 4));
}

ModeRegister1 PCA9685::_getModeRegister1() const {
    return ModeRegister1(this->_readReg(MODE1_REGISTER));
}

ModeRegister2 PCA9685::_getModeRegister2() const {
    return ModeRegister2(this->_readReg(MODE2_REGISTER));
}

void PCA9685::_setModeRegister1(const ModeRegister1 m1) const {
    this->_writeReg(MODE1_REGISTER, m1.getValue());
}

void PCA9685::_setModeRegister2(const ModeRegister2 m2) const {
    this->_writeReg(MODE2_REGISTER, m2.getValue());
}

};
