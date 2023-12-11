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

Device::Device(QObject *parent)
    : QObject(parent),
      size_(0),
      twp_(1),
      twc_(1),
      skipFF_(false),
      fastProg_(false),
      sectorSize_(0),
      runner_(this) {
    info_.deviceType = kParallelMemory;
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
    info_.voltage.vddProgram = 0.0f;
    info_.voltage.vppProgram = 0.0f;
    info_.voltage.vddRead = 0.0f;
    info_.voltage.vppRead = 0.0f;
    info_.voltage.vddErase = 0.0f;
    info_.voltage.vppErase = 0.0f;
    info_.voltage.vddGetId = 0.0f;
    info_.voltage.vppGetId = 0.0f;
    info_.voltage.vddUnprotect = 0.0f;
    info_.voltage.vppUnprotect = 0.0f;
}

Device::~Device() {}

void Device::setPort(const QString &path) {
    if (port_ == path) {
        return;
    }
    port_ = path;
}

QString Device::getPort() const { return port_; }

void Device::setSize(uint32_t value) {
    if (size_ == value) {
        return;
    }
    if (value) {
        size_ = value;
    }
}

uint32_t Device::getSize() const { return size_; }

void Device::setTwp(uint32_t us) {
    if (twp_ == us) {
        return;
    }
    if (us) {
        twp_ = us;
    }
}

uint32_t Device::getTwp() const { return twp_; }

void Device::setTwc(uint32_t us) {
    if (twc_ == us) {
        return;
    }
    if (us) {
        twc_ = us;
    }
}

uint32_t Device::getTwc() const { return twc_; }

void Device::setSkipFF(bool value) {
    if (skipFF_ == value) {
        return;
    }
    skipFF_ = value;
}

bool Device::getSkipFF() const { return skipFF_; }

void Device::setFastProg(bool value) {
    if (fastProg_ == value) {
        return;
    }
    fastProg_ = value;
}

bool Device::getFastProg() const { return fastProg_; }

void Device::setSectorSize(uint16_t value) {
    if (sectorSize_ == value) {
        return;
    }
    if (sectorSize_) {
        sectorSize_ = value;
    }
}

uint16_t Device::getSectorSize() const { return sectorSize_; }

TDeviceInformation Device::getInfo() const { return info_; }

bool Device::getId(TDeviceID &result) {
    (void)result;
    return false;
}

bool Device::read(QByteArray &buffer) {
    buffer.clear();
    return false;
}

bool Device::program(const QByteArray &buffer, bool verify) {
    if (verify) {
        return this->verify(buffer);
    }
    return false;
}

bool Device::verify(const QByteArray &buffer) {
    QByteArray read;
    if (!this->read(read)) {
        return false;
    }
    return (!buffer.compare(read));
}

bool Device::erase(bool check) {
    if (check) {
        return blankCheck();
    }
    return false;
}

bool Device::blankCheck() { return false; }

bool Device::unprotect() { return false; }
