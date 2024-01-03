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
 * @file backend/devices/parallel/eprom.cpp
 * @brief Implementation of the Parallel EPROM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "backend/devices/parallel/eprom.hpp"

// ---------------------------------------------------------------------------

EPROM::EPROM(QObject *parent) : SRAM(parent), mode16bit_(false) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "EPROM";
    info_.capability.hasRead = true;
    info_.capability.hasProgram = true;
    info_.capability.hasVerify = true;
    info_.capability.hasBlankCheck = true;
    info_.capability.hasGetId = true;
    info_.capability.hasVDD = true;
    info_.capability.hasVPP = true;
    skipFF_ = true;
    twp_ = 600;
    twc_ = 8;
    vddRd_ = 5.0f;
    vddWr_ = 6.0f;
    vpp_ = 13.0f;
    vee_ = 13.0f;
    size_ = 2048;
    maxAttemptsProg_ = 25;
    vppPulseDelay_ = 150;
}

EPROM::~EPROM() {}

bool EPROM::read(QByteArray &buffer) {
    canceling_ = false;
    int total = static_cast<int>(size_);
    // Init pins/bus to Read operation
    if (!initialize_(kDeviceOpRead)) return false;
    buffer.clear();
    uint16_t data = 0xFFFF;
    // Read entire device
    for (int current = 0; current < total; ++current) {
        emit onProgress(current, total);
        if (canceling_) return finalize_(current, total, true, false, true);
        // Read byte
        if (!read_(data) || runner_.hasError())
            return finalize_(current, total, true, false);
        if (mode16bit_) {
            uint8_t msb = (data & 0xFF00) >> 8;
            uint8_t lsb = data & 0xFF;
            buffer.append(msb);
            buffer.append(lsb);
        } else {
            buffer.append(data & 0xFF);
        }
        // Increment Address
        runner_.addrInc();
    }
    // Close resources
    return finalize_(total, total, true);
}

bool EPROM::program(const QByteArray &buffer, bool verify) {
    canceling_ = false;
    int total = qMin(static_cast<int>(size_), buffer.size());
    // Init pins/bus to Prog operation
    if (!initialize_(kDeviceOpProg)) return false;
    uint32_t current = 0;
    bool error = false;
    // Program the device
    if (!program_(buffer, current, total)) error = true;
    // If no error and verify flag is enabled, verify device
    if (!error && verify) {
        // VDD off and VPP off
        runner_.vppCtrl(false);
        runner_.vddCtrl(false);
        // VDD Rd on
        runner_.vddSet(vddRd_);
        runner_.vddCtrl();
        // VDD Rd on VPP
        runner_.vddOnVpp();
        // Clear AddrBus
        runner_.addrClr();
        initControlPins_(true);
        runner_.usDelay(initDelay_);
        if (runner_.hasError()) return finalize_(0, total, true, false);
        current = 0;
        // Read the device and verify (compare) buffer
        if (!verify_(buffer, current, total)) error = true;
    }
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalize_();
    return !error;
}

bool EPROM::verify(const QByteArray &buffer) {
    canceling_ = false;
    int total = static_cast<int>(size_);
    // Init pins/bus to Read operation
    if (!initialize_(kDeviceOpRead)) return false;
    uint32_t current = 0;
    bool error = false;
    // Read the device and verify (compare) buffer
    if (!verify_(buffer, current, total)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalize_();
    return !error;
}

bool EPROM::blankCheck() {
    // Create a blank (0xFF) buffer
    QByteArray data = QByteArray(size_, (char)0xFF);
    // Verify
    return verify(data);
}

bool EPROM::getId(TDeviceID &result) {
    canceling_ = false;
    int total = static_cast<int>(size_);
    // Init pins/bus to GetId operation
    if (!initialize_(kDeviceOpGetId)) return false;
    bool error = false;
    // Get manufacturer data (byte)
    result.manufacturer = runner_.dataGet();
    if (runner_.hasError()) error = true;
    // Increment Address (0x01)
    runner_.addrInc();
    // Get device data (byte)
    result.device = runner_.dataGet();
    // Error
    if (runner_.hasError()) return finalize_(0, total, true, false);
    // Success
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalize_();
    return (!error);
}

bool EPROM::isOeVppPin_() {
    switch (size_) {
        case 0x1000:    //  4KB
        case 0x10000:   // 64KB
        case 0x100000:  //  1MB
        case 0x400000:  //  4MB
            // ~OE/VPP Pin
            return true;
        default:
            // ~OE and VPP Pin are separated
            return false;
    }
}

bool EPROM::isPositiveProgPulse_() {
    if (size_ == 0x800) {  // 2KB
        // Positive prog pulse (HI)
        return true;
    } else {
        // Normal negative prog pulse (LO)
        return false;
    }
}

bool EPROM::isPgmCePin_() {
    switch (size_) {
        case 0x2000:   //   8KB
        case 0x4000:   //  16KB
        case 0x20000:  // 128KB
        case 0x40000:  // 256KB
            // ~CE and ~PGM Pin are separated
            return false;
        default:
            // ~CE/PGM Pin
            return true;
    }
}

bool EPROM::resetBus_() {
    // VDD off and VPP off
    runner_.vddCtrl(false);
    runner_.vppCtrl(false);
    // Clear AddrBus
    runner_.addrClr();
    // ~OE is HI
    runner_.setOE(false);
    // ~CE is HI
    runner_.setCE(false);
    if (isPositiveProgPulse_()) {
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
    return !runner_.hasError();
}

bool EPROM::read_(uint16_t &data) {
    if (isOeVppPin_()) {
        // ~OE/VPP is LO
        runner_.vddOnVpp(false);
    }
    // ~OE is LO
    runner_.setOE();
    if (mode16bit_) {
        data = runner_.dataGetW();
    } else {
        data = runner_.dataGet();
    }
    // ~OE is HI
    runner_.setOE(false);
    if (isOeVppPin_()) {
        // ~OE/VPP is VDD
        runner_.vddOnVpp();
    }
    return !runner_.hasError();
}

bool EPROM::write_(uint16_t data) {
    // VPP on
    runner_.vddOnVpp(false);
    runner_.vppCtrl();
    runner_.usDelay(vppPulseDelay_);
    // Set DataBus
    if (mode16bit_) {
        runner_.dataSetW(data);
    } else {
        runner_.dataSet(data & 0xFF);
    }
    if (isPositiveProgPulse_()) {
        // PGM is HI (start prog pulse)
        runner_.setWE(false);
    } else {
        // ~PGM is LO (start prog pulse)
        runner_.setWE();
    }
    runner_.usDelay(twp_);  // tWP uS
    if (isPositiveProgPulse_()) {
        // PGM is LO (end prog pulse)
        runner_.setWE();
    } else {
        // ~PGM is HI (end prog pulse)
        runner_.setWE(false);
    }
    runner_.usDelay(twc_);  // tWC uS
    // VPP off
    runner_.vppCtrl(false);
    // VPP is VDD
    runner_.vddOnVpp();
    runner_.usDelay(vppPulseDelay_);
    return !runner_.hasError();
}

bool EPROM::initialize_(kDeviceOperation operation) {
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
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
            if (!isOeVppPin_()) {
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
    if (runner_.hasError()) return finalize_(0, size_, true, false);
    return true;
}

void EPROM::initControlPins_(bool read) {
    // ~CE is LO (if pin connected)
    runner_.setCE();
    if (read) {
        // Read
        if (isPgmCePin_()) {
            // PGM/~CE is LO
            runner_.setWE();
        } else {
            // ~PGM is HI
            runner_.setWE(false);
        }
    } else {
        // Prog
        if (isPositiveProgPulse_()) {
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

bool EPROM::program_(const QByteArray &buffer, uint32_t &current,
                     uint32_t total) {
    uint16_t data, read = 0xFFFF;
    int increment = mode16bit_ ? 2 : 1;
    uint16_t empty = mode16bit_ ? 0xFFFF : 0xFF;
    // Program entire device
    for (int i = 0; i < buffer.size(); i += increment) {
        emit onProgress(current, total);
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            return false;
        }
        data = buffer[i];
        if (mode16bit_) {
            data <<= 8;             // MSB
            data |= buffer[i + 1];  // LSB
        }
        // Repeat for n max attempts
        for (int j = 1; j <= maxAttemptsProg_; j++) {
            // Write byte (if not 0xFF)
            if (data != empty) write_(data);
            // Read byte
            read_(read);
            // Verify
            if (read == data) break;
            if (runner_.hasError() || j == maxAttemptsProg_ || data == empty) {
                // Error
                emit onProgress(current, total, true, false);
                return false;
            }
        }
        // Increment Address
        runner_.addrInc();
        current += increment;
    }
    return true;
}

bool EPROM::verify_(const QByteArray &buffer, uint32_t &current,
                    uint32_t total) {
    uint16_t data, read = 0xFFFF;
    int increment = mode16bit_ ? 2 : 1;
    for (int i = 0; i < buffer.size(); i += increment) {
        emit onProgress(current, total);
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            return false;
        }
        data = buffer[i];
        if (mode16bit_) {
            data <<= 8;             // MSB
            data |= buffer[i + 1];  // LSB
        }
        read_(read);
        runner_.addrInc();
        if (runner_.hasError() || read != data) {
            emit onProgress(current, total, true, false);
            return false;
        }
        current += increment;
    }
    return true;
}

// ---------------------------------------------------------------------------

M27xxx::M27xxx(QObject *parent) : EPROM(parent) {
    info_.name = "EPROM 27xxx";
    twp_ = 50000;
    twc_ = 50;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 25.0f;
    vee_ = 12.0f;
    size_ = 2048;
}

M27xxx::~M27xxx() {}

// ---------------------------------------------------------------------------

M27Cxxx::M27Cxxx(QObject *parent) : EPROM(parent) {
    info_.name = "EPROM 27Cxxx";
    twp_ = 500;
    twc_ = 8;
    vddRd_ = 5.0f;
    vddWr_ = 6.0f;
    vpp_ = 13.0f;
    vee_ = 12.0f;
    size_ = 2048;
}

M27Cxxx::~M27Cxxx() {}

// ---------------------------------------------------------------------------

M27C16Bit::M27C16Bit(QObject *parent) : M27Cxxx(parent) {
    info_.name = "EPROM 27Cxxx (16Bit)";
    mode16bit_ = true;
}

M27C16Bit::~M27C16Bit() {}

// ---------------------------------------------------------------------------

W27Exxx::W27Exxx(QObject *parent) : M27Cxxx(parent) {
    info_.name = "EPROM W27C/27E/27SF";
    info_.capability.hasErase = true;
    info_.capability.hasBlankCheck = true;
    twp_ = 100;
    twc_ = 15;
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 12.0f;
    vee_ = 14.0f;
    size_ = 2048;
    erasePulseDelay_ = 100;  // 100 ms
}

W27Exxx::~W27Exxx() {}

bool W27Exxx::erase(bool check) {
    canceling_ = false;
    int total = static_cast<int>(size_);
    // Init pins/bus to Prog operation
    if (!initialize_(kDeviceOpErase)) return false;
    uint32_t current = 0;
    bool error = false;
    // Erase the device
    if (!erase_(current, total)) error = true;
    // If no error and check flag is enabled, verify device
    if (!error && check) {
        // VDD off and VPP off
        runner_.vppCtrl(false);
        runner_.vddCtrl(false);
        // VDD Rd on
        runner_.vddSet(vddRd_);
        runner_.vddCtrl();
        // VDD Rd on VPP
        runner_.vddOnVpp();
        // Clear AddrBus
        runner_.addrClr();
        initControlPins_(true);
        runner_.usDelay(initDelay_);
        if (runner_.hasError()) return finalize_(0, total, true, false);
        current = 0;
        // Creates a blank (0xFF) buffer
        QByteArray buffer(size_, 0xFF);
        // Read the device and verify (compare) buffer
        if (!verify_(buffer, current, total)) error = true;
    }
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalize_();
    return !error;
}

bool W27Exxx::blankCheck() {
    canceling_ = false;
    int total = static_cast<int>(size_);
    // Init pins/bus to Read operation
    if (!initialize_(kDeviceOpRead)) return false;
    uint32_t current = 0;
    bool error = false;
    // Creates a blank (0xFF) buffer
    QByteArray buffer(size_, 0xFF);
    // Read the device and verify (compare) buffer
    if (!verify_(buffer, current, total)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalize_();
    return !error;
}

bool W27Exxx::initialize_(kDeviceOperation operation) {
    if (operation != kDeviceOpErase) {
        return M27Cxxx::initialize_(operation);
    }
    // Operation == Erase
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
        return false;
    }
    // Init bus and pins
    resetBus_();
    // VDD Wr on
    runner_.vddSet(vddWr_);
    runner_.vddCtrl();
    initControlPins_(false);
    // VPP set with VEE
    // VPP on
    runner_.vppSet(vee_);
    runner_.vppCtrl();
    // VPP on A9
    runner_.vppOnA9();
    // Data is 0xFF
    if (mode16bit_) {
        runner_.dataSetW(0xFFFF);
    } else {
        runner_.dataSet(0xFF);
    }
    runner_.usDelay(initDelay_);
    if (runner_.hasError()) return finalize_(0, size_, true, false);
    return true;
}

bool W27Exxx::erase_(uint32_t &current, uint32_t total) {
    uint16_t read = 0xFFFF;
    int increment = mode16bit_ ? 2 : 1;
    uint16_t empty = mode16bit_ ? 0xFFFF : 0xFF;
    uint32_t initial = current;
    // Repeat for n max attempts
    for (int j = 1; j <= maxAttemptsProg_; j++) {
        current = initial;
        // Erase entire chip
        // Addr = 0
        runner_.addrClr();
        // Data = 0xFF
        if (mode16bit_) {
            runner_.dataSetW(0xFFFF);
        } else {
            runner_.dataSet(0xFF);
        }
        // VPP on A9
        runner_.vppOnA9();
        // ~PGM is LO (start erase pulse)
        runner_.setWE();
        runner_.msDelay(erasePulseDelay_);
        // ~PGM is HI (end erase pulse)
        runner_.setWE(false);
        runner_.usDelay(twc_);
        // VPP on A9 off
        runner_.vppOnA9(false);
        bool success = true;
        for (int i = 0; i < size_; i += increment) {
            // Read byte
            read_(read);
            // Verify
            if (read != empty) {
                success = false;
                break;
            }
            if (runner_.hasError() || j == maxAttemptsProg_) {
                // Error
                emit onProgress(current, total, true, false);
                return false;
            }
            // Increment Address
            runner_.addrInc();
            current += increment;
        }
        if (success) break;
    }
    return true;
}
