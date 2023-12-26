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
    size_ = 2048;
}

SRAM::~SRAM() {}

bool SRAM::program(const QByteArray &buffer, bool verify) {
    (void)buffer;
    (void)verify;
    uint32_t current = 0, total = size_ * 4;
    bool error = true;
    if (runner_.open(port_)) {
        error = false;
        resetBus_();
        runner_.vddSet(vddRd_);
        runner_.vddCtrl();
        runner_.setCE();
        runner_.usDelay(30);  // 30 uS
        if (!doPatternTest_(current, total) || !doRandomTest_(current, total)) {
            error = true;
        }
        resetBus_();
        if (!error) {
            error = runner_.hasError();
            if (error) {
                // fail (error)
                emit onProgress(total, total, true, false);
            }
        }
        runner_.close();
    } else {
        // fail (no open port)
        emit onProgress(0, total, true, false);
    }
    if (!error) {
        // success (done)
        emit onProgress(total, total, true);
    }
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
    runner_.usDelay(25);  // 25 uS
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
    for (const auto &data : buffer) {
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
    for (const auto &data : buffer) {
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
