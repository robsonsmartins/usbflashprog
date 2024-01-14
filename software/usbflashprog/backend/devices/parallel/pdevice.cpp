// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/parallel/pdevice.cpp
 * @brief Implementation of a Parallel Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QByteArray>
#include <QRandomGenerator>
#include <QLoggingCategory>

#include "backend/devices/parallel/pdevice.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(devicePar, "device.parallel")

#define DEBUG qCDebug(devicePar)
#define INFO qCInfo(devicePar)
#define WARNING qCWarning(devicePar)
#define CRITICAL qCCritical(devicePar)
#define FATAL qCFatal(devicePar)

// ---------------------------------------------------------------------------

ParDevice::ParDevice(QObject *parent) : Device(parent) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "Parallel Device";
    size_ = 2048;
    resetBusDelay_ = 25;  // 25 uS
    initDelay_ = 30;      // 30 uS
    DEBUG << info_.toString();
}

ParDevice::~ParDevice() {}

bool ParDevice::programDevice(const QByteArray &buffer) {
    DEBUG << "Programming data...";
    uint32_t current = 0;
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (is16bit_) total /= 2;
    int increment = is16bit_ ? 2 : 1;
    if (!runner_.deviceSetFlags(flags_) || !runner_.deviceSetTwp(twp_) ||
        !runner_.deviceSetTwc(twc_)) {
        emit onProgress(current, total, true, false);
        WARNING << "Program error: setting tWP, tWC or device flags";
        return false;
    }
    for (int i = 0; i < buffer.size(); i += increment) {
        if (i % 0x100 == 0) emit onProgress(current, total);
        runner_.processEvents();
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Program canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        uint16_t data = buffer[i] & 0xFF;
        if (is16bit_) {
            data <<= 8;                      // MSB
            data |= (buffer[i + 1] & 0xFF);  // LSB
        }
        // Repeat for n max attempts
        for (int j = 1; j <= maxAttemptsProg_; j++) {
            // Write (and verify) data
            if (writeData_(data)) break;  // Success: break
            // Error
            if (j == maxAttemptsProg_) {
                emit onProgress(current, total, true, false);
                WARNING << QString(
                               "Program error at 0x%1 of 0x%2. Data to "
                               "write 0x%3")
                               .arg(current, 6, 16, QChar('0'))
                               .arg(total, 6, 16, QChar('0'))
                               .arg(data, 2, 16, QChar('0'));
                return false;
            }
        }
        current++;
    }
    DEBUG << "Program OK";
    return true;
}

bool ParDevice::verifyDevice(const QByteArray &buffer) {
    DEBUG << "Verifying data...";
    uint32_t current = 0;
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (is16bit_) total /= 2;
    int increment = is16bit_ ? 2 : 1;
    if (!runner_.deviceSetFlags(flags_)) {
        emit onProgress(current, total, true, false);
        WARNING << "Verify error: setting device flags";
        return false;
    }
    for (int i = 0; i < buffer.size(); i += increment) {
        if (i % 0x100 == 0) emit onProgress(current, total);
        runner_.processEvents();
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Verify canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        uint16_t data = buffer[i] & 0xFF;
        if (is16bit_) {
            data <<= 8;                      // MSB
            data |= (buffer[i + 1] & 0xFF);  // LSB
        }
        if (!verifyData_(data)) {
            emit onProgress(current, total, true, false);
            WARNING << QString(
                           "Verify error at 0x%1 of 0x%2. Expected data 0x%3")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'))
                           .arg(data, 2, 16, QChar('0'));
            return false;
        }
        current++;
    }
    DEBUG << "Verify OK";
    return true;
}

bool ParDevice::readDevice(QByteArray &buffer) {
    DEBUG << "Reading data...";
    uint32_t current = 0;
    uint32_t total = size_;
    if (is16bit_) total /= 2;
    int increment = is16bit_ ? 2 : 1;
    if (!runner_.deviceSetFlags(flags_)) {
        emit onProgress(current, total, true, false);
        WARNING << "Verify error: setting device flags";
        return false;
    }
    buffer.clear();
    for (current = 0; current < total; current++) {
        if (current % 0x100 == 0) emit onProgress(current, total);
        runner_.processEvents();
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Read canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        uint16_t data = 0xFFFF;
        if (!readData_(data)) {
            emit onProgress(current, total, true, false);
            WARNING << QString("Read error at 0x%1 of 0x%2")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'));
            return false;
        }
        if (is16bit_) {
            uint8_t msb = (data & 0xFF00) >> 8;
            uint8_t lsb = data & 0xFF;
            buffer.append(msb);
            buffer.append(lsb);
        } else {
            buffer.append(data & 0xFF);
        }
    }
    DEBUG << "Read OK";
    return true;
}

bool ParDevice::initDevice(kDeviceOperation operation) {
    DEBUG << "Initializing...";
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
        WARNING << "Error opening Serial Port";
        return false;
    }
    // Init bus and pins
    resetBus_();
    switch (operation) {
        case kDeviceOpProg:
            // VDD Wr on
            runner_.vddSet(vddWr_);
            runner_.vddCtrl();
            initControlPins_(false);
            // VPP set
            runner_.vppSet(vpp_);
            break;
        case kDeviceOpGetId:
            // VDD Rd on
            runner_.vddSet(vddRd_);
            runner_.vddCtrl();
            initControlPins_(true);
            // VPP set with VEE
            runner_.vppSet(vee_);
            if (!flags_.vppOePin) {
                // VDD Rd on VPP
                runner_.vddOnVpp();
            }
            // ~OE is LO
            runner_.setOE();
            // VPP on A9
            runner_.vppOnA9();
            break;
        case kDeviceOpRead:
        default:
            // VDD Rd on
            runner_.vddSet(vddRd_);
            runner_.vddCtrl();
            initControlPins_(true);
            // VDD Rd on VPP
            runner_.vddOnVpp();
            break;
    }
    runner_.usDelay(initDelay_);
    if (runner_.hasError()) return finalizeDevice(0, size_, true, false);
    DEBUG << "Initialize OK";
    return true;
}

void ParDevice::finalizeDevice() {
    DEBUG << "Finalizing...";
    resetBus_();
    runner_.close();
    DEBUG << "Finalize OK";
}

bool ParDevice::finalizeDevice(uint32_t current, uint32_t total, bool done,
                               bool success, bool canceled) {
    finalizeDevice();
    emit onProgress(current, total, done, success, canceled);
    return success;
}

bool ParDevice::readData_(uint16_t &data) {
    if (is16bit_) {
        data = runner_.deviceReadW();
    } else {
        data = runner_.deviceRead();
    }
    return !runner_.hasError();
}

bool ParDevice::writeData_(uint16_t data) {
    if (is16bit_) {
        return runner_.deviceWriteW(data);
    } else {
        return runner_.deviceWrite(data & 0xFF);
    }
}

bool ParDevice::verifyData_(uint16_t data) {
    if (is16bit_) {
        return runner_.deviceVerifyW(data);
    } else {
        return runner_.deviceVerify(data & 0xFF);
    }
}

bool ParDevice::resetBus_() {
    DEBUG << "Reseting Bus...";
    // VDD off and VPP off
    runner_.vddCtrl(false);
    runner_.vppCtrl(false);
    // Clear AddrBus
    runner_.addrClr();
    // ~OE is HI
    runner_.setOE(false);
    // ~CE is HI
    runner_.setCE(false);
    if (flags_.pgmPositive) {
        // PGM is LO (no prog pulse)
        runner_.setWE();
    } else {
        // ~PGM is HI (no prog pulse)
        runner_.setWE(false);
    }
    // VPP on xx disabled
    runner_.vppOnA9(false);
    runner_.vppOnA18(false);
    runner_.vppOnCE(false);
    runner_.vppOnOE(false);
    runner_.vppOnWE(false);
    // Clear DataBus
    runner_.dataClr();
    runner_.usDelay(resetBusDelay_);
    if (runner_.hasError()) {
        WARNING << "Error in Bus Reset";
    } else {
        DEBUG << "Bus Reset OK";
    }
    return !runner_.hasError();
}

void ParDevice::initControlPins_(bool read) {
    // ~CE is LO (if pin connected)
    runner_.setCE();
    if (read) {
        // Read
        if (flags_.pgmCePin) {
            // PGM/~CE is LO
            runner_.setWE();
        } else {
            // ~PGM is HI
            runner_.setWE(false);
        }
    } else {
        // Prog
        if (flags_.pgmPositive) {
            // PGM is LO
            runner_.setWE();
        } else {
            // ~PGM is HI
            runner_.setWE(false);
        }
    }
    // ~OE is HI
    runner_.setOE(false);
}

QByteArray ParDevice::generateRandomData_() {
    DEBUG << "Generating Random Data...";
    QByteArray buffer(size_, 0);
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
    return buffer;
}

QByteArray ParDevice::generatePatternData_() {
    DEBUG << "Generating Pattern Data...";
    QByteArray buffer(size_, 0);
    uint8_t data = 0x55;
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(data);
        data = ~data;
    }
    return buffer;
}
