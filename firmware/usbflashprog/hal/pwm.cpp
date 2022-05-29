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
 * @file hal/pwm.cpp
 * @brief Implementation of the Pico PWM Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <vector>

#include "hal/pwm.hpp"

#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// ---------------------------------------------------------------------------

// typical choose for RP PWM module clock: 12.5 MHz
constexpr uint32_t kPwmDefaultClock = 12'500'000UL;

// clksys: typically 125 MHz
// let's arbitrarily choose to run pwm clock at kPwmDefaultClock
float Pwm::divider_ = clock_get_hz(clk_sys) / kPwmDefaultClock;

// ---------------------------------------------------------------------------

Pwm::Pwm(uint pin): pin_(pin), freq_(0), duty_(0.0f), running_(false) {
    slice_ = pwm_gpio_to_slice_num(pin);
    channel_ = pwm_gpio_to_channel(pin);
    setFreq(kPwmDefaultFreq);
}

Pwm::~Pwm() {
    stop();
}

uint Pwm::getPin() const {
    return pin_;
}

uint Pwm::getSlice() const {
    return slice_;
}

uint Pwm::getChannel() const {
    return channel_;
}

void Pwm::setFreq(uint32_t freq) {
    if (!freq) { freq = 1; }
    if (freq > (kPwmDefaultClock / 2)) { freq = kPwmDefaultClock / 2; }
    if (freq == freq_) { return; }
    // set frequency
    // determine top given Hz - assumes free-running counter rather than
    // phase-correct
    // pwm clock should now be running at kPwmDefaultClock
    pwm_set_clkdiv(slice_, divider_);
    uint32_t top =  kPwmDefaultClock / freq -1;
    pwm_set_wrap(slice_, top);
    freq_ = freq;
}

uint32_t Pwm::getFreq() const {
    return freq_;
}

void Pwm::setDuty(float duty) {
    if (duty < 0.0f) duty = 0.0f;
    if (duty > 100.0f) duty = 100.0f;
    if (duty == duty_) { return; }
    // set duty cycle
    uint32_t top =  kPwmDefaultClock / freq_ -1;
    // calculate channel level from given duty cycle in %
    uint16_t level = (uint16_t)((top + 1.0f) * duty / 100.0f);
    pwm_set_chan_level(slice_, channel_, level);
    duty_ = duty;
}

float Pwm::getDuty() const {
    return duty_;
}

void Pwm::start() {
    if (running_) { return; }
    // set PWM function for pin
    gpio_set_function(pin_, GPIO_FUNC_PWM);
    // enable PWM
    pwm_set_enabled(slice_, true);
    running_ = true;
}

void Pwm::stop() {
    if (!running_) { return; }
    // disable PWM
    pwm_set_enabled(slice_, false);
    // set GPIO/SIO function for pin
    gpio_set_function(pin_, GPIO_FUNC_SIO);
    // reset state of pin (set to low)
    gpio_set_dir(pin_, GPIO_OUT);
    gpio_put(pin_, false);
    running_ = false;
}

bool Pwm::isRunning() const {
    return running_;
}
