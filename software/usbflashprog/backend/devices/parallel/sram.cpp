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

#include <QByteArray>
#include <QRandomGenerator>
#include <QLoggingCategory>

#include "backend/devices/parallel/sram.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(deviceParSram, "device.parallel.sram")

#define DEBUG qCDebug(deviceParSram)
#define INFO qCInfo(deviceParSram)
#define WARNING qCWarning(deviceParSram)
#define CRITICAL qCCritical(deviceParSram)
#define FATAL qCFatal(deviceParSram)

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
    DEBUG << info_.toString();
}

SRAM::~SRAM() {}

bool SRAM::program(const QByteArray &buffer, bool verify) {
    (void)buffer;
    (void)verify;
    INFO << "Programming device...";
    canceling_ = false;
    uint32_t current = 0, total = size_;
    if (!initialize_(kDeviceOpRead)) {
        WARNING << "Error programming device";
        return false;
    }
    bool error = false;
    if (!doPatternTest_() || !doRandomTest_()) error = true;
    if (!error && runner_.hasError()) {
        return finalize_(total, total, true, false);
    }
    if (!error) emit onProgress(total, total, true);
    finalize_();
    if (error) {
        WARNING << "Error programming device";
    } else {
        INFO << "Programming device OK";
    }
    return !error;
}

bool SRAM::doPatternTest_() {
    QByteArray buffer = generatePatternData_();
    runner_.addrClr();
    uint32_t current = 0;
    if (!program_(buffer, current, size_)) return false;
    runner_.addrClr();
    current = 0;
    if (!verify_(buffer, current, size_)) return false;
    return !runner_.hasError();
}

bool SRAM::doRandomTest_() {
    QByteArray buffer = generateRandomData_();
    runner_.addrClr();
    uint32_t current = 0;
    if (!program_(buffer, current, size_)) return false;
    runner_.addrClr();
    current = 0;
    if (!verify_(buffer, current, size_)) return false;
    return !runner_.hasError();
}

bool SRAM::resetBus_() {
    DEBUG << "Reseting Bus...";
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
    if (runner_.hasError()) {
        WARNING << "Error in Bus Reset";
    } else {
        DEBUG << "Bus Reset OK";
    }
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
    DEBUG << "Programming data...";
    for (const uint8_t &data : buffer) {
        emit onProgress(current, total);
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Program canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        uint8_t read;
        write_(data);
        read_(read);
        if (runner_.hasError() || read != data) {
            emit onProgress(current, total, true, false);
            WARNING << QString("Program error at 0x%1 of 0x%2")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'));
            if (runner_.hasError()) {
                WARNING << "Runner status is Error";
            } else {
                WARNING << QString("Data to write 0x%1. Data read 0x%2")
                               .arg(data, 2, 16, QChar('0'))
                               .arg(read, 2, 16, QChar('0'));
            }
            return false;
        }
        runner_.addrInc();
        current++;
    }
    DEBUG << "Program OK";
    return true;
}

bool SRAM::verify_(const QByteArray &buffer, uint32_t &current,
                   uint32_t total) {
    DEBUG << "Verifying data...";
    for (const uint8_t &data : buffer) {
        emit onProgress(current, total);
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Verify canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        uint8_t read;
        read_(read);
        if (runner_.hasError() || read != data) {
            emit onProgress(current, total, true, false);
            WARNING << QString("Verify error at 0x%1 of 0x%2")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'));
            if (runner_.hasError()) {
                WARNING << "Runner status is Error";
            } else {
                WARNING << QString("Data to write 0x%1. Data read 0x%2")
                               .arg(data, 2, 16, QChar('0'))
                               .arg(read, 2, 16, QChar('0'));
            }
            return false;
        }
        current++;
        runner_.addrInc();
    }
    DEBUG << "Verify OK";
    return true;
}

bool SRAM::initialize_(kDeviceOperation operation) {
    DEBUG << "Initializing...";
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
        WARNING << "Error opening Serial Port.";
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
    DEBUG << "Initialize OK";
    return true;
}

void SRAM::finalize_() {
    DEBUG << "Finalizing...";
    resetBus_();
    runner_.close();
    DEBUG << "Finalize OK";
}

bool SRAM::finalize_(uint32_t current, uint32_t total, bool done, bool success,
                     bool canceled) {
    finalize_();
    emit onProgress(current, total, done, success, canceled);
    return success;
}

QByteArray SRAM::generateRandomData_() {
    DEBUG << "Generating Random Data...";
    QByteArray buffer(size_, 0);
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
    return buffer;
}

QByteArray SRAM::generatePatternData_() {
    DEBUG << "Generating Pattern Data...";
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
