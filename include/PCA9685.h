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

#include <cstdint>

/**
 * Datasheet
 * https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf
 */

namespace PCA9685Lib {

enum class PowerMode : std::uint8_t {
    NORMAL = 0,
    LOW_POWER = 1
};

enum class AutoIncrement : std::uint8_t {
    DISABLED = 0,
    ENABLED = 1
};

//OUTNE[1:0] registers
enum class OutputEnableMode : std::uint8_t {
    OFF = 0,
    FOLLOW_OUTDRV = 1,
    HIGH_IMPEDENCE = 2,
    HIGH_IMPEDENCE_2 = 3
};

enum class OutputDriverMode : std::uint8_t {
    OPEN_DRAIN = 0,
    TOTEM_POLE = 1
};

enum class OutputChangeMode : std::uint8_t {
    STOP = 0,
    ACK = 1
};

enum class OutputLogicState : std::uint8_t {
    NOT_INVERTED = 0,
    INVERTED = 1
};

struct ModeRegister1 {

public:

    ModeRegister1(const std::uint8_t val = 0b00010001) noexcept :
        _val(val) {
    }

    bool getALLCALL() const noexcept {
        return static_cast<bool>(this->_val & 1);
    }

    void setALLCALL(const bool respond) noexcept {
        this->_val |= static_cast<std::uint8_t>(respond);
    }

    bool getSUB3() const noexcept {
        return static_cast<bool>((this->_val >> 1) & 1);
    }

    void setSUB3(const bool respond) noexcept {
        this->_val |= static_cast<std::uint8_t>(respond) << 1;
    }

    bool getSUB2() const noexcept {
        return static_cast<bool>((this->_val >> 2) & 1);
    }

    void setSUB2(const bool respond) noexcept {
        this->_val |= static_cast<std::uint8_t>(respond) << 2;
    }

    bool getSUB1() const noexcept {
        return static_cast<bool>((this->_val >> 3) & 1);
    }

    void setSUB1(const bool respond) noexcept {
        this->_val |= static_cast<std::uint8_t>(respond) << 3;
    }

    PowerMode getSLEEP() const noexcept {
        return static_cast<PowerMode>((this->_val >> 4) & 1);
    }

    void setSLEEP(const PowerMode pm) noexcept {
        this->_val |= static_cast<std::uint8_t>(pm) << 4;
    }

    AutoIncrement getAI() const noexcept {
        return static_cast<AutoIncrement>((this->_val >> 5) & 1);
    }

    void setAI(const AutoIncrement ai) noexcept {
        this->_val |= static_cast<std::uint8_t>(ai) << 5;
    }

    bool getEXTCLK() const noexcept {
        return static_cast<bool>((this->_val >> 6) & 1);
    }

    void setEXTCLK(const bool use) noexcept {
        this->_val |= static_cast<std::uint8_t>(use) << 6;
    }

    bool getRESTART() const noexcept {
        return static_cast<bool>((this->_val >> 7) & 1);
    }

    void setRESTART() const noexcept {
        //datasheet pg. 14
        //only a 1 bit is permitted; 0 has no effect
        this->_val |= (1 >> 7) & 1;
    }

    std::uint8_t getValue() const noexcept {
        return this->_val;
    }


protected:
    std::uint8_t _val;

};

struct ModeRegister2 {

public:

    ModeRegister2(const std::uint8_t val = 0b00000100) noexcept :
        _val(val) {
    }

    OutputEnableMode getOUTNE() const noexcept {
        return static_cast<OutputEnableMode>(this->_val & 2);
    }

    void setOUTNE(const OutputEnableMode m) const noexcept {
        this->_val |= static_cast<std::uint8_t>(m);
    }

    OutputDriverMode getOUTDRV() const noexcept {
        return static_cast<OutputDriverMode>((this->_val >> 2) & 1);
    }

    void setOUTDRV(const OutputDriverMode m) const noexcept {
        this->_val |= static_cast<std::uint8_t>(m) << 2;
    }

    OutputChangeMode getOCH() const noexcept {
        return static_cast<OutputChangeMode>((this->_val >> 3) & 1);
    }

    void setOCH(const OutputChangeMode m) const noexcept {
        this->_val |= static_cast<std::uint8_t>(m) << 3;
    }

    OutputLogicState getINVRT() const noexcept {
        return static_cast<OutputLogicState>((this->_val >> 4) & 1);
    }

    void setINVRT(const OutputLogicState s) const noexcept {
        this->_val |= static_cast<std::uint8_t>(s) << 4;
    }

    std::uint8_t getValue() const noexcept {
        return this->_val;
    }


protected:
    std::uint8_t _val;

};

struct LedRegister {

protected:
    std::uint8_t _base;

public:
    explicit LedRegister(const std::uint8_t base) noexcept :
        _base(base) {
    }

    std::uint8_t getOnL() const noexcept {
        return this->_base;
    }

    std::uint8_t getOnH() const noexcept {
        return this->_base + 1;
    }

    std::uint8_t getOffL() const noexcept {
        return this->_base + 2;
    }

    std::uint8_t getOffH() const noexcept {
        return this->_base + 3;
    }


};

struct Channel {

protected:
    std::uint8_t _onLow;
    std::uint8_t _onHigh;
    std::uint8_t _offLow;
    std::uint8_t _offHigh;

public:

    Channel() noexcept :
        _onLow(0),
        _onHigh(0),
        _offLow(0),
        _offHigh(0) {
    }

    Channel(
        const std::uint8_t onH,
        const std::uint8_t onL,
        const std::uint8_t offH,
        const std::uint8_t offL) noexcept :
            _onHigh(onH),
            _onLow(onL),
            _offHigh(offH),
            _offLow(offL) {
        }

    unsigned short getOn() const noexcept {
        return static_cast<unsigned short>(this->_onHigh & 0b00111111) |
            static_cast<unsigned short>(this->_onLow);
    }

    unsigned short getOff() const noexcept {
        return static_cast<unsigned short>(this->_offHigh * 0b00111111) |
            static_cast<unsigned short>(this->_offLow);
    }

    void setOn(const unsigned short on) noexcept {
        this->_onHigh = (on >> 8) & 0b00111111;
        this->_onLow = on & 0xff;
    }

    void setOn(const std::uint8_t h, const std::uint8_t l) noexcept {
        this->setOn(
            (static_cast<unsigned short>(h & 0b00111111) << 8) |
            static_cast<unsigned short>(l));
    }

    void setOff(const unsigned short off) noexcept {
        this->_offHigh = (off >> 8) & 0b00111111;
        this->_offLow = off & 0xff;
    }

    void setOff(const std::uint8_t h, const std::uint8_t l) noexcept {
        this->setOff(
            (static_cast<unsigned short>(h & 0b00111111) << 8) |
            static_cast<unsigned short>(l));
    }

    void setOnH(const std::uint8_t h) noexcept {
        this->_onHigh = h;
    }

    void setOnL(const std::uint8_t l) noexcept {
        this->_onLow = l;
    }

    void setOffH(const std::uint8_t h) noexcept {
        this->_offHigh = h;
    }

    void setOffL(const std::uint8_t l) noexcept {
        this->_offLow = l;
    }

};

class PCA9685 {

public:

    static const std::uint8_t MODE1_REGISTER = 0x00;
    static const std::uint8_t MODE2_REGISTER = 0x01;
    static const std::uint8_t LED_BASE_REGISTER = 0x06;
    static const std::uint8_t ALL_LED_BASE_REGISTER = 0xfa;
    static const std::uint8_t PRE_SCALE_REGISTER = 0xfe;

    static const unsigned int OSCILLATOR_HZ = 25000000;
    static const std::uint8_t MIN_PRESCALE = 3;
    static const std::uint8_t MAX_PRESCALE = 255;

    static const int DEFAULT_I2C_DEVICE = 0;
    static const int DEFAULT_I2C_ADDRESS = 0x40;
    static const int DEFAULT_I2C_FLAGS = 0;

    //datasheet pg. 28
    static const int SWRST_I2C_ADDRESS = 0x0;
    static const int SWRST_I2C_FLAGS = 0;
    static const std::uint8_t SWRST_BYTE = 0x06;

    static const int LED_ALL_CALL_I2C_ADDRESS = 0xe0;


    PCA9685(
        const int device = DEFAULT_I2C_DEVICE,
        const int address = DEFAULT_I2C_ADDRESS,
        const int flags = DEFAULT_I2C_FLAGS) noexcept;

    virtual ~PCA9685();

    void connect();
    void disconnect();

    Channel getChannel(const std::uint8_t channel);
    Channel getAllChannels();
    unsigned int getFrequency();

    void setChannelOn(const std::uint8_t channel);
    void setChannelOff(const std::uint8_t channel);
    void setChannelPWM(const std::uint8_t channel, const std::uint16_t pwm);

    void setAllChannelsOn();
    void setAllChannelsOff();
    void setAllChannelsPWM(const std::uint16_t pwm);

    std::uint16_t getChannelPWM(const std::uint8_t channel);


    void setAllChannels(const unsigned short on, const unsigned short off);
    void setFrequency(const unsigned int hz);

    void enableSubAddress(const std::uint8_t addrNum, const std::uint8_t addr);
    void disableSubAddress(const std::uint8_t addrNum);

    void enableAllCallAddress(const std::uint8_t addr);
    void disableAllCallAddress();

    static void reset_all(const int device = DEFAULT_I2C_DEVICE);


protected:
    int _handle;
    int _device;
    int _address;
    int _flags;

    std::uint8_t _readReg(const std::uint8_t reg);
    void _writeReg(const std::uint8_t reg, const std::uint8_t val);

    /**
     * 
     * @param  {int} osc_clock   : Hz (max is 50MHz - ext clock only, default internal oscillator is 25MHz)
     * @param  {int} update_rate : Hz (range is 3 (max) to 256 (min))
     * @return {std::uint8_t}    : 
     */
    std::uint8_t _prescale_value(const unsigned int osc_clock, const unsigned int update_rate) noexcept;

    LedRegister _getLedRegister(const std::uint8_t channel) const noexcept;
    
    ModeRegister1 _getModeRegister1() const;
    ModeRegister2 _getModeRegister2() const;

    void _setModeRegister1(const ModeRegister1 m1) const;
    void _setModeRegister2(const ModeRegister2 m2) const;


};
};
#endif
