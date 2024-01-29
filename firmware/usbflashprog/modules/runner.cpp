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
    device_.init();
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
        runAddrBusCommand_(code->first);
        runDataBusCommand_(code->first);

        runDeviceSettingsCommand_(code->first);
        runDeviceReadCommand_(code->first);
        runDeviceWriteCommand_(code->first);
        runDeviceVerifyCommand_(code->first);
        runDeviceGetIdCommand_(code->first);
        runDeviceEraseCommand_(code->first);
        runDeviceProtectCommand_(code->first);
    }
}

void Runner::runVddCommand_(uint8_t opcode) {
    float v;
    TByteArray response;
    switch (opcode) {
        case kCmdVddCtrl:
            serial_.putChar(kCmdResponseOk, true);
            device_.vddCtrl(getParamAsBool_());
            break;
        case kCmdVddSetV:
            serial_.putChar(kCmdResponseOk, true);
            device_.vddSetV(getParamAsFloat_());
            break;
        case kCmdVddGetV:
            v = device_.vddGetV();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVddGetDuty:
            v = device_.vddGetDuty();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVddGetCal:
            v = device_.vddGetCal();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVddInitCal:
            serial_.putChar(kCmdResponseOk, true);
            device_.vddInitCal();
            break;
        case kCmdVddSaveCal:
            serial_.putChar(kCmdResponseOk, true);
            device_.vddSaveCal(getParamAsFloat_());
            break;
        case kCmdVddOnVpp:
            serial_.putChar(kCmdResponseOk, true);
            device_.vddOnVpp(getParamAsBool_());
            break;
        default:
            break;
    }
}

void Runner::runVppCommand_(uint8_t opcode) {
    float v;
    TByteArray response;
    switch (opcode) {
        case kCmdVppCtrl:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppCtrl(getParamAsBool_());
            break;
        case kCmdVppSetV:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppSetV(getParamAsFloat_());
            break;
        case kCmdVppGetV:
            v = device_.vppGetV();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVppGetDuty:
            v = device_.vppGetDuty();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVppGetCal:
            v = device_.vppGetCal();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromFloat_(&response, v);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdVppInitCal:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppInitCal();
            break;
        case kCmdVppSaveCal:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppSaveCal(getParamAsFloat_());
            break;
        case kCmdVppOnA9:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppOnA9(getParamAsBool_());
            break;
        case kCmdVppOnA18:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppOnA18(getParamAsBool_());
            break;
        case kCmdVppOnCE:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppOnCE(getParamAsBool_());
            break;
        case kCmdVppOnOE:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppOnOE(getParamAsBool_());
            break;
        case kCmdVppOnWE:
            serial_.putChar(kCmdResponseOk, true);
            device_.vppOnWE(getParamAsBool_());
            break;
        default:
            break;
    }
}

void Runner::runCtrlBusCommand_(uint8_t opcode) {
    switch (opcode) {
        case kCmdBusCE:
            serial_.putChar(kCmdResponseOk, true);
            device_.setCE(getParamAsBool_());
            break;
        case kCmdBusOE:
            serial_.putChar(kCmdResponseOk, true);
            device_.setOE(getParamAsBool_());
            break;
        case kCmdBusWE:
            serial_.putChar(kCmdResponseOk, true);
            device_.setWE(getParamAsBool_());
            break;
        default:
            break;
    }
}

void Runner::runAddrBusCommand_(uint8_t opcode) {
    switch (opcode) {
        case kCmdBusAddrClr:
            if (device_.addrClr()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrInc:
            if (device_.addrInc()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrSet:
            if (device_.addrSet(getParamAsDWord_())) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrSetB:
            if (device_.addrSetB(getParamAsByte_())) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusAddrSetW:
            if (device_.addrSetW(getParamAsWord_())) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
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
            if (device_.dataClr()) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusDataSet:
            if (device_.dataSet(getParamAsByte_())) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusDataSetW:
            if (device_.dataSetW(getParamAsWord_())) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdBusDataGet:
            w = device_.dataGet();
            response.resize(2);
            response[0] = kCmdResponseOk;
            createParamsFromByte_(&response, w);
            serial_.putBuf(response.data(), response.size());
            break;
        case kCmdBusDataGetW:
            w = device_.dataGetW();
            response.resize(3);
            response[0] = kCmdResponseOk;
            createParamsFromWord_(&response, w);
            serial_.putBuf(response.data(), response.size());
            break;
        default:
            break;
    }
}

void Runner::runDeviceSettingsCommand_(uint8_t opcode) {
    switch (opcode) {
        case kCmdDeviceSetTwp:
            serial_.putChar(kCmdResponseOk, true);
            device_.setTwp(getParamAsDWord_());
            break;
        case kCmdDeviceSetTwc:
            serial_.putChar(kCmdResponseOk, true);
            device_.setTwc(getParamAsDWord_());
            break;
        case kCmdDeviceSetFlags:
            serial_.putChar(kCmdResponseOk, true);
            device_.setFlags(getParamAsByte_());
            break;
        case kCmdDeviceSetupBus:
            if (device_.setupBus(getParamAsByte_())) {
                serial_.putChar(kCmdResponseOk, true);
                sleep_ms(kStabilizationTime);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceReadCommand_(uint8_t opcode) {
    TByteArray response;
    uint8_t blockSize;
    switch (opcode) {
        case kCmdDeviceRead:
            blockSize = getParamAsByte_();
            response = device_.read(blockSize);
            if (response.size() == blockSize) {
                response.insert(response.begin(), kCmdResponseOk);
                serial_.putBuf(response.data(), response.size());
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceReadW:
            blockSize = getParamAsByte_();
            response = device_.readW(blockSize / 2);
            if (response.size() == blockSize) {
                response.insert(response.begin(), kCmdResponseOk);
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
    TByteArray buffer;
    uint16_t sectorSize;
    switch (opcode) {
        case kCmdDeviceWrite:
            sectorSize = getParamAsByte_();
            buffer = readByte_(sectorSize);
            if (device_.write(buffer, sectorSize, true)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceWriteW:
            sectorSize = getParamAsByte_();
            buffer = readByte_(sectorSize);
            if (device_.writeW(buffer, sectorSize / 2, true)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceWriteSector:
            sectorSize = getParamAsWord_();
            buffer = readByte_(sectorSize);
            if (device_.writeSector(buffer, sectorSize, true)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceWriteSectorW:
            sectorSize = getParamAsWord_();
            buffer = readByte_(sectorSize);
            if (device_.writeSectorW(buffer, sectorSize / 2, true)) {
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
    TByteArray buffer;
    uint8_t blockSize;
    switch (opcode) {
        case kCmdDeviceVerify:
            blockSize = getParamAsByte_();
            buffer = readByte_(blockSize);
            if (device_.verify(buffer, blockSize)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceVerifyW:
            blockSize = getParamAsByte_();
            buffer = readByte_(blockSize);
            if (device_.verifyW(buffer, blockSize / 2)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceBlankCheck:
            blockSize = getParamAsByte_();
            if (device_.blankCheck(blockSize)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceBlankCheckW:
            blockSize = getParamAsByte_();
            if (device_.blankCheckW(blockSize / 2)) {
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceGetIdCommand_(uint8_t opcode) {
    uint16_t w;
    TByteArray response;
    switch (opcode) {
        case kCmdDeviceGetId:
            if (device_.getId(w)) {
                // response
                response.resize(3);
                response[0] = kCmdResponseOk;
                createParamsFromWord_(&response, w);
                serial_.putBuf(response.data(), response.size());
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceEraseCommand_(uint8_t opcode) {
    uint8_t algo;
    switch (opcode) {
        case kCmdDeviceErase:
            algo = getParamAsByte_();
            if (device_.erase(algo)) {
                // response
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
}

void Runner::runDeviceProtectCommand_(uint8_t opcode) {
    uint8_t algo;
    switch (opcode) {
        case kCmdDeviceProtect:
            algo = getParamAsByte_();
            if (device_.protect(algo)) {
                // response
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        case kCmdDeviceUnprotect:
            algo = getParamAsByte_();
            if (device_.unprotect(algo)) {
                // response
                serial_.putChar(kCmdResponseOk);
            } else {
                serial_.putChar(kCmdResponseNok);
            }
            break;
        default:
            break;
    }
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