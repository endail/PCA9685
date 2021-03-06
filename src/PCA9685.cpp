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
#include <bitset>
#include <chrono>
#include <cstdint>
#include <lgpio.h>
#include <stdexcept>
#include <thread>

namespace PCA9685Lib {

Register::Register() noexcept :
    Register(0) { }

Register::Register(const std::uint8_t v) noexcept :
    std::bitset<8>(v) { }

Register::Register(const std::bitset<8>& bs) noexcept :
    Register(static_cast<std::uint8_t>(bs.to_ulong())) { }

std::uint8_t Register::toByte() const noexcept {
    return static_cast<std::uint8_t>(this->to_ulong());
}

PCA9685::PCA9685(const int device, const int address, const int flags) noexcept :
    _handle(-1),
    _device(device),
    _address(address),
    _flags(flags) {
}

PCA9685::~PCA9685() {
    
    try {
        this->disconnect();
    }
    catch(...) {
        //prevent propagation
    }

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

unsigned int PCA9685::getFrequency() {
    return 0;
    //https://github.com/adafruit/Adafruit_CircuitPython_PCA9685/blob/2ee578ab813da74d0947741a22d92d2ab8ebe62d/adafruit_pca9685.py#L134
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

    m1.setSleep(PowerMode::LOW_POWER);
    this->_setModeRegister1(m1);

    this->_writeReg(PRE_SCALE_REGISTER, prescale);

    m1.setSleep(PowerMode::NORMAL);
    this->_setModeRegister1(m1);

    //TODO: make this a non magic number
    std::this_thread::sleep_for(std::chrono::microseconds(500));

    m1.setRestart();
    this->_setModeRegister1(m1);

}

void PCA9685::setChannelOn(const std::uint8_t channel) {
    this->_writeChannelPWM(this->_getLedRegister(channel), PWM_FULL, 0);
}

void PCA9685::setChannelOff(const std::uint8_t channel) {
    this->_writeChannelPWM(this->_getLedRegister(channel), 0, PWM_FULL);
}

void PCA9685::setChannelPWM(const std::uint8_t channel, const std::uint16_t pwm) {
    
}

void PCA9685::getChannel(const std::uint8_t channel, std::uint16_t* const on, std::uint16_t* const off) {

    const auto reg = _getLedRegister(channel);

    const auto onH = this->_readReg(reg.getOnH());
    const auto onL = this->_readReg(reg.getOnL());
    const auto offH = this->_readReg(reg.getOffH());
    const auto offL = this->_readReg(reg.getOffL());

    if(onH < 0 || onL < 0 || offH < 0 || offL < 0) {
        throw std::runtime_error("failed to read channel data");
    }

    on = static_cast<std::uint16_t>(onH) << 8 |
        static_cast<std::uint16_t>(onL);

    off = static_cast<std::uint16_t>(offH) << 8 |
        static_cast<std::uint16_t>(offL);

}

void PCA9685::getAllChannels(std::uint16_t* const on, std::uint16_t* const off) {

    const auto onH = this->_readReg(ALL_LED_BASE_REGISTER);
    const auto onL = this->_readReg(ALL_LED_BASE_REGISTER + 1);
    const auto offH = this->_readReg(ALL_LED_BASE_REGISTER + 2);
    const auto offL = this->_readReg(ALL_LED_BASE_REGISTER + 3);

    if(onH < 0 || onL < 0 || offH < 0 || offL < 0) {
        throw std::runtime_error("failed to read channel data");
    }

    on = static_cast<std::uint16_t>(onH) << 8 |
        static_cast<std::uint16_t>(onL);

    off = static_cast<std::uint16_t>(offH) << 8 |
        static_cast<std::uint16_t>(offL);

}

std::uint16_t PCA9685::getChannelPWM(const std::uint8_t channel) {

}

void PCA9685::setAllChannelsOn() {

}

void PCA9685::setAllChannelsOff() {

}

void PCA9685::setAllChannelsPWM(const std::uint16_t pwm) {

}

void PCA9685::setAllChannels(const std::uint16_t on, const std::uint16_t off) {

}

void PCA9685::enableSub1Address(const std::uint8_t addr) {

}

void PCA9685::enableSub2Address(const std::uint8_t addr) {

}

void PCA9685::enableSub3Address(const std::uint8_t addr) {

}

void PCA9685::disableSub1Address() {

}

void PCA9685::disableSub2Address() {

}

void PCA9685::disableSub3Address() {

}

void PCA9685::enableAllCallAddress(const std::uint8_t addr) {

}

void PCA9685::disableAllCallAddress() {

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

void PCA9685::_writeChannelPWM(const LedRegister led, const std::uint16_t phaseBegin, const std::uint16_t phaseEnd) {
    this->_writeReg(led.getOnL(), static_cast<std::uint8_t>(phaseBegin & 0b11111111));
    this->_writeReg(led.getOnH(), static_cast<std::uint8_t>((phaseBegin >> 8) & 0b11111111));
    this->_writeReg(led.getOffL(), static_cast<std::uint8_t>(phaseEnd & 0b11111111));
    this->_writeReg(led.getOffH(), static_cast<std::uint8_t>((phaseEnd >> 8) & 0b11111111));
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
