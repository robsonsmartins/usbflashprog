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

// To remove
#include <iostream>
#include <iomanip>
#include "pico/stdlib.h"
#include "hal/gpio.hpp"
// /To remove

#include "usbflashprog.hpp"
#include "config.hpp"

// ---------------------------------------------------------------------------

constexpr float VPP_INITIAL = 12.0f;
constexpr float VDD_INITIAL =  5.0f;

// ---------------------------------------------------------------------------

// To remove
static Gpio gpio;
// static HC595 hc595(12, 14, 15, 13);
// static uint8_t bit = 0;
// / To remove

// ---------------------------------------------------------------------------

UsbFlashProg::UsbFlashProg() {}

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
    config_.vpp.divider            =               VPP_DIVIDER;
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
    config_.vdd.divider            =               VDD_DIVIDER;
    config_.vdd.vTolerance         =        VDD_VOUT_TOLERANCE;
    config_.vdd.ctrlPin            =              VDD_CTRL_PIN;
    config_.vdd.onVppPin           =            VDD_ON_VPP_PIN;

    vgen_.configure(config_);

    // To remove
    stdio_init_all();
     // hc595.clear();
     // hc595.outputEnable();
    // /To remove

    vgen_.vpp.setV(VPP_INITIAL);
    vgen_.vdd.setV(VDD_INITIAL);
    vgen_.start();
}

void UsbFlashProg::firstCore(void) {
    // To remove
    gpio.togglePin(PICO_DEFAULT_LED_PIN);
    // hc595.setBit(bit);
    // hc595.resetBit(bit);
    // bit++;
    // if (bit > 4) bit = 0;
    MultiCore::msleep(200);
    float vpp = vgen_.vpp.getV();
    float vdd = vgen_.vdd.getV();
    std::cout << "VPP: "
        << std::fixed << std::setprecision(2) << vpp << "V" << " ";
    std::cout << "VDD: "
        << std::fixed << std::setprecision(2) << vdd << "V" << " ";
    std::cout << "1.Cal VDD | ";
    std::cout << "2.Cal VPP | ";
    std::cout << "3.Set VDD | ";
    std::cout << "4.Set VPP | ";
    std::cout << "5.VDD On/Off | ";
    std::cout << "6.VPP On/Off" << std::endl;
    int opc = getchar_timeout_us(0);
    float v;
    switch (opc) {
        case '1':
            std::cout << "*** VDD Cal = ";
            std::cout << vgen_.vdd.getCalibration() << std::endl;
            std::cout << "Escreva o VDD Medido: ";
            std::cout.flush();
            vgen_.vdd.initCalibration(5.0f);
            std::cin >> v;
            if (v <= 0.0f || v >= 10.0f) v = 5.0f;
            std::cout << v << "V" << std::endl;
            vgen_.vdd.saveCalibration(v);
            std::cout << "*** VDD Cal = ";
            std::cout << vgen_.vdd.getCalibration() << std::endl;
            vgen_.vdd.setV(VDD_INITIAL);
            vgen_.vdd.on();
            vgen_.vpp.on();
            break;
        case '2':
            std::cout << "*** VPP Cal = ";
            std::cout << vgen_.vpp.getCalibration() << std::endl;
            std::cout << "Escreva o VPP Medido: ";
            std::cout.flush();
            vgen_.vpp.initCalibration(12.0f);
            std::cin >> v;
            if (v <= 0.0f || v >= 30.0f) v = 12.0f;
            std::cout << v << "V" << std::endl;
            vgen_.vpp.saveCalibration(v);
            std::cout << "*** VPP Cal = ";
            std::cout << vgen_.vpp.getCalibration() << std::endl;
            vgen_.vpp.setV(VPP_INITIAL);
            vgen_.vpp.on();
            vgen_.vdd.on();
            break;
        case '3':
            std::cout << "Escreva o VDD Desejado: ";
            std::cout.flush();
            std::cin >> v;
            if (v <   0.0f) { v =  0.0f; }
            if (v >= 10.0f) { v = 10.0f; }
            std::cout << v << "V" << std::endl;
            vgen_.vdd.setV(v);
            vgen_.vdd.on();
            break;
        case '4':
            std::cout << "Escreva o VPP Desejado: ";
            std::cout.flush();
            std::cin >> v;
            if (v <   0.0f) { v =  0.0f; }
            if (v >= 30.0f) { v = 30.0f; }
            std::cout << v << "V" << std::endl;
            vgen_.vpp.setV(v);
            vgen_.vpp.on();
            break;
        case '5':
            vgen_.vdd.toggle();
            break;
        case '6':
            vgen_.vpp.toggle();
            break;
        default:
            break;
    }
    // /To remove
}
