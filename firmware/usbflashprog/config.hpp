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
constexpr uint kVddPwmPin = 20;
/** @brief VDD/PWM : PWM Frequency, in Hertz. */
constexpr uint32_t kVddPwmFreq = Pwm::kPwmDefaultFreq;

/** @brief VDD/PWM : PWM Min Duty Cycle, in Percent. */
constexpr float kVddPwmMinDuty = VddConfig::kPwmMinDutyCycleDefault;

/** @brief VDD/PWM : PWM Max Duty Cycle, in Percent. */
constexpr float kVddPwmMaxDuty = VddConfig::kPwmMaxDutyCycleDefault;

/** @brief VDD/PWM : PWM Slow Step Duty Cycle, in Percent. */
constexpr float kVddPwmSlowStepDuty = VddConfig::kPwmSlowStepDutyCycleDefault;

/** @brief VDD/PWM : PWM Fast Step Duty Cycle, in Percent. */
constexpr float kVddPwmFastStepDuty = VddConfig::kPwmFastStepDutyCycleDefault;

/** @brief VDD/PWM : PWM Voltage Tolerance to use Fast Step, in Percent. */
constexpr float kVddPwmToleranceToFast = VddConfig::kPwmToleranceToFastDefault;

/** @brief VDD/ADC : Assigned ADC channel. */
constexpr uint kVddAdcChannel = 0;
/** @brief VDD/ADC : ADC Reference Voltage, in Volts. */
constexpr float kVddAdcVRef = Adc::kAdcDefaultVRef;
/** @brief VDD/VC : Voltage Converter Feedback R1 resistor, in Ohms. */
constexpr float kVddFbR1 = 470.0f;
/** @brief VDD/VC : Voltage Converter Feedback R2 resistor, in Ohms. */
constexpr float kVddFbR2 = 330.0f;
/** @brief VDD/VC : Voltage Converter Feedback Divisor, from 0.0 to 1.0. */
constexpr float kVddDivider = (kVddFbR1 + kVddFbR2) / kVddFbR2;
/** @brief VDD/VC : Output Voltage Tolerance, in percent. */
constexpr float kVddVoutTolerance = VddConfig::kDc2DcVoutToleranceDefault;

/** @brief VDD/CTRL : Assigned Voltage Control GPIO pin. */
constexpr uint kVddCtrlPin = 22;
/** @brief VDD/CTRL : Assigned VDD on VPP GPIO pin. */
constexpr uint kVddOnVppPin = 17;

// ---------------------------------------------------------------------------

/** @brief VPP/PWM : Assigned PWM GPIO pin. */
constexpr uint kVppPwmPin = 21;
/** @brief VPP/PWM : PWM Frequency, in Hertz. */
constexpr uint32_t kVppPwmFreq = Pwm::kPwmDefaultFreq;

/** @brief VPP/PWM : PWM Min Duty Cycle, in Percent. */
constexpr float kVppPwmMinDuty = VddConfig::kPwmMinDutyCycleDefault;

/** @brief VPP/PWM : PWM Max Duty Cycle, in Percent. */
constexpr float kVppPwmMaxDuty = VddConfig::kPwmMaxDutyCycleDefault;

/** @brief VPP/PWM : PWM Slow Step Duty Cycle, in Percent. */
constexpr float kVppPwmSlowStepDuty = VddConfig::kPwmSlowStepDutyCycleDefault;

/** @brief VPP/PWM : PWM Fast Step Duty Cycle, in Percent. */
constexpr float kVppPwmFastStepDuty = VddConfig::kPwmFastStepDutyCycleDefault;

/** @brief VPP/PWM : PWM Voltage Tolerance to use Fast Step, in Percent. */
constexpr float kVppPwmToleranceToFast = VddConfig::kPwmToleranceToFastDefault;

/** @brief VPP/ADC : Assigned ADC channel. */
constexpr uint kVppAdcChannel = 1;
/** @brief VPP/ADC : ADC Reference Voltage, in Volts. */
constexpr float kVppAdcVRef = Adc::kAdcDefaultVRef;
/** @brief VPP/VC : Voltage Converter Feedback R1 resistor, in Ohms. */
constexpr float kVppFbR1 = 3900.0f;
/** @brief VPP/VC : Voltage Converter Feedback R2 resistor, in Ohms. */
constexpr float kVppFbR2 = 470.0f;
/** @brief VPP/VC : Voltage Converter Feedback Divisor, from 0.0 to 1.0. */
constexpr float kVppDivider = (kVppFbR1 + kVppFbR2) / kVppFbR2;
/** @brief VPP/VC : Output Voltage Tolerance, in percent. */
constexpr float kVppVoutTolerance = VddConfig::kDc2DcVoutToleranceDefault;

/** @brief VPP/CTRL : Assigned Voltage Control GPIO pin. */
constexpr uint kVppCtrlPin = 28;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register SIN, GPIO pin. */
constexpr uint kVppVcSinPin = 12;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register CLK, GPIO pin. */
constexpr uint kVppVcClkPin = 14;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register CLR, GPIO pin. */
constexpr uint kVppVcClrPin = 15;
/** @brief VPP/CTRL : Assigned Voltage Control, Shift Register RCK, GPIO pin. */
constexpr uint kVppVcRckPin = 13;

// ---------------------------------------------------------------------------

/** @brief VPP/CTRL : VC Shift Register, VPP on A18 bit. */
constexpr uint8_t kVppOnA18VcRegisterBit = 0;
/** @brief VPP/CTRL : VC Shift Register, VPP on A9 bit. */
constexpr uint8_t kVppOnA9VcRegisterBit = 1;
/** @brief VPP/CTRL : VC Shift Register, VPP on CE bit. */
constexpr uint8_t kVppOnCEVcRegisterBit = 2;
/** @brief VPP/CTRL : VC Shift Register, VPP on OE bit. */
constexpr uint8_t kVppOnOEVcRegisterBit = 3;
/** @brief VPP/CTRL : VC Shift Register, VPP on WE bit. */
constexpr uint8_t kVppOnWEVcRegisterBit = 4;

// ---------------------------------------------------------------------------

/** @brief VPP/GEN : VPP Initial value, in Volts. */
constexpr float kVppInitial = 12.0f;
/** @brief VDD/GEN : VDD Initial value, in Volts. */
constexpr float kVddInitial = 5.0f;

// ---------------------------------------------------------------------------

/** @brief BUS/CTRL : Assigned CE GPIO pin. */
constexpr uint kBusCEPin = 11;
/** @brief BUS/CTRL : Assigned OE GPIO pin. */
constexpr uint kBusOEPin = 9;
/** @brief BUS/CTRL : Assigned WE GPIO pin. */
constexpr uint kBusWEPin = 10;

// ---------------------------------------------------------------------------

/** @brief BUS/Data : Assigned Data Bus, Shift Register SIN, GPIO pin. */
constexpr uint kBusDataSinPin = 4;
/** @brief BUS/Data : Assigned Data Bus, Shift Register CLK, GPIO pin. */
constexpr uint kBusDataClkPin = 6;
/** @brief BUS/Data : Assigned Data Bus, Shift Register CLR, GPIO pin. */
constexpr uint kBusDataClrPin = 7;
/** @brief BUS/Data : Assigned Data Bus, Shift Register RCK, GPIO pin. */
constexpr uint kBusDataRckPin = 5;
/** @brief BUS/Data : Assigned Data Bus, Shift Register SOUT, GPIO pin. */
constexpr uint kBusDataSoutPin = 8;

// ---------------------------------------------------------------------------

/** @brief BUS/Address : Assigned Address Bus, Shift Register SIN, GPIO pin. */
constexpr uint kBusAddrSinPin = 0;
/** @brief BUS/Address : Assigned Address Bus, Shift Register CLK, GPIO pin. */
constexpr uint kBusAddrClkPin = 2;
/** @brief BUS/Address : Assigned Address Bus, Shift Register CLR, GPIO pin. */
constexpr uint kBusAddrClrPin = 3;
/** @brief BUS/Address : Assigned Address Bus, Shift Register RCK, GPIO pin. */
constexpr uint kBusAddrRckPin = 1;

// ---------------------------------------------------------------------------

/** @brief COMM : Communication timeout, in milliseconds. */
constexpr uint32_t kCommTimeOut = 50;

// ---------------------------------------------------------------------------

/** @brief GENERAL : Time for stabilization, in milliseconds. */
constexpr uint32_t kStabilizationTime = 200;

#endif  // CONFIG_HPP_
