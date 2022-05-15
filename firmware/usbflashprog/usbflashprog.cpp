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
 * @file usbflashprog.cpp
 * @brief Implementation of the UsbFlashProg Main Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "usbflashprog.hpp"
#include "config.hpp"

// To remove
#include <iostream>
#include <iomanip>
#include "pico/stdlib.h"
#include "hal/gpio.hpp"
// /To remove

// ---------------------------------------------------------------------------

constexpr float VPP_CALIBRATION = 0.969137048f;
constexpr float VDD_CALIBRATION = 0.982591876f;

constexpr float VPP_INITIAL = 12.0f;
constexpr float VDD_INITIAL =  5.0f;

// ---------------------------------------------------------------------------

// To remove
static Gpio gpio;
static HC595 hc595(12, 14, 15, 13);
static uint8_t bit = 0;
// / To remove

// ---------------------------------------------------------------------------

UsbFlashProg::UsbFlashProg(MultiCore::MultiCoreEntry entry):
        multicore_(entry) {}

UsbFlashProg::~UsbFlashProg() {}

void UsbFlashProg::init() {
    config_.vpp.pwmPin             =               VPP_PWM_PIN;
    config_.vpp.pwmFreq            =              VPP_PWM_FREQ;
    config_.vpp.pwmMinDuty         =          VPP_PWM_MIN_DUTY;
    config_.vpp.pwmMaxDuty         =          VPP_PWM_MAX_DUTY;
    config_.vpp.pwmSlowStepDuty    =    VPP_PWM_SLOW_STEP_DUTY;
    config_.vpp.pwmFastStepDuty    =    VPP_PWM_FAST_STEP_DUTY;
    config_.vpp.pwmToleranceToFast = VPP_PWM_TOLERANCE_TO_FAST;
    config_.vpp.adcChannel         =           VPP_ADC_CHANNEL;
    config_.vpp.adcVref            =              VPP_ADC_VREF;
    config_.vpp.divider            = VPP_DIVIDER * VPP_CALIBRATION;
    config_.vpp.vTolerance         =        VPP_VOUT_TOLERANCE;
    config_.vpp.ctrlPin            =              VPP_CTRL_PIN;
    config_.vpp.vcSinPin           =            VPP_VC_SIN_PIN;
    config_.vpp.vcClkPin           =            VPP_VC_CLK_PIN;
    config_.vpp.vcClrPin           =            VPP_VC_CLR_PIN;
    config_.vpp.vcRckPin           =            VPP_VC_RCK_PIN;

    config_.vdd.pwmPin             =               VDD_PWM_PIN;
    config_.vdd.pwmFreq            =              VDD_PWM_FREQ;
    config_.vdd.pwmMinDuty         =          VDD_PWM_MIN_DUTY;
    config_.vdd.pwmMaxDuty         =          VDD_PWM_MAX_DUTY;
    config_.vdd.pwmSlowStepDuty    =    VDD_PWM_SLOW_STEP_DUTY;
    config_.vdd.pwmFastStepDuty    =    VDD_PWM_FAST_STEP_DUTY;
    config_.vdd.pwmToleranceToFast = VDD_PWM_TOLERANCE_TO_FAST;
    config_.vdd.adcChannel         =           VDD_ADC_CHANNEL;
    config_.vdd.adcVref            =              VDD_ADC_VREF;
    config_.vdd.divider            = VDD_DIVIDER * VDD_CALIBRATION;
    config_.vdd.vTolerance         =        VDD_VOUT_TOLERANCE;
    config_.vdd.ctrlPin            =              VDD_CTRL_PIN;
    config_.vdd.onVppPin           =            VDD_ON_VPP_PIN;

    vpp_.configure(config_.vpp);
    vdd_.configure(config_.vdd);

    // To remove
    stdio_init_all();
    hc595.clear();
    hc595.outputEnable();
    // /To remove

    vpp_.setV(VPP_INITIAL);
    vdd_.setV(VDD_INITIAL);
    vpp_.start();
    vdd_.start();
    multicore_.launch();
}

UsbFlashProgConfig UsbFlashProg::getConfig() const {
    return config_;
}

void UsbFlashProg::firstCore(void) {
    // To remove
    gpio.togglePin(PICO_DEFAULT_LED_PIN);
    hc595.setBit(bit);
    multicore_.msleep(200);
    hc595.resetBit(bit);
    bit++;
    if (bit > 4) bit = 0;

    int opc = getchar_timeout_us(0);
    switch (opc) {
        case '+':
            break;
        case '-':
            break;
        case 'a':
            vdd_.stop();
            break;
        case 's':
            vdd_.start();
            break;
        case '0':
            vpp_.stop();
            break;
        case '1':
            vpp_.start();
            break;
        default:
            break;
    }
    // /To remove
}

void UsbFlashProg::secondCore(MultiCore& core) { // NOLINT
    vpp_.adjust();
    vdd_.adjust();
    // To remove
    float vpp = vpp_.getV();
    float vdd = vdd_.getV();
    std::cout << "VPP: "
        << std::fixed << std::setprecision(2) << vpp << "V" << " ";
    std::cout << "VDD: "
        << std::fixed << std::setprecision(2) << vdd << "V" << std::endl;
    // /To remove
}
