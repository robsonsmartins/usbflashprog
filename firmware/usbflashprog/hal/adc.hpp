// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @file hal/adc.hpp
 * @brief Header of the Pico ADC Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_ADC_HPP_
#define HAL_ADC_HPP_

#include <set>

#include "pico/stdlib.h"

/**
 * @brief Pico ADC Class
 * @details The purpose of this class is to manipulate the ADC on the 
 *  board.
 */
class Adc {
 public:
    /** @brief Default reference voltage, in Volts. */
    static constexpr float DEFAULT_VREF = 3.3f;
    /** 
     * @brief Constructor.
     * @details As default, the reference voltage is set to
     *   DEFAULT_VREF value.
     */
    Adc();
    /**
     * @brief Constructor.
     * @param vref Reference voltage of the ADC, in Volts.
     */
    explicit Adc(float vref);
    /**
     * @brief Captures one sample from the ADC.
     * @details Captures and returns one sample.
     *   The valid channels are 0 to 3.
     * @param channel Number of the ADC channel (0 to 3).
     * @return Value of the sample, in Volts. 
     */
    float capture(uint channel);
    /**
     * @brief Captures multiple samples from the ADC.
     * @details Uses FIFO to capture samples from ADC.
     *   The valid channels are 0 to 3.
     * @param channel Number of the ADC channel (0 to 3).
     * @param size Number of samples to capture.
     * @return Mean of all sample values, in Volts. 
     */
    float capture(uint channel, size_t size);
    /**
     * @brief Captures multiple samples from the ADC.
     * @details Uses FIFO to capture samples from ADC.
     *   The valid channels are 0 to 3.
     * @param channel Number of the ADC channel (0 to 3).
     * @param buf Pointer to buffer that receive the captured samples.
     * @param size Number of samples to capture.
     * @return Mean of all sample values, in Volts. 
     */
    float capture(uint channel, float* buf, size_t size);

 private:
    /** @brief Current reference voltage. */
    float vref_;
    /** @brief Set of initialized channels. */
    std::set<uint> initChannels_;
    /**
     * @brief Initializes the channel.
     * @param channel Number of the ADC channel (0 to 3).
     * @return Bool if success. False otherwise.
     */
    bool initChannel_(uint channel);
    /**
     * @brief Calculate the real value, based in VRef. 
     * @param value ADC raw value (12 bits).
     * @return Real value, in Volts. 
     */
    float calculate_(uint16_t value);
};

#endif  // HAL_ADC_HPP_
