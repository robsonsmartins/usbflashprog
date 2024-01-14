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

Runner::Runner() {
    settings_.twp = 0;
    settings_.twc = 0;
    settings_.skipFF = false;
    settings_.progWithVpp = false;
    settings_.vppOePin = false;
    settings_.pgmCePin = false;
    settings_.pgmPositive = false;
}

void Runner::init() {
    vgenConfig_.vpp.pwmPin = kVppPwmPin;
    vgenConfig_.vpp.pwmFreq = kVppPwmFreq;
    vgenConfig_.vpp.pwmMinDuty = kVppPwmMinDuty;
    vgenConfig_.vpp.pwmMaxDuty = kVppPwmMaxDuty;
    vgenConfig_.vpp.pwmSlowStepDuty = kVppPwmSlowStepDuty;
    vgenConfig_.vpp.pwmFastStepDuty = kVppPwmFastStepDuty;
    vgenConfig_.vpp.pwmToleranceToFast = kVppPwmToleranceToFast;
    vgenConfig_.vpp.adcChannel = kVppAdcChannel;
    vgenConfig_.vpp.adcVref = kVppAdcVRef;
    vgenConfig_.vpp.divider = kVppDivider;
    vgenConfig_.vpp.vTolerance = kVppVoutTolerance;
    vgenConfig_.vpp.ctrlPin = kVppCtrlPin;
    vgenConfig_.vpp.vcSinPin = kVppVcSinPin;
    vgenConfig_.vpp.vcClkPin = kVppVcClkPin;
    vgenConfig_.vpp.vcClrPin = kVppVcClrPin;
    vgenConfig_.vpp.vcRckPin = kVppVcRckPin;

    vgenConfig_.vdd.pwmPin = kVddPwmPin;
    vgenConfig_.vdd.pwmFreq = kVddPwmFreq;
    vgenConfig_.vdd.pwmMinDuty = kVddPwmMinDuty;
    vgenConfig_.vdd.pwmMaxDuty = kVddPwmMaxDuty;
    vgenConfig_.vdd.pwmSlowStepDuty = kVddPwmSlowStepDuty;
    vgenConfig_.vdd.pwmFastStepDuty = kVddPwmFastStepDuty;
    vgenConfig_.vdd.pwmToleranceToFast = kVddPwmToleranceToFast;
    vgenConfig_.vdd.adcChannel = kVddAdcChannel;
    vgenConfig_.vdd.adcVref = kVddAdcVRef;
    vgenConfig_.vdd.divider = kVddDivider;
    vgenConfig_.vdd.vTolerance = kVddVoutTolerance;
    vgenConfig_.vdd.ctrlPin = kVddCtrlPin;
    vgenConfig_.vdd.onVppPin = kVddOnVppPin;

    ctrlBusConfig_.cePin = kBusCEPin;
    ctrlBusConfig_.oePin = kBusOEPin;
    ctrlBusConfig_.wePin = kBusWEPin;

    dataBusConfig_.dSinPin = kBusDataSinPin;
    dataBusConfig_.dSoutPin = kBusDataSoutPin;
    dataBusConfig_.dClkPin = kBusDataClkPin;
    dataBusConfig_.dClrPin = kBusDataClrPin;
    dataBusConfig_.dRckPin = kBusDataRckPin;

    addrBusConfig_.aSinPin = kBusAddrSinPin;
    addrBusConfig_.aClkPin = kBusAddrClkPin;
    addrBusConfig_.aClrPin = kBusAddrClrPin;
    addrBusConfig_.aRckPin = kBusAddrRckPin;

    vgen_.configure(vgenConfig_);
    vgen_.vpp.setV(kVppInitial);
    vgen_.vdd.setV(kVddInitial);
    vgen_.start();

    dataBus_.configure(dataBusConfig_);
    addrBus_.configure(addrBusConfig_);
    ctrlBus_.configure(ctrlBusConfig_);
}

void Runner::loop() {
    int c = serial_.getChar(0);
    if (c == PICO_ERROR_TIMEOUT) return;
    TByteArray data;
    data.insert(data.end(), c & 0xFF);
    command_.clear();
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
    if (command_.size() < 1) return kCmdOpCodes.end();
    return kCmdOpCodes.find(static_cast<kCmdOpCodeEnum>(command_[0]));
}

void Runner::runCommand_() {
    if (command_.size() < 1) return;
    auto code = findOpCode_();
    if (code == kCmdOpCodes.end() ||
        command_.size() < (code->second.params + 1)) {
        // opcode not found or nparams invalid
        serial_.putChar(kCmdResponseNok);
        return;
    }
    if (code->first == kCmdNop) {  // NOP
        serial_.putChar(kCmdResponseOk);
    } else {
        runVddCommand_(code->first);
        runVppCommand_(code->first);
        runCtrlBusCommand_(code->first);
        runDataBusCommand_(code->first);
        runAddrBusCommand_(code->first);

        runDeviceSettingsCommand_(code->first);
        runDeviceReadCommand_(code->first);
        runDeviceWriteCommand_(code->first);
        runDeviceVerifyCommand_(code->first);
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
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVddSetV:
            v = getParamAsFloat_();
            vgen_.vdd.setV(v);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVddGetV:
            v = vgen_.vdd.getV();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVddGetDuty:
            v = vgen_.vdd.getDuty();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVddGetCal:
            v = vgen_.vdd.getCalibration();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVddInitCal:
            vgen_.vdd.initCalibration();
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVddSaveCal:
            v = getParamAsFloat_();
            vgen_.vdd.saveCalibration(v);
            vgen_.vdd.setV(kVddInitial);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVddOnVpp:
            b = getParamAsBool_();
            vgen_.vdd.onVpp(b);
            serial_.putChar(kCmdResponseOk);
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
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppSetV:
            v = getParamAsFloat_();
            vgen_.vpp.setV(v);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppGetV:
            v = vgen_.vpp.getV();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVppGetDuty:
            v = vgen_.vpp.getDuty();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVppGetCal:
            v = vgen_.vpp.getCalibration();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVppInitCal:
            vgen_.vpp.initCalibration();
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppSaveCal:
            v = getParamAsFloat_();
            vgen_.vpp.saveCalibration(v);
            vgen_.vpp.setV(kVppInitial);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppOnA9:
            b = getParamAsBool_();
            vgen_.vpp.onA9(b);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppOnA18:
            b = getParamAsBool_();
            vgen_.vpp.onA18(b);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppOnCE:
            b = getParamAsBool_();
            vgen_.vpp.onCE(b);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppOnOE:
            b = getParamAsBool_();
            vgen_.vpp.onOE(b);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdVppOnWE:
            b = getParamAsBool_();
            vgen_.vpp.onWE(b);
            serial_.putChar(kCmdResponseOk);
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
            ctrlBus_.setCE(b);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdBusOE:
            b = getParamAsBool_();
            ctrlBus_.setOE(b);
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdBusWE:
            b = getParamAsBool_();
            ctrlBus_.setWE(b);
            serial_.putChar(kCmdResponseOk);
            break;
        default:
            break;
    }
}

void Runner::runDataBusCommand_(uint8_t opcode) {
    uint16_t w;
    TByteArray response;
    switch (opcode) {
        case kCmdBusDataClr:
            if (dataBus_.writeByte(0)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusDataSet:
            w = getParamAsWord_();
            if (dataBus_.writeWord(w)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusDataSetB:
            w = getParamAsByte_();
            if (dataBus_.writeByte(w)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusDataGet:
            w = dataBus_.readWord();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromWord_(&response, w);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdBusDataGetB:
            w = dataBus_.readByte();
            response.resize(2);
            response[0] = kCmdResponseOk;
            createParamsFromByte_(&response, w);
            serial_.putBuf(response.data(), response.size());
            break;
        default:
            break;
    }
}

void Runner::runAddrBusCommand_(uint8_t opcode) {
    uint32_t dw;
    TByteArray response;
    switch (opcode) {
        case kCmdBusAddrClr:
            if (addrBus_.writeByte(0)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrInc:
            if (addrBus_.increment()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrSet:
            dw = getParamAsDWord_();
            if (addrBus_.writeDWord(dw)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrSetB:
            dw = getParamAsByte_();
            if (addrBus_.writeByte(dw)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrSetW:
            dw = getParamAsWord_();
            if (addrBus_.writeWord(dw)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceSettingsCommand_(uint8_t opcode) {
    uint32_t dw;
    TByteArray response;
    switch (opcode) {
        case kCmdDeviceSetTwp:
            dw = getParamAsDWord_();
            settings_.twp = dw;
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdDeviceSetTwc:
            dw = getParamAsDWord_();
            settings_.twc = dw;
            serial_.putChar(kCmdResponseOk);
            break;
        case kCmdDeviceSetFlags:
            dw = getParamAsByte_();
            // 0 = Skip Write 0xFF
            // 1 = Prog with VPP on
            // 2 = VPP/~OE Pin
            // 3 = ~PGM/~CE Pin
            // 4 = PGM positive
            // clang-format off
            settings_.skipFF      = (dw & 0x01 != 0);
            settings_.progWithVpp = (dw & 0x02 != 0);
            settings_.vppOePin    = (dw & 0x04 != 0);
            settings_.pgmCePin    = (dw & 0x08 != 0);
            settings_.pgmPositive = (dw & 0x10 != 0);
            // clang-format on
            serial_.putChar(kCmdResponseOk);
            break;
        default:
            break;
    }
}

void Runner::runDeviceReadCommand_(uint8_t opcode) {
    uint16_t w;
    TByteArray response;
    switch (opcode) {
        case kCmdDeviceRead:
            deviceRead_(w, true);
            if (addrBus_.increment()) {
                // response
                response.resize(3);
                response[0] = kCmdResponseOk;
                createParamsFromWord_(&response, w);
                serial_.putBuf(response.data(), response.size());
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceReadB:
            deviceRead_(w, false);
            if (addrBus_.increment()) {
                // response
                response.resize(2);
                response[0] = kCmdResponseOk;
                createParamsFromByte_(&response, w);
                serial_.putBuf(response.data(), response.size());
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceWriteCommand_(uint8_t opcode) {
    uint16_t w;
    TByteArray response;
    bool success = false;
    switch (opcode) {
        case kCmdDeviceWrite:
            if (deviceWriteAndVerify_(w, true) && addrBus_.increment()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceWriteB:
            if (deviceWriteAndVerify_(w, false) && addrBus_.increment()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceVerifyCommand_(uint8_t opcode) {
    uint16_t w;
    TByteArray response;
    bool success = false;
    switch (opcode) {
        case kCmdDeviceVerify:
            if (deviceVerify_(w, true) && addrBus_.increment()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceVerifyB:
            if (deviceVerify_(w, false) && addrBus_.increment()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::deviceRead_(uint16_t &data, bool is16bit) {
    // ~OE/VPP is LO
    if (settings_.vppOePin) vgen_.vdd.onVpp(false);
    // ~OE is LO
    ctrlBus_.setOE(true);
    // get data
    if (is16bit) {
        data = dataBus_.readWord();
    } else {
        data = dataBus_.readByte();
    }
    // ~OE is HI
    ctrlBus_.setOE(false);
    // ~OE/VPP is VDD
    if (settings_.vppOePin) vgen_.vdd.onVpp(true);
}

bool Runner::deviceWrite_(uint16_t &data, bool is16bit) {
    bool success = true;
    if (settings_.progWithVpp) {
        // VPP on
        vgen_.vdd.onVpp(false);
        vgen_.vpp.on();
        sleep_us(kStabilizationTime);
    }
    // Set DataBus
    if (is16bit) {
        data = getParamAsWord_();
        if (!dataBus_.writeWord(data)) success = false;
    } else {
        data = getParamAsByte_();
        if (!dataBus_.writeByte(data)) success = false;
    }
    if (settings_.pgmPositive) {
        // PGM is HI (start prog pulse)
        ctrlBus_.setWE(false);
        sleep_us(settings_.twp);  // tWP uS
        // PGM is LO (end prog pulse)
        ctrlBus_.setWE(true);
    } else {
        // ~PGM is LO (start prog pulse)
        ctrlBus_.setWE(true);
        sleep_us(settings_.twp);  // tWP uS
        // ~PGM is HI (end prog pulse)
        ctrlBus_.setWE(false);
    }
    sleep_us(settings_.twc);  // tWC uS
    if (settings_.progWithVpp) {
        // VPP off
        vgen_.vpp.off();
        vgen_.vdd.onVpp(true);
        sleep_us(kStabilizationTime);
    }
    return success;
}

bool Runner::deviceVerify_(uint16_t &data, bool is16bit) {
    uint16_t wr, rd;
    // get parameter
    if (is16bit) {
        wr = getParamAsWord_();
    } else {
        wr = getParamAsByte_();
    }
    // read
    deviceRead_(rd, is16bit);
    if (!is16bit) {
        wr &= 0xFF;
        rd &= 0xFF;
    }
    // verify
    if (rd == wr) data = rd;
    return (rd == wr);
}

bool Runner::deviceWriteAndVerify_(uint16_t &data, bool is16bit) {
    uint16_t wr, rd;
    bool emptyData;
    // get parameter
    if (is16bit) {
        wr = getParamAsWord_();
        emptyData = (wr == 0xFFFF);
    } else {
        wr = getParamAsByte_();
        emptyData = (wr & 0xFF == 0xFF);
    }
    // write
    if (!settings_.skipFF || !emptyData) {
        if (!deviceWrite_(wr, is16bit)) return false;
    }
    // PGM/~CE is LO
    if (settings_.pgmCePin) ctrlBus_.setWE(true);
    // read
    deviceRead_(rd, is16bit);
    if (!is16bit) {
        wr &= 0xFF;
        rd &= 0xFF;
    }
    // verify
    if (rd == wr) data = rd;
    return (rd == wr);
}

bool Runner::getParamAsBool_() {
    return (OpCode::getValueAsBool(command_.data(), command_.size()));
}

float Runner::getParamAsFloat_() {
    return (OpCode::getValueAsFloat(command_.data(), command_.size()));
}

uint8_t Runner::getParamAsByte_() {
    return (OpCode::getValueAsByte(command_.data(), command_.size()));
}

u_int16_t Runner::getParamAsWord_() {
    return (OpCode::getValueAsWord(command_.data(), command_.size()));
}

u_int32_t Runner::getParamAsDWord_() {
    return (OpCode::getValueAsDWord(command_.data(), command_.size()));
}

void Runner::createParamsFromFloat_(TByteArray *response, float src) {
    OpCode::setFloat(response->data(), response->size(), src);
}

void Runner::createParamsFromByte_(TByteArray *response, uint8_t src) {
    OpCode::setByte(response->data(), response->size(), src);
}

void Runner::createParamsFromWord_(TByteArray *response, u_int16_t src) {
    OpCode::setWord(response->data(), response->size(), src);
}

void Runner::createParamsFromDWord_(TByteArray *response, u_int32_t src) {
    OpCode::setDWord(response->data(), response->size(), src);
}
