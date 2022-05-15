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
 * @file config.hpp
 * @brief Configuration of UsbFlashProg.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "pico/stdlib.h"
#include "hal/adc.hpp"
#include "hal/pwm.hpp"
#include "modules/vgenerator.hpp"

// ---------------------------------------------------------------------------

/** @brief VDD/PWM : Assigned PWM GPIO pin. */
constexpr uint     VDD_PWM_PIN  = 20;
/** @brief VDD/PWM : PWM Frequency, in Hertz. */
constexpr uint32_t VDD_PWM_FREQ = Pwm::PWM_DEFAULT_FREQ;

/** @brief VDD/PWM : PWM Min Duty Cycle, in Percent. */
constexpr float    VDD_PWM_MIN_DUTY
    = VddConfig::PWM_MIN_DUTY_CYCLE_DEFAULT;

/** @brief VDD/PWM : PWM Max Duty Cycle, in Percent. */
constexpr float    VDD_PWM_MAX_DUTY
    = VddConfig::PWM_MAX_DUTY_CYCLE_DEFAULT;

/** @brief VDD/PWM : PWM Slow Step Duty Cycle, in Percent. */
constexpr float    VDD_PWM_SLOW_STEP_DUTY
    = VddConfig::PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT;

/** @brief VDD/PWM : PWM Fast Step Duty Cycle, in Percent. */
constexpr float    VDD_PWM_FAST_STEP_DUTY
    = VddConfig::PWM_FAST_STEP_DUTY_CYCLE_DEFAULT;

/** @brief VDD/PWM : PWM Voltage Tolerance to use Fast Step, in Percent. */
constexpr float    VDD_PWM_TOLERANCE_TO_FAST
    = VddConfig::PWM_TOLERANCE_TO_FAST_DEFAULT;

/** @brief VDD/ADC : Assigned ADC channel. */
constexpr uint     VDD_ADC_CHANNEL    = 0;
/** @brief VDD/ADC : ADC Reference Voltage, in Volts. */
constexpr float    VDD_ADC_VREF       = Adc::DEFAULT_VREF;
/** @brief VDD/VC : Voltage Converter Feedback R1 resistor, in Ohms. */
constexpr float    VDD_FB_R1          = 470.0f;
/** @brief VDD/VC : Voltage Converter Feedback R2 resistor, in Ohms. */
constexpr float    VDD_FB_R2          = 330.0f;
/** @brief VDD/VC : Voltage Converter Feedback Divisor, from 0.0 to 1.0. */
constexpr float    VDD_DIVIDER        = (VDD_FB_R1 + VDD_FB_R2) / VDD_FB_R2;
/** @brief VDD/VC : Output Voltage Tolerance, in percent. */
constexpr float    VDD_VOUT_TOLERANCE = VddConfig::DC2DC_VOUT_TOLERANCE_DEFAULT;

/** @brief VDD/CTRL : Assigned Voltage Control GPIO pin. */
constexpr uint     VDD_CTRL_PIN     = 22;
/** @brief VDD/CTRL : Assigned VDD on VPP GPIO pin. */
constexpr uint     VDD_ON_VPP_PIN   = 17;

// ---------------------------------------------------------------------------

/** @brief VPP/PWM : Assigned PWM GPIO pin. */
constexpr uint     VPP_PWM_PIN  = 21;
/** @brief VPP/PWM : PWM Frequency, in Hertz. */
constexpr uint32_t VPP_PWM_FREQ = Pwm::PWM_DEFAULT_FREQ;

/** @brief VPP/PWM : PWM Min Duty Cycle, in Percent. */
constexpr float    VPP_PWM_MIN_DUTY
    = VddConfig::PWM_MIN_DUTY_CYCLE_DEFAULT;

/** @brief VPP/PWM : PWM Max Duty Cycle, in Percent. */
constexpr float    VPP_PWM_MAX_DUTY
    = VddConfig::PWM_MAX_DUTY_CYCLE_DEFAULT;

/** @brief VPP/PWM : PWM Slow Step Duty Cycle, in Percent. */
constexpr float    VPP_PWM_SLOW_STEP_DUTY
    = VddConfig::PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT;

/** @brief VPP/PWM : PWM Fast Step Duty Cycle, in Percent. */
constexpr float    VPP_PWM_FAST_STEP_DUTY
    = VddConfig::PWM_FAST_STEP_DUTY_CYCLE_DEFAULT;

/** @brief VPP/PWM : PWM Voltage Tolerance to use Fast Step, in Percent. */
constexpr float    VPP_PWM_TOLERANCE_TO_FAST
    = VddConfig::PWM_TOLERANCE_TO_FAST_DEFAULT;

/** @brief VPP/ADC : Assigned ADC channel. */
constexpr uint     VPP_ADC_CHANNEL    = 1;
/** @brief VPP/ADC : ADC Reference Voltage, in Volts. */
constexpr float    VPP_ADC_VREF       = Adc::DEFAULT_VREF;
/** @brief VPP/VC : Voltage Converter Feedback R1 resistor, in Ohms. */
constexpr float    VPP_FB_R1          = 3900.0f;
/** @brief VPP/VC : Voltage Converter Feedback R2 resistor, in Ohms. */
constexpr float    VPP_FB_R2          = 470.0f;
/** @brief VPP/VC : Voltage Converter Feedback Divisor, from 0.0 to 1.0. */
constexpr float    VPP_DIVIDER        = (VPP_FB_R1 + VPP_FB_R2) / VPP_FB_R2;
/** @brief VPP/VC : Output Voltage Tolerance, in percent. */
constexpr float    VPP_VOUT_TOLERANCE = VddConfig::DC2DC_VOUT_TOLERANCE_DEFAULT;

/** @brief VPP/CTRL : Assigned Voltage Control GPIO pin. */
constexpr uint     VPP_CTRL_PIN     = 28;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register SIN, GPIO pin. */
constexpr uint     VPP_VC_SIN_PIN   = 12;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register CLK, GPIO pin. */
constexpr uint     VPP_VC_CLK_PIN   = 14;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register CLR, GPIO pin. */
constexpr uint     VPP_VC_CLR_PIN   = 15;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register RCK, GPIO pin. */
constexpr uint     VPP_VC_RCK_PIN   = 13;

// ---------------------------------------------------------------------------

/** @brief VPP/CTRL : VC Shift Register, VPP on A18 bit. */
constexpr uint8_t VPP_ON_A18_VC_REGISTER_BIT = 0;
/** @brief VPP/CTRL : VC Shift Register, VPP on A9 bit. */
constexpr uint8_t VPP_ON_A9_VC_REGISTER_BIT  = 1;
/** @brief VPP/CTRL : VC Shift Register, VPP on CE bit. */
constexpr uint8_t VPP_ON_CE_VC_REGISTER_BIT  = 2;
/** @brief VPP/CTRL : VC Shift Register, VPP on OE bit. */
constexpr uint8_t VPP_ON_OE_VC_REGISTER_BIT  = 3;
/** @brief VPP/CTRL : VC Shift Register, VPP on WE bit. */
constexpr uint8_t VPP_ON_WE_VC_REGISTER_BIT  = 4;

#endif  // CONFIG_HPP_
