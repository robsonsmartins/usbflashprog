// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
 * @file modules/vgenerator.cpp
 * @brief Implementation of the Votage Generator Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "modules/vgenerator.hpp"
#include "config.hpp"

// ---------------------------------------------------------------------------

VGenConfig::VGenConfig(): Dc2DcConfig(), ctrlPin(0xFF) {}

VGenConfig::VGenConfig(uint pwmPin, uint adcChannel, uint ctrlPin,
                       float divider, uint32_t pwmFreq,
                       float adcVref, float pwmMinDuty, float pwmMaxDuty,
                       float pwmSlowStepDuty, float pwmFastStepDuty,
                       float pwmToleranceToFast, float vTolerance):
        Dc2DcConfig(pwmPin, adcChannel, divider, pwmFreq, adcVref,
                    pwmMinDuty, pwmMaxDuty, pwmSlowStepDuty,
                    pwmFastStepDuty, pwmToleranceToFast, vTolerance),
        ctrlPin(ctrlPin) {}

VGenConfig& VGenConfig::operator=(const VGenConfig& src) {
    Dc2DcConfig::operator=(src);
    this->ctrlPin = src.ctrlPin;
    return *this;
}

bool operator==(const VGenConfig& a, const VGenConfig& b) {
    return (operator==((Dc2DcConfig&)a, (Dc2DcConfig&)b) &&
            a.ctrlPin == b.ctrlPin);
}

// ---------------------------------------------------------------------------

VddConfig::VddConfig(): VGenConfig(), onVppPin(0xFF) {}

VddConfig::VddConfig(uint pwmPin, uint adcChannel, uint ctrlPin, uint onVppPin,
                     float divider, uint32_t pwmFreq, float adcVref,
                     float pwmMinDuty, float pwmMaxDuty, float pwmSlowStepDuty,
                     float pwmFastStepDuty, float pwmToleranceToFast,
                     float vTolerance):
        VGenConfig(pwmPin, adcChannel, ctrlPin, divider, pwmFreq, adcVref,
                   pwmMinDuty, pwmMaxDuty, pwmSlowStepDuty, pwmFastStepDuty,
                   pwmToleranceToFast, vTolerance),
        onVppPin(onVppPin) {}

VddConfig& VddConfig::operator=(const VddConfig& src) {
    VGenConfig::operator=(src);
    this->onVppPin = src.onVppPin;
    return *this;
}

bool operator==(const VddConfig& a, const VddConfig& b) {
    return (operator==((VGenConfig&)a, (VGenConfig&)b) &&
            a.onVppPin == b.onVppPin);
}

bool operator!=(const VddConfig& a, const VddConfig& b) {
    return !operator==(a, b);
}

// ---------------------------------------------------------------------------

VppConfig::VppConfig(): VGenConfig(),
        vcSinPin(0xFF),
        vcClkPin(0xFF),
        vcClrPin(0xFF),
        vcRckPin(0xFF) {}

VppConfig::VppConfig(uint pwmPin, uint adcChannel, uint ctrlPin, uint vcSinPin,
                     uint vcClkPin, uint vcClrPin, uint vcRckPin, float divider,
                     uint32_t pwmFreq, float adcVref, float pwmMinDuty,
                     float pwmMaxDuty, float pwmSlowStepDuty,
                     float pwmFastStepDuty, float pwmToleranceToFast,
                     float vTolerance):
        VGenConfig(pwmPin, adcChannel, ctrlPin, divider, pwmFreq, adcVref,
                   pwmMinDuty, pwmMaxDuty, pwmSlowStepDuty, pwmFastStepDuty,
                   pwmToleranceToFast, vTolerance),
        vcSinPin(vcSinPin),
        vcClkPin(vcClkPin),
        vcClrPin(vcClrPin),
        vcRckPin(vcRckPin) {}

VppConfig& VppConfig::operator=(const VppConfig& src) {
    VGenConfig::operator=(src);
    this->vcSinPin = src.vcSinPin;
    this->vcClkPin = src.vcClkPin;
    this->vcClrPin = src.vcClrPin;
    this->vcRckPin = src.vcRckPin;
    return *this;
}

bool operator==(const VppConfig& a, const VppConfig& b) {
    return (operator==((VGenConfig&)a, (VGenConfig&)b) &&
            a.vcSinPin == b.vcSinPin &&
            a.vcClkPin == b.vcClkPin &&
            a.vcClrPin == b.vcClrPin &&
            a.vcRckPin == b.vcRckPin);
}

bool operator!=(const VppConfig& a, const VppConfig& b) {
    return !operator==(a, b);
}

// ---------------------------------------------------------------------------

VGenerator::VGenerator(): on_(false) {}

VGenerator::~VGenerator() {
    off();
    stop();
}

bool VGenerator::start() {
    if (!isValidConfig_()) { return false; }
    return dc2dc_.start();
}

void VGenerator::stop() {
    if (!isValidConfig_()) { return; }
    dc2dc_.stop();
}

bool VGenerator::isRunning() const {
    return dc2dc_.isRunning();
}

bool VGenerator::on() {
    if (!isValidConfig_()) { return false; }
    gpio_.setPin(config_.ctrlPin);
    on_ = true;
    return true;
}

void VGenerator::off() {
    if (!isValidConfig_()) { return; }
    gpio_.resetPin(config_.ctrlPin);
    on_ = false;
}

bool VGenerator::isOn() const {
    return on_;
}

void VGenerator::adjust() {
    dc2dc_.adjust();
}

void VGenerator::setV(float v) {
    dc2dc_.setV(v);
}

float VGenerator::getV() const {
    return dc2dc_.getV();
}

float VGenerator::getVTarget() const {
    return dc2dc_.getVTarget();
}

float VGenerator::getDuty() const {
    return dc2dc_.getDuty();
}

bool VGenerator::isValidConfig_() const {
    return (config_.pwmPin     != 0xFF &&
            config_.pwmFreq    != 0.0f &&
            config_.adcChannel != 0xFF &&
            config_.ctrlPin    != 0xFF);
}

// ---------------------------------------------------------------------------

VddGenerator::VddGenerator(): VGenerator(), onVpp_(false) {}

VddGenerator::VddGenerator(const VddConfig& config): VddGenerator() {
    configure(config);
}

VddGenerator::~VddGenerator() {
    onVpp(false);
}

void VddGenerator::configure(const VddConfig& config) {
    off();
    dc2dc_.configure(config);
    config_ = config;
    VGenerator::config_ = config;
}

VddConfig VddGenerator::getConfig() const {
    return config_;
}

bool VddGenerator::onVpp(bool status) {
    if (!isValidConfig_()) { return false; }
    gpio_.setPin(config_.onVppPin, status);
    onVpp_ = status;
    return true;
}

bool VddGenerator::isOnVpp() const {
    return onVpp_;
}

bool VddGenerator::isValidConfig_() const {
    return (VGenerator::isValidConfig_() &&
            config_.onVppPin != 0xFF);
}

// ---------------------------------------------------------------------------

VppGenerator::VppGenerator(): VGenerator() {}

VppGenerator::VppGenerator(const VppConfig& config): VppGenerator() {
    configure(config);
}

VppGenerator::~VppGenerator() {
    if (isValidConfig_()) {
        vcRegister_.clear();
    }
}

void VppGenerator::configure(const VppConfig& config) {
    off();
    dc2dc_.configure(config);
    config_ = config;
    VGenerator::config_ = config;
    if (isValidConfig_()) {
        vcRegister_.clear();
    }
}

VppConfig VppGenerator::getConfig() const {
    return config_;
}

bool VppGenerator::onA9(bool status) {
    if (!isValidConfig_()) { return false; }
    vcRegister_.setBit(VPP_ON_A9_VC_REGISTER_BIT, status);
    return true;
}

bool VppGenerator::onA18(bool status) {
    if (!isValidConfig_()) { return false; }
    vcRegister_.setBit(VPP_ON_A18_VC_REGISTER_BIT, status);
    return true;
}

bool VppGenerator::onCE(bool status) {
    if (!isValidConfig_()) { return false; }
    vcRegister_.setBit(VPP_ON_CE_VC_REGISTER_BIT, status);
    return true;
}

bool VppGenerator::onOE(bool status) {
    if (!isValidConfig_()) { return false; }
    vcRegister_.setBit(VPP_ON_OE_VC_REGISTER_BIT, status);
    return true;
}

bool VppGenerator::onWE(bool status) {
    if (!isValidConfig_()) { return false; }
    vcRegister_.setBit(VPP_ON_WE_VC_REGISTER_BIT, status);
    return true;
}

bool VppGenerator::isOnA9() const {
    if (!isValidConfig_()) { return false; }
    return vcRegister_.getBit(VPP_ON_A9_VC_REGISTER_BIT);
}

bool VppGenerator::isOnA18() const {
    if (!isValidConfig_()) { return false; }
    return vcRegister_.getBit(VPP_ON_A18_VC_REGISTER_BIT);
}

bool VppGenerator::isOnCE() const {
    if (!isValidConfig_()) { return false; }
    return vcRegister_.getBit(VPP_ON_CE_VC_REGISTER_BIT);
}

bool VppGenerator::isOnOE() const {
    if (!isValidConfig_()) { return false; }
    return vcRegister_.getBit(VPP_ON_OE_VC_REGISTER_BIT);
}

bool VppGenerator::isOnWE() const {
    if (!isValidConfig_()) { return false; }
    return vcRegister_.getBit(VPP_ON_WE_VC_REGISTER_BIT);
}

bool VppGenerator::isValidConfig_() const {
    return (VGenerator::isValidConfig_() &&
            config_.vcSinPin != 0xFF &&
            config_.vcClkPin != 0xFF &&
            config_.vcClrPin != 0xFF &&
            config_.vcRckPin != 0xFF);
}
