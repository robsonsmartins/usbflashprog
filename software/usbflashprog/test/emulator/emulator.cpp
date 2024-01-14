// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/emulator/emulator.cpp
 * @brief Implementation of Emulator Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QRandomGenerator>
#include <chrono>
#include <thread>

#include "emulator.hpp"

// ---------------------------------------------------------------------------

static BaseParChip* globalEmuParChip_ = nullptr;

// ---------------------------------------------------------------------------

Emulator::Emulator(QObject* parent)
    : vdd_(5.0f),
      vpp_(12.0f),
      timeout_(200),
      running_(false),
      error_(false),
      address_(0),
      twp_(1),
      twc_(1) {
    // clang-format off
    flags_.skipFF      = false;
    flags_.progWithVpp = false;
    flags_.vppOePin    = false;
    flags_.pgmCePin    = false;
    flags_.pgmPositive = false;
    // clang-format on
}

Emulator::~Emulator() {
    close();
}

TSerialPortList Emulator::list() const {
    return QSerialPortInfo::availablePorts();
}

bool Emulator::open(const QString& path) {
    if (running_) close();
    if (path.isNull() || path.isEmpty()) return false;
    path_ = path;
    running_ = true;
    error_ = false;
    return true;
}

void Emulator::close() {
    running_ = false;
    error_ = false;
    path_.clear();
}

bool Emulator::isOpen() const {
    return running_;
}

bool Emulator::hasError() const {
    return error_;
}

QString Emulator::getPath() const {
    return path_;
}

uint32_t Emulator::getTimeOut() const {
    return timeout_;
}

void Emulator::setTimeOut(uint32_t value) {
    if (timeout_ == value) return;
    timeout_ = value;
}

bool Emulator::nop() {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vddCtrl(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setVDD(on);
    return true;
}

bool Emulator::vddSet(float value) {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    vdd_ = value;
    return true;
}

float Emulator::vddGet() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    return vdd_;
}

float Emulator::vddGetDuty() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return 50.0f;
}

bool Emulator::vddInitCal() {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vddSaveCal(float value) {
    (void)value;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

float Emulator::vddGetCal() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return vdd_;
}

bool Emulator::vppCtrl(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setVPP(on);
    return true;
}

bool Emulator::vppSet(float value) {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    vpp_ = value;
    return true;
}

float Emulator::vppGet() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    return vpp_;
}

float Emulator::vppGetDuty() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return 50.0f;
}

bool Emulator::vppInitCal() {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppSaveCal(float value) {
    (void)value;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

float Emulator::vppGetCal() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return vpp_;
}

bool Emulator::vddOnVpp(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnA9(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnA18(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnCE(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnOE(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnWE(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::setCE(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setCE(on);
    return true;
}

bool Emulator::setOE(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setOE(on);
    return true;
}

bool Emulator::setWE(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setWE(on);
    return true;
}

bool Emulator::addrClr() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = 0;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrInc() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_++;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrSet(uint32_t value) {
    if (!value) return addrClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = value;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrSetB(uint8_t value) {
    if (!value) return addrClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = value;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrSetW(uint16_t value) {
    if (!value) return addrClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = value;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

uint32_t Emulator::addrGet() const {
    return address_;
}

bool Emulator::dataClr() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setDataBus(0);
    return true;
}

bool Emulator::dataSet(uint8_t value) {
    if (!value) return dataClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setDataBus(value);
    return true;
}

bool Emulator::dataSetW(uint16_t value) {
    if (!value) return dataClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setDataBus(value);
    return true;
}

uint8_t Emulator::dataGet() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return 0xff;
    }
    uint16_t data = globalEmuParChip_->getDataBus();
    return static_cast<uint8_t>(data & 0xff);
}

uint16_t Emulator::dataGetW() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return 0xffff;
    }
    return globalEmuParChip_->getDataBus();
}

bool Emulator::deviceSetTwp(uint32_t value) {
    if (value == twp_) return true;
    twp_ = value;
    return true;
}

bool Emulator::deviceSetTwc(uint32_t value) {
    if (value == twc_) return true;
    twc_ = value;
    return true;
}

bool Emulator::deviceSetFlags(const TDeviceSettings& value) {
    // clang-format off
    flags_.skipFF      = value.skipFF     ;
    flags_.progWithVpp = value.progWithVpp;
    flags_.vppOePin    = value.vppOePin   ;
    flags_.pgmCePin    = value.pgmCePin   ;
    flags_.pgmPositive = value.pgmPositive;
    // clang-format on
    return true;
}

uint8_t Emulator::deviceRead() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return 0xff;
    }
    uint8_t data = deviceRead_(false);
    // inc address
    addrInc();
    if (error_) return 0xff;
    return data;
}

uint16_t Emulator::deviceReadW() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return 0xffff;
    }
    uint16_t data = deviceRead_(true);
    // inc address
    addrInc();
    if (error_) return 0xffff;
    return data;
}

bool Emulator::deviceWrite(uint8_t value) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    if (!deviceWrite_(value, false)) return false;
    // PGM/~CE is LO
    if (flags_.pgmCePin) setWE(true);
    // read
    uint8_t rd = deviceRead_(false);
    // verify
    if (error_ || rd != value) return false;
    // inc address
    addrInc();
    if (error_) return false;
    return true;
}

bool Emulator::deviceWriteW(uint16_t value) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    if (!deviceWrite_(value, true)) return false;
    // PGM/~CE is LO
    if (flags_.pgmCePin) setWE(true);
    // read
    uint16_t rd = deviceRead_(true);
    // verify
    if (error_ || rd != value) return false;
    // inc address
    addrInc();
    if (error_) return false;
    return true;
}

bool Emulator::deviceVerify(uint8_t value) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    // read
    uint8_t rd = deviceRead_(false);
    // verify
    if (error_ || rd != value) return false;
    // inc address
    addrInc();
    return true;
}

bool Emulator::deviceVerifyW(uint16_t value) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    // read
    uint16_t rd = deviceRead_(true);
    // verify
    if (error_ || rd != value) return false;
    // inc address
    addrInc();
    return true;
}

void Emulator::usDelay(uint64_t value) {
    (void)value;
    return;
}

void Emulator::msDelay(uint32_t value) {
    (void)value;
    return;
}

void Emulator::processEvents() {
    return;
}

void Emulator::setChip(BaseParChip* chip) {
    if (globalEmuParChip_ == chip) return;
    globalEmuParChip_ = chip;
}

void Emulator::randomizeBuffer(QByteArray& buffer, uint32_t size) {
    if (!size) return;
    buffer.resize(size);
    for (int i = 0; i < buffer.size(); ++i) {
        buffer[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
}

uint16_t Emulator::deviceRead_(bool is16bit) {
    uint16_t data;
    // ~OE/VPP is LO
    if (flags_.vppOePin) vddOnVpp(false);
    // ~OE is LO
    setOE(true);
    // get data
    if (is16bit) {
        data = dataGetW();
    } else {
        data = dataGet();
    }
    // ~OE is HI
    setOE(false);
    // ~OE/VPP is VDD
    if (flags_.vppOePin) vddOnVpp(true);
    return data;
}

bool Emulator::deviceWrite_(uint16_t value, bool is16bit) {
    bool emptyData = is16bit ? (value == 0xFFFF) : (value == 0xFF);
    // write
    if (!flags_.skipFF || !emptyData) {
        if (flags_.progWithVpp) {
            // VPP on
            vddOnVpp(false);
            vppCtrl(true);
        }
        // Set DataBus
        if (is16bit) {
            dataSetW(value);
        } else {
            dataSet(value);
        }
        if (flags_.pgmPositive) {
            // PGM is HI (start prog pulse)
            setWE(false);
            usDelay(twp_);  // tWP uS
            // PGM is LO (end prog pulse)
            setWE(true);
        } else {
            // ~PGM is LO (start prog pulse)
            setWE(true);
            usDelay(twp_);  // tWP uS
            // ~PGM is HI (end prog pulse)
            setWE(false);
        }
        usDelay(twc_);  // tWC uS
        if (flags_.progWithVpp) {
            // VPP off
            vppCtrl(false);
            vddOnVpp(true);
        }
        if (error_) return false;
    }
    return true;
}
