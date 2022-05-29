// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup Firmware
 * @file circuits/74hc595.hpp
 * @brief Header of the 74HC595 Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CIRCUITS_74HC595_HPP_
#define CIRCUITS_74HC595_HPP_

#include <vector>

#include "pico/stdlib.h"
#include "hal/gpio.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief 74xx595 Shift Register Class.
 * @details The purpose of this class is to interface a 74xx595 
 *  Shift Register. 
 *  Datasheet available on: https://tinyurl.com/5crbkb52.
 * @nosubgrouping
 */
class HC595 {
 public:
    /** @brief Type of Data buffer. */
    typedef std::vector<uint8_t> TData;
    /** @brief Default value to pulse time, in microseconds. */
    static constexpr uint kDefaultPulseTime = 1;
    /** @brief Constructor. */
    HC595();
    /**
     * @brief Constructor.
     * @details Constructs a new HC595 class object, and configures it.
     * @param sinPin GPIO Pin number of Serial Data Input.
     * @param clkPin GPIO Pin number of Data Clock
     *  (default = 0xFF, no pin).
     * @param clrPin GPIO Pin number of ~Clear
     *  (default = 0xFF, no pin). 
     * @param rckPin GPIO Pin number of Latch Clock
     *  (default = 0xFF, no pin).
     * @param oePin GPIO Pin number of Output Enable
     *  (default = 0xFF, no pin).
     * @param pulseTime Pulse time, in microseconds
     *  (default = kDefaultPulseTime).
     */
    HC595(uint sinPin, uint clkPin = 0xFF, uint clrPin = 0xFF,
          uint rckPin = 0xFF, uint oePin = 0xFF,
          uint pulseTime = kDefaultPulseTime);
    /**
     * @brief Configures the HC595 class object.
     * @param sinPin GPIO Pin number of Serial Data Input
     *  (default = 0xFF, no pin).
     * @param clkPin GPIO Pin number of Data Clock
     *  (default = 0xFF, no pin).
     * @param clrPin GPIO Pin number of ~Clear
     *  (default = 0xFF, no pin). 
     * @param rckPin GPIO Pin number of Latch Clock
     *  (default = 0xFF, no pin).
     * @param oePin GPIO Pin number of Output Enable
     *  (default = 0xFF, no pin).
     * @param pulseTime Pulse time, in microseconds
     *  (default = kDefaultPulseTime).
     */
    void configure(uint sinPin = 0xFF, uint clkPin = 0xFF, uint clrPin = 0xFF,
                   uint rckPin = 0xFF, uint oePin = 0xFF,
                   uint pulseTime = kDefaultPulseTime);
    /** @brief Clears the output of the HC595. */
    void clear();
    /**
     * @brief Changes the Output Enable pin of the HC595.
     * @param value If true, activate OE. 
     *  Else, disables the output (put its in tristate).
     */
    void outputEnable(bool value = true);
    /** @brief Disables the output (put its in tristate). */
    void outputDisable(void);
    /**
     * @brief Writes a byte to output of the HC595.
     * @param value Value to write (bits 0..7).
     */
    void writeByte(uint8_t value);
    /**
     * @brief Writes a word to outputs of two HC595
     *  (in cascade).
     * @param value Value to write (bits 0..15).
     */
    void writeWord(uint16_t value);
    /**
     * @brief Writes a double word to outputs of quad HC595
     *  (in cascade).
     * @param value Value to write (bits 0..31).
     */
    void writeDWord(uint32_t value);
    /**
     * @brief Writes data to outputs of "n" HC595
     *  (arbitrary units in cascade).
     * @param buffer Pointer to values to write.
     * @param size Size of buffer.
     */
    void writeData(const uint8_t* buffer, uint size);
    /**
     * @brief Sets a bit of any HC595 in cascade.
     * @param bit Number of bit (0..n).
     * @param value Value to set (default = true).
     */
    void setBit(uint bit, bool value = true);
    /**
     * @brief Resets a bit of any HC595 in cascade.
     * @param bit Number of bit (0..n).
     */
    void resetBit(uint bit);
    /**
     * @brief Toggles a bit of any HC595 in cascade.
     * @param bit Number of bit (0..n).
     */
    void toggleBit(uint bit);
    /**
     * @brief Gets the data written in HC595 units.
     * @return Reference to "software stored" data buffer.
     */
    const TData& getData(void) const;
    /**
     * @brief Gets a bit of data written in HC595 units.
     * @param bit Number of bit (0..n).
     * @return Value of bit ("software stored").
     */
    const bool getBit(uint bit) const;
    /**
     * @brief Gets the Output Enable status. 
     * @return Value of Output Enable ("software stored").
     */
    const bool getOE(void) const;

 private:
    /* @brief Pulse time, in microseconds. */
    uint pulseTime_;
    /* @brief ~OE GPIO pin number. */
    uint oePin_;
    /* @brief SIN GPIO pin number. */
    uint sinPin_;
    /* @brief CLK GPIO pin number. */
    uint clkPin_;
    /* @brief ~CLR GPIO pin number. */
    uint clrPin_;
    /* @brief RCK GPIO pin number. */
    uint rckPin_;
    /* @brief GPIO handler. */
    Gpio gpio_;
    /* @brief Current data buffer. */
    TData buffer_;
    /* @brief Current OE pin status. */
    bool oe_;
};

#endif  // CIRCUITS_74HC595_HPP_
