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
    TByteArray data;
    command_.clear();
    data = readByte_(1);
    command_.insert(command_.end(), data.begin(), data.end());
    auto code = findOpCode_();
    if (code != kCmdOpCodes.end() && code->second.params) {
      data = readByte_(code->second.params);
      command_.insert(command_.end(), data.begin(), data.end());
    }
    runCommand_();
    gpio_.resetPin(PICO_DEFAULT_LED_PIN);
}

Runner::TByteArray Runner::readByte_(size_t len) {
    TByteArray result;
    if (len) {
        uint8_t *buf = new uint8_t[len];
        if (serial_.getBuf(buf, len, kCommTimeOut * 1000) == len) {
            result.insert(result.end(), buf, buf + len);
            gpio_.togglePin(PICO_DEFAULT_LED_PIN);
        }
        delete[] buf;
    }
    return result;
}

TCmdOpCodeMap::const_iterator Runner::findOpCode_() {
    if (command_.size() < 1) { return kCmdOpCodes.end(); }
    return kCmdOpCodes.find(static_cast<kCmdOpCodeEnum>(command_[0]));
}

void Runner::runCommand_() {
    if (command_.size() < 1) { return; }
    auto code = findOpCode_();
    if (code == kCmdOpCodes.end() ||
            command_.size() < (code->second.params + 1)) {
        // opcode not found or nparams invalid
        serial_.putChar(kCmdResponseNok, true);
        return;
    }
    if (code->first == kCmdNop) {  // NOP
        serial_.putChar(kCmdResponseOk, true);
    } else {
      runVddCommand_(code->first);
      runVppCommand_(code->first);
      runCtrlBusCommand_(code->first);
      // TODO(robsonsmartins): runAddrBusCommand_
      // TODO(robsonsmartins): runDataBusCommand_
    }
}

void Runner::runVddCommand_(uint8_t opcode) {
    float v;
    bool b;
    TByteArray response;
    switch (opcode) {
      case kCmdVddCtrl:
        if (getParamAsBool_()) {
            vgen_.vdd.on();
        } else {
            vgen_.vdd.off();
        }
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVddSetV:
        v = getParamsAsFloat_();
        vgen_.vdd.setV(v);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVddGetV:
        v = vgen_.vdd.getV();
        response.resize(3);
        response[0] = kCmdResponseOk;
        createParamsFromFloat_(&response, v);
        serial_.putBuf(response.data(), response.size(), true);
        break;
      case kCmdVddGetDuty:
        v = vgen_.vdd.getDuty();
        response.resize(3);
        response[0] = kCmdResponseOk;
        createParamsFromFloat_(&response, v);
        serial_.putBuf(response.data(), response.size(), true);
        break;
      case kCmdVddGetCal:
        v = vgen_.vdd.getCalibration();
        response.resize(3);
        response[0] = kCmdResponseOk;
        createParamsFromFloat_(&response, v);
        serial_.putBuf(response.data(), response.size(), true);
        break;
      case kCmdVddInitCal:
        vgen_.vdd.initCalibration();
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVddSaveCal:
        v = getParamsAsFloat_();
        vgen_.vdd.saveCalibration(v);
        vgen_.vdd.setV(kVddInitial);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVddOnVpp:
        b = getParamAsBool_();
        vgen_.vdd.onVpp(b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      default:
        break;
    }
}

void Runner::runVppCommand_(uint8_t opcode) {
    float v;
    bool b;
    TByteArray response;
    switch (opcode) {
      case kCmdVppCtrl:
        if (getParamAsBool_()) {
            vgen_.vpp.on();
        } else {
            vgen_.vpp.off();
        }
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppSetV:
        v = getParamsAsFloat_();
        vgen_.vpp.setV(v);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppGetV:
        v = vgen_.vpp.getV();
        response.resize(3);
        response[0] = kCmdResponseOk;
        createParamsFromFloat_(&response, v);
        serial_.putBuf(response.data(), response.size(), true);
        break;
      case kCmdVppGetDuty:
        v = vgen_.vpp.getDuty();
        response.resize(3);
        response[0] = kCmdResponseOk;
        createParamsFromFloat_(&response, v);
        serial_.putBuf(response.data(), response.size(), true);
        break;
      case kCmdVppGetCal:
        v = vgen_.vpp.getCalibration();
        response.resize(3);
        response[0] = kCmdResponseOk;
        createParamsFromFloat_(&response, v);
        serial_.putBuf(response.data(), response.size(), true);
        break;
      case kCmdVppInitCal:
        vgen_.vpp.initCalibration();
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppSaveCal:
        v = getParamsAsFloat_();
        vgen_.vpp.saveCalibration(v);
        vgen_.vpp.setV(kVppInitial);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppOnA9:
        b = getParamAsBool_();
        vgen_.vpp.onA9(b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppOnA18:
        b = getParamAsBool_();
        vgen_.vpp.onA18(b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppOnCE:
        b = getParamAsBool_();
        vgen_.vpp.onCE(b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppOnOE:
        b = getParamAsBool_();
        vgen_.vpp.onOE(b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdVppOnWE:
        b = getParamAsBool_();
        vgen_.vpp.onWE(b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      default:
        break;
    }
}

void Runner::runCtrlBusCommand_(uint8_t opcode) {
    bool b;
    TByteArray response;
    switch (opcode) {
      case kCmdBusCE:
        b = getParamAsBool_();
        gpio_.setPin(kBusCEPin, b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdBusOE:
        b = getParamAsBool_();
        gpio_.setPin(kBusOEPin, b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      case kCmdBusWE:
        b = getParamAsBool_();
        gpio_.setPin(kBusWEPin, b);
        serial_.putChar(kCmdResponseOk, true);
        break;
      default:
        break;
    }
}

bool Runner::getParamAsBool_() {
    return (OpCode::getValueAsBool(command_.data(), command_.size()));
}

float Runner::getParamsAsFloat_() {
    return (OpCode::getValueAsFloat(command_.data(), command_.size()));
}

void Runner::createParamsFromFloat_(TByteArray *response, float src) {
    OpCode::setValue(response->data(), response->size(), src);
}
