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

#include <QLoggingCategory>
#include <QRandomGenerator>

#include "backend/devices/parallel/dummy.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(deviceParDummy, "device.parallel.dummy")

#define DEBUG qCDebug(deviceParDummy)
#define INFO qCInfo(deviceParDummy)
#define WARNING qCWarning(deviceParDummy)
#define CRITICAL qCCritical(deviceParDummy)
#define FATAL qCFatal(deviceParDummy)

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
    vddRd_ = 5.0f;
    vddWr_ = 5.0f;
    vpp_ = 12.0f;
    vee_ = 12.0f;
    DEBUG << info_.toString();
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
    INFO << "Getting ID from device...";
    canceling_ = false;
    result.manufacturer = 0x01;
    result.device = 0x01;
    emit onProgress(100, 100, true);
    INFO << "Getting ID from device OK";
    return true;
}

bool Dummy::read(QByteArray &buffer) {
    INFO << "Reading device...";
    canceling_ = false;
    int end = buffer_.size();
    buffer.clear();
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            INFO << QString("Read canceled at 0x%1 of 0x%2")
                        .arg(i, 6, 16, QChar('0'))
                        .arg(end, 6, 16, QChar('0'));
            return false;
        }
        buffer.append(buffer_[i]);
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    emit onProgress(end, end, true);
    INFO << "Reading device OK";
    return true;
}

bool Dummy::program(const QByteArray &buffer, bool verify) {
    INFO << "Programming device...";
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
            INFO << QString("Program canceled at 0x%1 of 0x%2")
                        .arg(i, 6, 16, QChar('0'))
                        .arg(end, 6, 16, QChar('0'));
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
    INFO << "Programming device OK";
    return true;
}

bool Dummy::verify(const QByteArray &buffer) {
    INFO << "Verifying device...";
    canceling_ = false;
    int end = qMin(buffer.size(), buffer_.size());
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            INFO << QString("Verify canceled at 0x%1 of 0x%2")
                        .arg(i, 6, 16, QChar('0'))
                        .arg(end, 6, 16, QChar('0'));
            return false;
        }
        if (buffer[i] != buffer_[i]) {
            emit onProgress(i, end, true, false);
            WARNING << QString("Verify error at 0x%1 of 0x%2")
                           .arg(i, 6, 16, QChar('0'))
                           .arg(end, 6, 16, QChar('0'));
            WARNING << QString("Data to write 0x%1. Data read 0x%2")
                           .arg(buffer[i], 2, 16, QChar('0'))
                           .arg(buffer_[i], 2, 16, QChar('0'));
            return false;
        }
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    emit onProgress(end, end, true);
    INFO << "Verifying device OK";
    return true;
}

bool Dummy::erase(bool check) {
    INFO << "Erasing device...";
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
            INFO << QString("Erase canceled at 0x%1 of 0x%2")
                        .arg(i, 6, 16, QChar('0'))
                        .arg(end, 6, 16, QChar('0'));
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
    INFO << "Erasing device OK";
    return true;
}

bool Dummy::blankCheck() {
    INFO << "Checking device...";
    canceling_ = false;
    int end = buffer_.size();
    for (int i = 0; i < end; ++i) {
        emit onProgress(i, end);
        if (canceling_) {
            emit onProgress(end, end, true, false, true);
            INFO << QString("Blank Check canceled at 0x%1 of 0x%2")
                        .arg(i, 6, 16, QChar('0'))
                        .arg(end, 6, 16, QChar('0'));
            return false;
        }
        if (buffer_[i] != (char)0xFF) {
            emit onProgress(i, end, true, false);
            WARNING << QString("Blank Check error at 0x%1 of 0x%2")
                           .arg(i, 6, 16, QChar('0'))
                           .arg(end, 6, 16, QChar('0'));
            WARNING << QString("Data to write 0x%1. Data read 0x%2")
                           .arg(0xFF, 2, 16, QChar('0'))
                           .arg(buffer_[i], 2, 16, QChar('0'));
            return false;
        }
        Runner::usDelay(twp_);
    }
    Runner::usDelay(twc_);
    emit onProgress(end, end, true);
    INFO << "Checking device OK";
    return true;
}

bool Dummy::unprotect() {
    INFO << "Unprotecting device...";
    canceling_ = false;
    if (!protected_) {
        emit onProgress(0, 100, true, false);
        WARNING << "Error on Unprotect: Already unprotected";
        return false;
    }
    protected_ = false;
    Runner::usDelay(twc_);
    emit onProgress(100, 100, true);
    INFO << "Unprotecting device OK";
    return true;
}

// ---------------------------------------------------------------------------

Dummy16Bit::Dummy16Bit(QObject *parent) : Dummy(parent) {
    info_.name = "Dummy (16 bits)";
    DEBUG << info_.toString();
}

Dummy16Bit::~Dummy16Bit() {}
