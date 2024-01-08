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
 * @file circuits/74hc165.hpp
 * @brief Header of the 74HC165 Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CIRCUITS_74HC165_HPP_
#define CIRCUITS_74HC165_HPP_

#include <vector>

#include "pico/stdlib.h"
#include "hal/gpio.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief 74xx165 Shift Register Class.
 * @details The purpose of this class is to interface a 74xx165
 *  Shift Register.
 *  Datasheet available on: https://tinyurl.com/bdzmty8v.
 * @nosubgrouping
 */
class HC165 {
  public:
    /** @brief Type of Data buffer. */
    typedef std::vector<bool> TData;
    /** @brief Default value to pulse time, in microseconds. */
    static constexpr uint kDefaultPulseTime = 1;
    /** @brief Constructor. */
    HC165();
    /**
     * @brief Constructor.
     * @details Constructs a new HC165 class object, and configures it.
     * @param plPin GPIO Pin number of ~Parallel Load.
     * @param clkPin GPIO Pin number of Data Clock
     *  (default = 0xFF, no pin).
     * @param cePin GPIO Pin number of ~Chip Enable
     *  (default = 0xFF, no pin).
     * @param q7Pin GPIO Pin number of Q7
     *  (default = 0xFF, no pin).
     * @param nq7Pin GPIO Pin number of ~Q7
     *  (default = 0xFF, no pin).
     * @param pulseTime Pulse time, in microseconds
     *  (default = kDefaultPulseTime).
     */
    HC165(uint plPin, uint clkPin = 0xFF, uint cePin = 0xFF, uint q7Pin = 0xFF,
          uint nq7Pin = 0xFF, uint pulseTime = kDefaultPulseTime);
    /**
     * @brief Configures the HC165 class object.
     * @param plPin GPIO Pin number of ~Parallel Load
     *  (default = 0xFF, no pin).
     * @param clkPin GPIO Pin number of Data Clock
     *  (default = 0xFF, no pin).
     * @param cePin GPIO Pin number of ~Chip Enable
     *  (default = 0xFF, no pin).
     * @param q7Pin GPIO Pin number of Q7
     *  (default = 0xFF, no pin).
     * @param nq7Pin GPIO Pin number of ~Q7
     *  (default = 0xFF, no pin).
     * @param pulseTime Pulse time, in microseconds
     *  (default = kDefaultPulseTime).
     */
    void configure(uint plPin = 0xFF, uint clkPin = 0xFF, uint cePin = 0xFF,
                   uint q7Pin = 0xFF, uint nq7Pin = 0xFF,
                   uint pulseTime = kDefaultPulseTime);
    /**
     * @brief Changes the Chip Enable pin of the HC165.
     * @param value If true, activate CE.
     *  Else, disables the chip.
     */
    void chipEnable(bool value = true);
    /** @brief Disables the chip. */
    void chipDisable(void);
    /**
     * @brief Gets the Chip Enable status.
     * @return Value of Chip Enable ("software stored").
     */
    const bool getCE(void) const;
    /** @brief Loads data into HC165. */
    void load();
    /**
     * @brief Gets a bit of the data loaded in HC165 units.
     *   Shifts data in units if need.
     * @param index Index of bit (0..n). Default = zero.
     * @return Value of bit.
     */
    bool getBit(uint index = 0);
    /**
     * @brief Reads a byte of the data loaded in HC165 units.
     *   Shifts data in units if need.
     * @param reverse If true, reverses the bit order (Q0 to Q7).
     *   Otherwise (default), uses unit bit order (Q7 to Q0).
     * @return Value of byte (bits 0..7).
     */
    uint8_t readByte(bool reverse = false);
    /**
     * @brief Reads a word of the data loaded in HC165 units.
     *   Shifts data in units if need.
     * @param reverse If true, reverses the bit order (Q0 to Q7).
     *   Otherwise (default), uses unit bit order (Q7 to Q0).
     * @return Value of word (bits 0..15).
     */
    uint16_t readWord(bool reverse = false);
    /**
     * @brief Reads a double word of the data loaded in HC165 units.
     *   Shifts data in units if need.
     * @param reverse If true, reverses the bit order (Q0 to Q7).
     *   Otherwise (default), uses unit bit order (Q7 to Q0).
     * @return Value of word (bits 0..31).
     */
    uint32_t readDWord(bool reverse = false);
    /**
     * @brief Reads data from the arbitrary HC165 units in cascade.
     *   Shifts data in units if need.
     * @param buffer Pointer to buffer to receive data.
     *   First is the LSB byte.
     * @param size Size of buffer.
     * @param reverse If true, reverses the bit order (Q0 to Q7).
     *   Otherwise (default), uses unit bit order (Q7 to Q0).
     * @return Size of readed data.
     */
    uint readData(uint8_t* buffer, uint size, bool reverse = false);

  private:
    /* @brief GPIO handler. */
    Gpio gpio_;
    /* @brief ~CE GPIO pin number. */
    uint cePin_;
    /* @brief ~PL GPIO pin number. */
    uint plPin_;
    /* @brief CLK GPIO pin number. */
    uint clkPin_;
    /* @brief Q7 GPIO pin number. */
    uint q7Pin_;
    /* @brief ~Q7 GPIO pin number. */
    uint nq7Pin_;
    /* @brief Pulse time, in microseconds. */
    uint pulseTime_;
    /* @brief Current CE pin status. */
    bool ce_;
};

#endif  // CIRCUITS_74HC165_HPP_
