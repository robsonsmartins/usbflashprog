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
 * @file backend/devices/parallel/dummy.cpp
 * @brief Implementation of the Parallel Dummy Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QRandomGenerator>
#include "backend/devices/parallel/dummy.hpp"

// ---------------------------------------------------------------------------

Dummy::Dummy(QObject *parent) : Device(parent), protected_(true) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "Dummy";
    info_.capability.hasProgram = true;
    info_.capability.hasVerify = true;
    info_.capability.hasErase = true;
    info_.capability.hasGetId = true;
    info_.capability.hasRead = true;
    info_.capability.hasBlankCheck = true;
    info_.capability.hasUnprotect = true;
    info_.capability.hasSectorSize = true;
    info_.capability.hasFastProg = true;
    info_.capability.hasSkipFF = true;
    info_.capability.hasVDD = true;
    info_.capability.hasVPP = true;
    twp_ = 1000;
    twc_ = 2000;
    vdd_ = 5.0f;
    vpp_ = 12.0f;
    setSize(2048);
}

Dummy::~Dummy() {}

void Dummy::setSize(uint32_t value) {
    Device::setSize(value);
    buffer_.resize(value);
    for (int i = 0; i < value; ++i) {
        buffer_[i] = (QRandomGenerator::global()->bounded(256) & 0xFF);
    }
}

bool Dummy::getId(TDeviceID &result) {
    canceling_ = false;
    result.manufacturer = 0x01;
    result.device = 0x01;
    emit onProgress(100, 100, true);
    return true;
}

bool Dummy::read(QByteArray &buffer) {
    canceling_ = false;
    int end = buffer_.size();
    buffer.clear();
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            return false;
        }
        buffer.append(buffer_[i]);
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    emit onProgress(end, end, true);
    return true;
}

bool Dummy::program(const QByteArray &buffer, bool verify) {
    canceling_ = false;
    int end = qMin(buffer.size(), buffer_.size());
    if (protected_) {
        emit onProgress(0, end, true, false);
        return false;
    }
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            return false;
        }
        if (skipFF_ && buffer[i] == (char)0xFF) continue;
        if (fastProg_ && buffer_[i] == buffer[i]) continue;
        buffer_[i] = buffer[i];
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    if (verify) {
        return this->verify(buffer);
    } else {
        emit onProgress(end, end, true);
    }
    return true;
}

bool Dummy::verify(const QByteArray &buffer) {
    canceling_ = false;
    int end = qMin(buffer.size(), buffer_.size());
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            return false;
        }
        if (buffer[i] != buffer_[i]) {
            emit onProgress(i, end, true, false);
            return false;
        }
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    emit onProgress(end, end, true);
    return true;
}

bool Dummy::erase(bool check) {
    canceling_ = false;
    int end = buffer_.size();
    if (protected_) {
        emit onProgress(0, end, true, false);
        return false;
    }
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            return false;
        }
        if (fastProg_ && buffer_[i] == (char)0xFF) continue;
        buffer_[i] = 0xFF;
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    if (check) {
        return blankCheck();
    } else {
        emit onProgress(end, end, true);
    }
    return true;
}

bool Dummy::blankCheck() {
    canceling_ = false;
    int end = buffer_.size();
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            return false;
        }
        if (buffer_[i] != (char)0xFF) {
            emit onProgress(i, end, true, false);
            return false;
        }
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    emit onProgress(end, end, true);
    return true;
}

bool Dummy::unprotect() {
    canceling_ = false;
    if (!protected_) {
        emit onProgress(0, 100, true, false);
        return false;
    }
    protected_ = false;
    Runner::usDelay(twc_);
    emit onProgress(100, 100, true);
    return true;
}
