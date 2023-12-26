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
 * @file backend/devices/device.cpp
 * @brief Implementation of the Base class of a Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "backend/devices/device.hpp"

// ---------------------------------------------------------------------------

TDeviceID::TDeviceID() : manufacturer(0), device(0) {}

QString TDeviceID::getManufacturerName(void) const {
    switch (manufacturer) {
        case 0x01:
            return "AMD";
        case 0x02:  // to validate
            return "Macronix";
        case 0x04:
            return "Fujitsu";
        case 0x0C:  // to validate
        case 0x29:
            return "Microchip Technology";
        case 0x1C:  // to validate
            return "EON";
        case 0x1E:
        case 0x1F:
            return "Atmel";
        case 0x20:
            return "ST";
        case 0x31:
            return "Catalyst";
        case 0x37:  // to validate
            return "AMIC Technology";
        case 0x38:  // to validate
            return "Winbond";
        case 0x40:
        case 0x42:
            return "SyncMOS";
        case 0x49:  // to validate
            return "Toshiba";
        case 0x4A:  // to validate
            return "Macronix";
        case 0x50:  // to validate
            return "Spansion";
        case 0x7F:  // to validate
            return "Adesto Technologies";
        case 0x89:
            return "Intel";
        case 0x8F:
            return "National Semiconductor";
        case 0x9D:  // to validate
            return "Xicor";
        case 0xAD:
            return "Hyudai";
        case 0xB0:
            return "Sharp";
        case 0xBF:
            return "SST";
        case 0xC2:
            return "MXIC";
        case 0xC8:  // to validate
            return "GigaDevice";
        case 0xDA:
            return "ASD/WinBond";
        case 0xEF:  // to validate
            return "ISSI";
        default:
            return Device::tr("Unknown");
    }
}

// ---------------------------------------------------------------------------

TDeviceCapabilities::TDeviceCapabilities()
    : hasProgram(false),
      hasVerify(false),
      hasErase(false),
      hasGetId(false),
      hasRead(false),
      hasBlankCheck(false),
      hasUnprotect(false),
      hasSectorSize(false),
      hasFastProg(false),
      hasSkipFF(false) {}

// ---------------------------------------------------------------------------

TDeviceInformation::TDeviceInformation()
    : deviceType(kDeviceParallelMemory), name("") {}

// ---------------------------------------------------------------------------

Device::Device(QObject *parent)
    : QObject(parent),
      canceling_(false),
      size_(0),
      twp_(1),
      twc_(1),
      vddRd_(5.0f),
      vddWr_(5.0f),
      vpp_(12.0f),
      skipFF_(false),
      fastProg_(false),
      sectorSize_(0),
      runner_(this) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "Device";
    info_.capability.hasProgram = false;
    info_.capability.hasVerify = false;
    info_.capability.hasErase = false;
    info_.capability.hasGetId = false;
    info_.capability.hasRead = false;
    info_.capability.hasBlankCheck = false;
    info_.capability.hasUnprotect = false;
    info_.capability.hasSectorSize = false;
    info_.capability.hasFastProg = false;
    info_.capability.hasSkipFF = false;
    info_.capability.hasVDD = true;
    info_.capability.hasVPP = false;
}

Device::~Device() {}

void Device::setPort(const QString &path) {
    if (port_ == path) return;
    port_ = path;
}

QString Device::getPort() const {
    return port_;
}

void Device::setSize(uint32_t value) {
    if (size_ == value) return;
    if (value) size_ = value;
}

uint32_t Device::getSize() const {
    return size_;
}

void Device::setTwp(uint32_t us) {
    if (twp_ == us) return;
    if (us) twp_ = us;
}

uint32_t Device::getTwp() const {
    return twp_;
}

void Device::setTwc(uint32_t us) {
    if (twc_ == us) return;
    if (us) twc_ = us;
}

uint32_t Device::getTwc() const {
    return twc_;
}

void Device::setVddRd(float value) {
    if (vddRd_ == value) return;
    if (value >= 0.0f) vddRd_ = value;
}

float Device::getVddRd() const {
    return vddRd_;
}

void Device::setVddWr(float value) {
    if (vddWr_ == value) return;
    if (value >= 0.0f) vddWr_ = value;
}

float Device::getVddWr() const {
    return vddWr_;
}

void Device::setVpp(float value) {
    if (vpp_ == value) return;
    if (value >= 0.0f) vpp_ = value;
}

float Device::getVpp() const {
    return vpp_;
}

void Device::setSkipFF(bool value) {
    if (skipFF_ == value) return;
    skipFF_ = value;
}

bool Device::getSkipFF() const {
    return skipFF_;
}

void Device::setFastProg(bool value) {
    if (fastProg_ == value) return;
    fastProg_ = value;
}

bool Device::getFastProg() const {
    return fastProg_;
}

void Device::setSectorSize(uint16_t value) {
    if (sectorSize_ == value) return;
    if (value) sectorSize_ = value;
}

uint16_t Device::getSectorSize() const {
    return sectorSize_;
}

TDeviceInformation Device::getInfo() const {
    return info_;
}

void Device::cancel() {
    if (canceling_) return;
    canceling_ = true;
}

bool Device::getId(TDeviceID &result) {
    (void)result;
    return false;
}

bool Device::read(QByteArray &buffer) {
    buffer.clear();
    return false;
}

bool Device::program(const QByteArray &buffer, bool verify) {
    if (verify) return this->verify(buffer);
    return false;
}

bool Device::verify(const QByteArray &buffer) {
    QByteArray read;
    if (!this->read(read)) return false;
    return (!buffer.compare(read));
}

bool Device::erase(bool check) {
    if (check) return blankCheck();
    return false;
}

bool Device::blankCheck() {
    return false;
}

bool Device::unprotect() {
    return false;
}
