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

#include "sram.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(deviceParSram, "device.parallel.sram")

#define DEBUG qCDebug(deviceParSram)
#define INFO qCInfo(deviceParSram)
#define WARNING qCWarning(deviceParSram)
#define CRITICAL qCCritical(deviceParSram)
#define FATAL qCFatal(deviceParSram)

// ---------------------------------------------------------------------------

SRAM::SRAM(QObject *parent) : ParDevice(parent) {
    info_.name = "SRAM";
    info_.capability.hasProgram = true;
    info_.capability.hasVDD = true;
    twp_ = 1;
    twc_ = 1;
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
    INFO << "Testing device...";
    canceling_ = false;
    uint32_t current = 0, total = size_;
    if (!initDevice(kDeviceOpRead)) {
        WARNING << "Error testing device";
        return false;
    }
    bool error = false;
    if (!doPatternTest_() || !doRandomTest_()) error = true;
    if (!error && runner_.hasError()) {
        return finalizeDevice(total, total, true, false);
    }
    if (!error) emit onProgress(total, total, true);
    finalizeDevice();
    if (error) {
        WARNING << "Error testing device";
    } else {
        INFO << "Testing device OK";
    }
    return !error;
}

bool SRAM::doPatternTest_() {
    QByteArray buffer = generatePatternData_();
    runner_.addrClr();
    if (!programDevice(buffer)) return false;
    runner_.addrClr();
    if (!verifyDevice(buffer)) return false;
    return !runner_.hasError();
}

bool SRAM::doRandomTest_() {
    QByteArray buffer = generateRandomData_();
    runner_.addrClr();
    if (!programDevice(buffer)) return false;
    runner_.addrClr();
    if (!verifyDevice(buffer)) return false;
    return !runner_.hasError();
}
