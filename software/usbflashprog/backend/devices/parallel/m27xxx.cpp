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
 * @file backend/devices/parallel/m27xxx.cpp
 * @brief Implementation of a Parallel EPROM 27xxx.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "backend/devices/parallel/m27xxx.hpp"

// ---------------------------------------------------------------------------

M27xxx::M27xxx(QObject *parent)
    : SRAM(parent), progWithWE_(true), progIsInverted_(false) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "EPROM 27xxx";
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
    size_ = 2048;
}

M27xxx::~M27xxx() {}

bool M27xxx::read(QByteArray &buffer) {
    return false;
}

bool M27xxx::program(const QByteArray &buffer, bool verify) {
    return false;
}

bool M27xxx::verify(const QByteArray &buffer) {
    return false;
}

bool M27xxx::blankCheck() {
    return false;
}

bool M27xxx::getId(TDeviceID &result) {
    return false;
}
