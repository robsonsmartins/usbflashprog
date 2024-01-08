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
 * @brief Implementation of the Voltage Generator Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstring>

#include "modules/vgenerator.hpp"
#include "config.hpp"
#include "hal/flash.hpp"

// ---------------------------------------------------------------------------

void second_core(MultiCore& core); // NOLINT

// ---------------------------------------------------------------------------

VddConfig::VddConfig(): Dc2DcConfig(), ctrlPin(0xFF), onVppPin(0xFF) {}

VddConfig& VddConfig::operator=(const VddConfig& src) {
    Dc2DcConfig::operator=(src);
    this->ctrlPin  =  src.ctrlPin;
    this->onVppPin = src.onVppPin;
    return *this;
}

bool operator==(const VddConfig& a, const VddConfig& b) {
    return (operator==((Dc2DcConfig&)a, (Dc2DcConfig&)b) &&
            a.ctrlPin  == b.ctrlPin &&
            a.onVppPin == b.onVppPin);
}

// ---------------------------------------------------------------------------

VppConfig::VppConfig(): Dc2DcConfig(),
        ctrlPin(0xFF), vcSinPin(0xFF), vcClkPin(0xFF),
        vcClrPin(0xFF), vcRckPin(0xFF) {}

VppConfig& VppConfig::operator=(const VppConfig& src) {
    Dc2DcConfig::operator=(src);
    this->ctrlPin  =  src.ctrlPin;
    this->vcSinPin = src.vcSinPin;
    this->vcClkPin = src.vcClkPin;
    this->vcClrPin = src.vcClrPin;
    this->vcRckPin = src.vcRckPin;
    return *this;
}

bool operator==(const VppConfig& a, const VppConfig& b) {
    return (operator==((Dc2DcConfig&)a, (Dc2DcConfig&)b) &&
            a.ctrlPin  == b.ctrlPin  &&
            a.vcSinPin == b.vcSinPin &&
            a.vcClkPin == b.vcClkPin &&
            a.vcClrPin == b.vcClrPin &&
            a.vcRckPin == b.vcRckPin);
}

// ---------------------------------------------------------------------------

VGenConfig& VGenConfig::operator=(const VGenConfig& src) {
    this->vpp = src.vpp;
    this->vdd = src.vdd;
    return *this;
}

bool operator==(const VGenConfig& a, const VGenConfig& b) {
    return (a.vpp == b.vpp && a.vdd == b.vdd);
}

bool operator!=(const VGenConfig& a, const VGenConfig& b) {
    return (!(a.vpp == b.vpp) || !(a.vdd == b.vdd));
}

// ---------------------------------------------------------------------------

GenericGenerator::GenericGenerator(const VGenerator *owner):
        owner_(const_cast<VGenerator*>(owner)), on_(false) {}

bool GenericGenerator::isRunning() const {
    return dc2dc_.isRunning();
}

void GenericGenerator::setV(float value) {
    dc2dc_.setV(value);
}

float GenericGenerator::getVTarget() const {
    return dc2dc_.getVTarget();
}

float GenericGenerator::getV() const {
    if (!owner_->isRunning() ||
         dc2dc_.getV() < 0.0f) { return 0.0f; }
    return dc2dc_.getV();
}

float GenericGenerator::getCalibration() const {
    return dc2dc_.getCalibration();
}

float GenericGenerator::getDuty() const {
    return dc2dc_.getDuty();
}

void GenericGenerator::initCalibration(float reference) {
    if (!owner_->isRunning()) { return; }
    if (reference <= 0.0f) { return; }
    dc2dc_.setCalibration(0.0f);
    setV(reference);
    on();
}

void GenericGenerator::saveCalibration(float measure) {
    if (!owner_->isRunning()) { return; }
    float v = dc2dc_.getV();
    if (measure <= 0.0f || v <= 0.0f) {
        dc2dc_.setCalibration(0.0f);
    } else {
        dc2dc_.setCalibration(measure - v);
    }
    owner_->stop();
    owner_->writeCalData_();
    setV(0.0f);
    off();
    owner_->start();
}

void GenericGenerator::toggle() {
    if (isOn()) {
        off();
    } else {
        on();
    }
}

bool GenericGenerator::isOn() const {
    return on_;
}

bool GenericGenerator::start_() {
    return dc2dc_.start();
}

void GenericGenerator::stop_() {
    dc2dc_.stop();
}

void GenericGenerator::adjust_() {
    dc2dc_.adjust();
}

// ---------------------------------------------------------------------------

VddGenerator::VddGenerator(const VGenerator *owner): GenericGenerator(owner),
        onVpp_(false) {}

void VddGenerator::on() {
    gpio_.setPin(owner_->config_.vdd.ctrlPin);
    on_ = true;
}

void VddGenerator::off() {
    gpio_.resetPin(owner_->config_.vdd.ctrlPin);
    on_ = false;
}

void VddGenerator::initCalibration(float reference) {
    GenericGenerator::initCalibration(reference);
}

void VddGenerator::onVpp(bool status) {
    gpio_.setPin(owner_->config_.vdd.onVppPin, status);
    onVpp_ = status;
}

bool VddGenerator::isOnVpp() const {
    return onVpp_;
}

// ---------------------------------------------------------------------------

VppGenerator::VppGenerator(const VGenerator *owner):
        GenericGenerator(owner) {}

void VppGenerator::on() {
    gpio_.setPin(owner_->config_.vpp.ctrlPin);
    on_ = true;
}

void VppGenerator::off() {
    gpio_.resetPin(owner_->config_.vpp.ctrlPin);
    on_ = false;
}

void VppGenerator::initCalibration(float reference) {
    GenericGenerator::initCalibration(reference);
}

void VppGenerator::onA9(bool status) {
    vcRegister_.setBit(kVppOnA9VcRegisterBit, status);
}

void VppGenerator::onA18(bool status) {
    vcRegister_.setBit(kVppOnA18VcRegisterBit, status);
}

void VppGenerator::onCE(bool status) {
    vcRegister_.setBit(kVppOnCEVcRegisterBit, status);
}

void VppGenerator::onOE(bool status) {
    vcRegister_.setBit(kVppOnOEVcRegisterBit, status);
}

void VppGenerator::onWE(bool status) {
    vcRegister_.setBit(kVppOnWEVcRegisterBit, status);
}

bool VppGenerator::isOnA9() const {
    return vcRegister_.getBit(kVppOnA9VcRegisterBit);
}

bool VppGenerator::isOnA18() const {
    return vcRegister_.getBit(kVppOnA18VcRegisterBit);
}

bool VppGenerator::isOnCE() const {
    return vcRegister_.getBit(kVppOnCEVcRegisterBit);
}

bool VppGenerator::isOnOE() const {
    return vcRegister_.getBit(kVppOnOEVcRegisterBit);
}

bool VppGenerator::isOnWE() const {
    return vcRegister_.getBit(kVppOnWEVcRegisterBit);
}

bool VppGenerator::start_() {
    vcRegister_.configure(
        owner_->config_.vpp.vcSinPin,
        owner_->config_.vpp.vcClkPin,
        owner_->config_.vpp.vcClrPin,
        owner_->config_.vpp.vcRckPin);
    vcRegister_.clear();
    vcRegister_.outputEnable();
    return GenericGenerator::start_();
}

void VppGenerator::stop_() {
    vcRegister_.clear();
    GenericGenerator::stop_();
}

// ---------------------------------------------------------------------------

VGenerator::VGenerator(): status_(MultiCore::csStopped),
                          vpp(this), vdd(this), multicore_(second_core) {
    readCalData_();
}

VGenerator::VGenerator(const VGenConfig& config): VGenerator() {
    configure(config);
}

VGenerator::~VGenerator() {
    stop();
}

void VGenerator::configure(const VGenConfig& config) {
    if (isRunning()) { stop(); }
    config_ = config;
    vpp.dc2dc_.configure(config_.vpp);
    vdd.dc2dc_.configure(config_.vdd);
}

VGenConfig VGenerator::getConfig() const {
    return config_;
}

bool VGenerator::start() {
    if (status_ != MultiCore::csStopped) { return true; }
    if (!isValidConfig_()) { return false; }
    status_ = MultiCore::csStarting;
    bool vppRes = vpp.start_();
    bool vddRes = vdd.start_();
    multicore_.launch();
    multicore_.putParam(reinterpret_cast<uintptr_t>(&vpp));
    multicore_.putParam(reinterpret_cast<uintptr_t>(&vdd));
    while (status_ == MultiCore::csStarting) { MultiCore::usleep(1); }
    MultiCore::msleep(200);
    return (vppRes && vddRes);
}

void VGenerator::stop() {
    if (status_ != MultiCore::csRunning) { return; }
    multicore_.lock();
    status_ = MultiCore::csStopping;
    multicore_.unlock();
    vpp.off();
    vdd.off();
    vpp.stop_();
    vdd.stop_();
    multicore_.stop();
#ifdef __arm__
    multicore_.lock();
    status_ = MultiCore::csStopped;
    multicore_.unlock();
#endif
}

bool VGenerator::isRunning() const {
    return (status_ != MultiCore::csStopped);
}

bool VGenerator::isValidConfig_() const {
    return (config_.vdd.pwmPin     != 0xFF &&
            config_.vdd.pwmFreq    != 0.0f &&
            config_.vdd.adcChannel != 0xFF &&
            config_.vdd.ctrlPin    != 0xFF &&
            config_.vdd.onVppPin   != 0xFF &&
            config_.vpp.pwmPin     != 0xFF &&
            config_.vpp.pwmFreq    != 0.0f &&
            config_.vpp.adcChannel != 0xFF &&
            config_.vpp.ctrlPin    != 0xFF &&
            config_.vpp.vcSinPin   != 0xFF &&
            config_.vpp.vcClkPin   != 0xFF &&
            config_.vpp.vcClrPin   != 0xFF &&
            config_.vpp.vcRckPin   != 0xFF);
}

void VGenerator::readCalData_() {
    size_t len = sizeof(float) * 2;
    uint8_t *buf = new uint8_t[len + 1]();
    Flash::read(buf, len + 1);
    float vddCal, vppCal;
    if (checksum_(buf, len) == buf[len]) {
        vddCal = *(reinterpret_cast<float*>(buf));
        vppCal = *(reinterpret_cast<float*>(buf + sizeof(float)));
    } else {
        vddCal = 0.0f;
        vppCal = 0.0f;
    }
    vdd.dc2dc_.setCalibration(vddCal);
    vpp.dc2dc_.setCalibration(vppCal);
    delete[] buf;
}

void VGenerator::writeCalData_() {
    size_t len = sizeof(float) * 2;
    uint8_t *buf = new uint8_t[len + 1]();
    float vddCal = vdd.getCalibration();
    float vppCal = vpp.getCalibration();
    std::memcpy(buf, &vddCal, sizeof(float));
    std::memcpy(buf + sizeof(float), &vppCal, sizeof(float));
    buf[len] = checksum_(buf, len);
    Flash::write(buf, len + 1);
    delete[] buf;
}

uint8_t VGenerator::checksum_(const uint8_t *buf, size_t len) {
    uint8_t cs = 0;
    for (size_t i = 0; i < len; i++) { cs += buf[i]; }
    return cs;
}

// ---------------------------------------------------------------------------

void second_core(MultiCore& core) { // NOLINT
    VppGenerator *vpp = reinterpret_cast<VppGenerator*>(core.getParam());
    VddGenerator *vdd = reinterpret_cast<VddGenerator*>(core.getParam());
    core.lock();
    vpp->owner_->status_ = MultiCore::csRunning;
    core.unlock();
    while (!core.isStopRequested()) {
        vpp->adjust_();
        vdd->adjust_();
    }
    core.lock();
    vpp->owner_->status_ = MultiCore::csStopped;
    core.unlock();
}
