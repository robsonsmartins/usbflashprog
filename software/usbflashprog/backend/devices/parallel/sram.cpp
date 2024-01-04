// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2023) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/parallel/sram.cpp
 * @brief Implementation of a Parallel SRAM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "backend/devices/parallel/sram.hpp"

#include <QByteArray>
#include <QRandomGenerator>

// ---------------------------------------------------------------------------

SRAM::SRAM(QObject *parent) : Device(parent) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "SRAM";
    info_.capability.hasProgram = true;
    info_.capability.hasVDD = true;
    twp_ = 3;
    twc_ = 5;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    size_ = 2048;
    resetBusDelay_ = 25;  // 25 uS
    initDelay_ = 30;      // 30 uS
}

SRAM::~SRAM() {}

bool SRAM::program(const QByteArray &buffer, bool verify) {
    (void)buffer;
    (void)verify;
    canceling_ = false;
    uint32_t current = 0, total = size_ * 4;
    if (!initialize_(kDeviceOpRead)) return false;
    bool error = false;
    if (!doPatternTest_(current, total) || !doRandomTest_(current, total)) {
        error = true;
    }
    if (!error && runner_.hasError()) {
        return finalize_(total, total, true, false);
    }
    if (!error) emit onProgress(total, total, true);
    finalize_();
    return !error;
}

bool SRAM::doPatternTest_(uint32_t &current, uint32_t total) {
    QByteArray buffer = generatePatternData_();
    runner_.addrClr();
    if (!program_(buffer, current, total)) return false;
    runner_.addrClr();
    if (!verify_(buffer, current, total)) return false;
    return !runner_.hasError();
}

bool SRAM::doRandomTest_(uint32_t &current, uint32_t total) {
    QByteArray buffer = generateRandomData_();
    runner_.addrClr();
    if (!program_(buffer, current, total)) return false;
    runner_.addrClr();
    if (!verify_(buffer, current, total)) return false;
    return !runner_.hasError();
}

bool SRAM::resetBus_() {
    runner_.vddCtrl(false);
    runner_.vppCtrl(false);
    runner_.addrClr();
    runner_.setCE(false);
    runner_.setOE(false);
    runner_.setWE(false);
    runner_.vppOnA9(false);
    runner_.vppOnA18(false);
    runner_.vppOnCE(false);
    runner_.vppOnOE(false);
    runner_.vppOnWE(false);
    runner_.dataClr();
    runner_.usDelay(resetBusDelay_);
    return !runner_.hasError();
}

bool SRAM::write_(uint8_t data) {
    runner_.dataSet(data);
    runner_.setWE(true);
    runner_.usDelay(twp_);  // tWP uS
    runner_.setWE(false);
    runner_.usDelay(twc_);  // tWC uS
    return !runner_.hasError();
}

bool SRAM::read_(uint8_t &data) {
    runner_.setOE(true);
    data = runner_.dataGet();
    runner_.setOE(false);
    return !runner_.hasError();
}

bool SRAM::program_(const QByteArray &buffer, uint32_t &current,
                    uint32_t total) {
    for (const uint8_t &data : buffer) {
        emit onProgress(current, total);
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            return false;
        }
        uint8_t read;
        write_(data);
        read_(read);
        runner_.addrInc();
        if (runner_.hasError() || read != data) {
            emit onProgress(current, total, true, false);
            return false;
        }

        current++;
    }
    return true;
}

bool SRAM::verify_(const QByteArray &buffer, uint32_t &current,
                   uint32_t total) {
    for (const uint8_t &data : buffer) {
        emit onProgress(current, total);
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            return false;
        }
        uint8_t read;
        read_(read);
        runner_.addrInc();
        if (runner_.hasError() || read != data) {
            emit onProgress(current, total, true, false);
            return false;
        }
        current++;
    }
    return true;
}

bool SRAM::initialize_(kDeviceOperation operation) {
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
        return false;
    }
    resetBus_();
    switch (operation) {
        case kDeviceOpProg:
        case kDeviceOpGetId:
            runner_.vddSet(vddWr_);
            runner_.vddCtrl();
            runner_.setCE();
            runner_.vppSet(vpp_);
            runner_.vppCtrl();
            break;
        case kDeviceOpRead:
        default:
            runner_.vddSet(vddRd_);
            runner_.vddCtrl();
            runner_.setCE();
            runner_.vddOnVpp();
            break;
    }
    runner_.usDelay(initDelay_);
    if (runner_.hasError()) return finalize_(0, size_, true, false);
    return true;
}

void SRAM::finalize_() {
    resetBus_();
    runner_.close();
}

bool SRAM::finalize_(uint32_t current, uint32_t total, bool done, bool success,
                     bool canceled) {
    finalize_();
    emit onProgress(current, total, done, success, canceled);
    return success;
}

QByteArray SRAM::generateRandomData_() {
    QByteArray buffer(size_, 0);
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
    return buffer;
}

QByteArray SRAM::generatePatternData_() {
    QByteArray buffer(size_, 0);
    uint8_t data = 0x55;
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(data);
        data = ~data;
    }
    return buffer;
}

bool SRAM::getId(TDeviceID &result) {
    (void)result;
    return false;
}

bool SRAM::read(QByteArray &buffer) {
    (void)buffer;
    return false;
}

bool SRAM::verify(const QByteArray &buffer) {
    (void)buffer;
    return false;
}

bool SRAM::erase(bool check) {
    (void)check;
    return false;
}

bool SRAM::blankCheck() {
    return false;
}

bool SRAM::unprotect() {
    return false;
}
