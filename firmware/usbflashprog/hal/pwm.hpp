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
 * @file hal/pwm.hpp
 * @brief Header of the Pico PWM Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_PWM_HPP_
#define HAL_PWM_HPP_

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Pico PWM Class
 * @details The purpose of this class is to manipulate the PWM on the
 *  board.<br/>
 *  <b>Note</b>: Each instance of this class controls only one PWM pin.
 * @nosubgrouping
 */
class Pwm {
  public:
    /** @brief PWM default frequency, in Hertz. */
    static constexpr uint32_t kPwmDefaultFreq = 40'000UL;
    /**
     * @brief Constructor.
     * @details Creates a Pwm class instance with kPwmDefaultFreq frequency
     *  and zero duty cycle.
     * @param pin The pin number.
     */
    explicit Pwm(uint pin);
    /** @brief Destructor. */
    ~Pwm();
    /**
     * @brief Gets the pin number.
     * @return The pin number.
     */
    uint getPin() const;
    /**
     * @brief Gets the slice number of PWM.
     * @return The PWM slice number.
     */
    uint getSlice() const;
    /**
     * @brief Gets the PWM channel number.
     * @return The PWM channel number.
     */
    uint getChannel() const;
    /**
     * @brief Sets the PWM frequency.
     * @param freq PWM frequency, in Hertz.<br/>
     * <b>Note</b>: This value must be greater than zero and less than 6.25MHz.
     */
    void setFreq(uint32_t freq);
    /**
     * @brief Gets the PWM frequency.
     * @return The PWM frequency, in Hertz.
     */
    uint32_t getFreq() const;
    /**
     * @brief Sets the duty cycle of PWM.
     * @param duty Duty cycle, in percent (%).<br/>
     * <b>Note</b>: This value must be positive and less than/equal 100%.
     */
    void setDuty(float duty);
    /**
     * @brief Gets the duty cycle.
     * @return The duty cycle, in percent (%).
     */
    float getDuty() const;
    /**
     * @brief Enables the PWM on the pin.
     * @details Starts the output of the PWM signal on the pin.
     */
    void start();
    /**
     * @brief Disables the PWM on the pin.
     * @details Stops the output of the PWM signal on the pin, and
     *   puts its in low state.
     */
    void stop();
    /**
     * @brief Returns if the PWM is enabled on the pin.
     * @return True if the PWM is enabled, false otherwise.
     */
    bool isRunning() const;

  private:
    /* @brief Number of GPIO pin associated with this instance. */
    uint pin_;
    /* @brief Current PWM frequency (Hz). */
    uint32_t freq_;
    /* @brief Current PWM duty cycle (%). */
    float duty_;
    /* @brief Current PWM status flag. */
    bool running_;
    /* @brief Number of PWM slice associated with this instance. */
    uint slice_;
    /* @brief Number of PWM channel associated with this instance. */
    uint channel_;
    /*
     * @brief Value of frequency divider of this instance.
     * For internal use.
     */
    static float divider_;
};

#endif  // HAL_PWM_HPP_
