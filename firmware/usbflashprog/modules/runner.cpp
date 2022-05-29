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
 * @file modules/runner.cpp
 * @brief Implementation of the Communication Runner Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cmath>

#include "config.hpp"
#include "hal/string.hpp"
#include "modules/runner.hpp"

// ---------------------------------------------------------------------------

Runner::Runner() {}

void Runner::init() {
    config_.vpp.pwmPin             =             kVppPwmPin;
    config_.vpp.pwmFreq            =            kVppPwmFreq;
    config_.vpp.pwmMinDuty         =         kVppPwmMinDuty;
    config_.vpp.pwmMaxDuty         =         kVppPwmMaxDuty;
    config_.vpp.pwmSlowStepDuty    =    kVppPwmSlowStepDuty;
    config_.vpp.pwmFastStepDuty    =    kVppPwmFastStepDuty;
    config_.vpp.pwmToleranceToFast = kVppPwmToleranceToFast;
    config_.vpp.adcChannel         =         kVppAdcChannel;
    config_.vpp.adcVref            =            kVppAdcVRef;
    config_.vpp.divider            =            kVppDivider;
    config_.vpp.vTolerance         =      kVppVoutTolerance;
    config_.vpp.ctrlPin            =            kVppCtrlPin;
    config_.vpp.vcSinPin           =           kVppVcSinPin;
    config_.vpp.vcClkPin           =           kVppVcClkPin;
    config_.vpp.vcClrPin           =           kVppVcClrPin;
    config_.vpp.vcRckPin           =           kVppVcRckPin;

    config_.vdd.pwmPin             =             kVddPwmPin;
    config_.vdd.pwmFreq            =            kVddPwmFreq;
    config_.vdd.pwmMinDuty         =         kVddPwmMinDuty;
    config_.vdd.pwmMaxDuty         =         kVddPwmMaxDuty;
    config_.vdd.pwmSlowStepDuty    =    kVddPwmSlowStepDuty;
    config_.vdd.pwmFastStepDuty    =    kVddPwmFastStepDuty;
    config_.vdd.pwmToleranceToFast = kVddPwmToleranceToFast;
    config_.vdd.adcChannel         =         kVddAdcChannel;
    config_.vdd.adcVref            =            kVddAdcVRef;
    config_.vdd.divider            =            kVddDivider;
    config_.vdd.vTolerance         =      kVddVoutTolerance;
    config_.vdd.ctrlPin            =            kVddCtrlPin;
    config_.vdd.onVppPin           =           kVddOnVppPin;

    vgen_.configure(config_);
    vgen_.vpp.setV(kVppInitial);
    vgen_.vdd.setV(kVddInitial);
    vgen_.start();
}

void Runner::loop() {
    command_ = readByte_(1);
    auto code = findOpCode_();
    if (code != kCmdOpCodes.end()) {
        command_ += readByte_(code->second.params);
    }
    runCommand_();
    gpio_.resetPin(PICO_DEFAULT_LED_PIN);
}

std::string Runner::readByte_(size_t len) {
    std::string result;
    if (len) {
        len *= 2;
        char *buf = new char[len + 1];
        buf[len] = 0;
        if (serial_.getBuf(buf, len, kCommTimeOut * 1000) == len) {
            result = buf;
            gpio_.togglePin(PICO_DEFAULT_LED_PIN);
        }
        delete[] buf;
    }
    return result;
}

TCmdOpCodeMap::const_iterator Runner::findOpCode_() {
    if (command_.length() < 2) { return kCmdOpCodes.end(); }
    return kCmdOpCodes.find(command_.substr(0, 2).c_str());
}

void Runner::runCommand_() {
    if (command_.length() < 2) { return; }
    auto code = findOpCode_();
    if (code == kCmdOpCodes.end() ||
            command_.length() < (code->second.params * 2 + 2)) {
        // opcode not found or nparams invalid
        serial_.putStr(kCmdResponseNok, true);
        return;
    }
    runVddCommand_(code->second.opcode);
    runVppCommand_(code->second.opcode);
}

void Runner::runVddCommand_(uint8_t opcode) {
    float v;
    bool b;
    switch (opcode) {
      case kCmdVddCtrl:
        if (getParamAsBool_()) {
            vgen_.vdd.on();
        } else {
            vgen_.vdd.off();
        }
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVddSetV:
        v = getParamsAsFloat_();
        vgen_.vdd.setV(v);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVddGetV:
        v = vgen_.vdd.getV();
        serial_.putStr(kCmdResponseOk + createParamsFromFloat_(v), true);
        break;
      case kCmdVddGetDuty:
        v = vgen_.vdd.getDuty();
        serial_.putStr(kCmdResponseOk + createParamsFromFloat_(v), true);
        break;
      case kCmdVddGetCal:
        v = vgen_.vdd.getCalibration();
        serial_.putStr(kCmdResponseOk + createParamsFromFloat_(v), true);
        break;
      case kCmdVddInitCal:
        vgen_.vdd.initCalibration();
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVddSaveCal:
        v = getParamsAsFloat_();
        vgen_.vdd.saveCalibration(v);
        vgen_.vdd.setV(kVddInitial);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVddOnVpp:
        b = getParamAsBool_();
        vgen_.vdd.onVpp(b);
        serial_.putStr(kCmdResponseOk, true);
        break;
      default:
        break;
    }
}

void Runner::runVppCommand_(uint8_t opcode) {
    float v;
    bool b;
    switch (opcode) {
      case kCmdVppCtrl:
        if (getParamAsBool_()) {
            vgen_.vpp.on();
        } else {
            vgen_.vpp.off();
        }
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppSetV:
        v = getParamsAsFloat_();
        vgen_.vpp.setV(v);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppGetV:
        v = vgen_.vpp.getV();
        serial_.putStr(kCmdResponseOk + createParamsFromFloat_(v), true);
        break;
      case kCmdVppGetDuty:
        v = vgen_.vpp.getDuty();
        serial_.putStr(kCmdResponseOk + createParamsFromFloat_(v), true);
        break;
      case kCmdVppGetCal:
        v = vgen_.vpp.getCalibration();
        serial_.putStr(kCmdResponseOk + createParamsFromFloat_(v), true);
        break;
      case kCmdVppInitCal:
        vgen_.vpp.initCalibration();
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppSaveCal:
        v = getParamsAsFloat_();
        vgen_.vpp.saveCalibration(v);
        vgen_.vpp.setV(kVppInitial);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppOnA9:
        b = getParamAsBool_();
        vgen_.vpp.onA9(b);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppOnA18:
        b = getParamAsBool_();
        vgen_.vpp.onA18(b);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppOnCE:
        b = getParamAsBool_();
        vgen_.vpp.onCE(b);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppOnOE:
        b = getParamAsBool_();
        vgen_.vpp.onOE(b);
        serial_.putStr(kCmdResponseOk, true);
        break;
      case kCmdVppOnWE:
        b = getParamAsBool_();
        vgen_.vpp.onWE(b);
        serial_.putStr(kCmdResponseOk, true);
        break;
      default:
        break;
    }
}

uint8_t Runner::getParamAsByte_() {
    if (command_.length() < 4) { return 0; }
    return StringUtils::toInt(command_.substr(2, 2), 16);
}

bool Runner::getParamAsBool_() {
    return (getParamAsByte_() != 0);
}

float Runner::getParamsAsFloat_() {
    if (command_.length() < 6) { return 0.0f; }
    return StringUtils::toInt(command_.substr(2, 2), 16) +
           StringUtils::toInt(command_.substr(4, 2), 16) * 0.01f;
}

std::string Runner::createParamFromByte_(uint8_t src) {
    return StringUtils::fromInt(src, 16);
}

std::string Runner::createParamFromBool_(bool src) {
    if (src) {
        return createParamFromByte_(1);
    } else {
        return createParamFromByte_(0);
    }
}

std::string Runner::createParamsFromFloat_(float src) {
    double i, f = std::modf(src, &i);
    uint8_t b1 = i, b2 = f * 100;
    return createParamFromByte_(b1) + createParamFromByte_(b2);
}
