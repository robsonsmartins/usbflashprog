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
 * @file modules/bus.cpp
 * @brief Implementation of the Bus Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "modules/bus.hpp"
#include "config.hpp"

// ---------------------------------------------------------------------------

CtrlBusConfig::CtrlBusConfig() : cePin(0xFF), oePin(0xFF), wePin(0xFF) {}

CtrlBusConfig& CtrlBusConfig::operator=(const CtrlBusConfig& src) {
    cePin = src.cePin;
    oePin = src.oePin;
    wePin = src.wePin;
    return *this;
}

bool operator==(const CtrlBusConfig& a, const CtrlBusConfig& b) {
    return (a.cePin == b.cePin && a.oePin == b.oePin && a.wePin == b.wePin);
}

// ---------------------------------------------------------------------------

DataBusConfig::DataBusConfig()
    : dSinPin(0xFF),
      dClkPin(0xFF),
      dClrPin(0xFF),
      dRckPin(0xFF),
      dSoutPin(0xFF) {}

DataBusConfig& DataBusConfig::operator=(const DataBusConfig& src) {
    dSinPin = src.dSinPin;
    dClkPin = src.dClkPin;
    dClrPin = src.dClrPin;
    dRckPin = src.dRckPin;
    dSoutPin = src.dSoutPin;
    return *this;
}

bool operator==(const DataBusConfig& a, const DataBusConfig& b) {
    return (a.dSinPin == b.dSinPin && a.dClkPin == b.dClkPin &&
            a.dClrPin == b.dClrPin && a.dRckPin == b.dRckPin &&
            a.dSoutPin == b.dSoutPin);
}

// ---------------------------------------------------------------------------

AddrBusConfig::AddrBusConfig()
    : aSinPin(0xFF), aClkPin(0xFF), aClrPin(0xFF), aRckPin(0xFF) {}

AddrBusConfig& AddrBusConfig::operator=(const AddrBusConfig& src) {
    aSinPin = src.aSinPin;
    aClkPin = src.aClkPin;
    aClrPin = src.aClrPin;
    aRckPin = src.aRckPin;
    return *this;
}

bool operator==(const AddrBusConfig& a, const AddrBusConfig& b) {
    return (a.aSinPin == b.aSinPin && a.aClkPin == b.aClkPin &&
            a.aClrPin == b.aClrPin && a.aRckPin == b.aRckPin);
}

// ---------------------------------------------------------------------------

CtrlBus::CtrlBus() {}

CtrlBus::CtrlBus(const CtrlBusConfig& config) {
    configure(config);
}

CtrlBus::~CtrlBus() {}

void CtrlBus::configure(const CtrlBusConfig& config) {
    config_ = config;
}

CtrlBusConfig CtrlBus::getConfig() const {
    return config_;
}

void CtrlBus::setCE(bool value) {
    if (!isValidConfig_()) {
        return;
    }
    gpio_.setPin(config_.cePin, value);
}

void CtrlBus::setOE(bool value) {
    if (!isValidConfig_()) {
        return;
    }
    gpio_.setPin(config_.oePin, value);
}

void CtrlBus::setWE(bool value) {
    if (!isValidConfig_()) {
        return;
    }
    gpio_.setPin(config_.wePin, value);
}

bool CtrlBus::isValidConfig_() const {
    return (config_.cePin != 0xFF && config_.oePin != 0xFF &&
            config_.wePin != 0xFF);
}

// ---------------------------------------------------------------------------

DataBus::DataBus() : data_(0) {}

DataBus::DataBus(const DataBusConfig& config) {
    configure(config);
}

DataBus::~DataBus() {}

void DataBus::configure(const DataBusConfig& config) {
    config_ = config;
    outRegister_.configure(config.dSinPin, config.dClkPin, config.dClrPin,
                           config.dRckPin);
    outRegister_.outputEnable();
    inRegister_.configure(config.dClrPin, config.dClkPin, 0xFF,
                          config.dSoutPin);
    inRegister_.chipEnable();
    outRegister_.clear();
    data_ = 0;
}

DataBusConfig DataBus::getConfig() const {
    return config_;
}

bool DataBus::writeByte(uint8_t value) {
    if (!isValidConfig_()) {
        return false;
    }
    outRegister_.writeByte(value);
    data_ = value;
    return true;
}

bool DataBus::writeWord(uint16_t value) {
    if (!isValidConfig_()) {
        return false;
    }
    outRegister_.writeWord(value);
    data_ = value;
    return true;
}

uint8_t DataBus::readByte(void) {
    if (!isValidConfig_()) {
        return 0;
    }
    inRegister_.load();
    return inRegister_.readByte(true);
}

uint16_t DataBus::readWord(void) {
    if (!isValidConfig_()) {
        return 0;
    }
    inRegister_.load();
    return inRegister_.readWord(true);
}

bool DataBus::isValidConfig_() const {
    return (config_.dClkPin != 0xFF && config_.dClrPin != 0xFF &&
            config_.dRckPin != 0xFF && config_.dSinPin != 0xFF &&
            config_.dSoutPin != 0xFF);
}

// ---------------------------------------------------------------------------

AddrBus::AddrBus() : address_(0) {}

AddrBus::AddrBus(const AddrBusConfig& config) {
    configure(config);
}

AddrBus::~AddrBus() {}

void AddrBus::configure(const AddrBusConfig& config) {
    config_ = config;
    outRegister_.configure(config.aSinPin, config.aClkPin, config.aClrPin,
                           config.aRckPin);
    outRegister_.outputEnable();
    outRegister_.clear();
    address_ = 0;
}

AddrBusConfig AddrBus::getConfig() const {
    return config_;
}

bool AddrBus::writeByte(uint8_t value) {
    if (!isValidConfig_()) {
        return false;
    }
    outRegister_.writeByte(value);
    address_ = value;
    return true;
}

bool AddrBus::writeWord(uint16_t value) {
    if (!isValidConfig_()) {
        return false;
    }
    outRegister_.writeWord(value);
    address_ = value;
    return true;
}

bool AddrBus::writeDWord(uint32_t value) {
    if (!isValidConfig_()) {
        return false;
    }
    outRegister_.writeDWord(value);
    address_ = value;
    return true;
}

bool AddrBus::increment() {
    if (!isValidConfig_()) {
        return false;
    }
    address_++;
    outRegister_.writeDWord(address_);
    return true;
}

uint32_t AddrBus::get() const {
    return address_;
}

bool AddrBus::isValidConfig_() const {
    return (config_.aClkPin != 0xFF && config_.aClrPin != 0xFF &&
            config_.aRckPin != 0xFF && config_.aSinPin != 0xFF);
}
