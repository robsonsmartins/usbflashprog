// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
 * @file modules/device.cpp
 * @brief Implementation of the Device operation Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "modules/device.hpp"

#include "config.hpp"
#include "modules/opcodes.hpp"

// ---------------------------------------------------------------------------

// clang-format off

/* @brief Command sequence to Unprotect an EEPROM 28C/X28 64
     (ST/Atmel/Xicor). */
constexpr Device::TDeviceCommand kDeviceCmdUnprotect28C64[] = {
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0x80},
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0x20}
};

/* @brief Command sequence to Protect an EEPROM 28C/X28 64
     (ST/Atmel/Xicor). */
constexpr Device::TDeviceCommand kDeviceCmdProtect28C64[] = {
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0xA0}
};

/* @brief Command sequence to Unprotect an EEPROM 28C/X28 256
     (ST/Atmel/Xicor). */
constexpr Device::TDeviceCommand kDeviceCmdUnprotect28C256[] = {
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x80},
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x20}
};

/* @brief Command sequence to Protect an EEPROM 28C/X28 256
     (ST/Atmel/Xicor). */
constexpr Device::TDeviceCommand kDeviceCmdProtect28C256[] = {
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0xA0}
};

// clang-format on

// ---------------------------------------------------------------------------

Device::Device() {
    settings_.twp = 0;
    settings_.twc = 0;
    settings_.flags.skipFF = false;
    settings_.flags.progWithVpp = false;
    settings_.flags.vppOePin = false;
    settings_.flags.pgmCePin = false;
    settings_.flags.pgmPositive = false;
}

void Device::init() {
    vgenConfig_.vpp.pwmPin = kVppPwmPin;
    vgenConfig_.vpp.pwmFreq = kVppPwmFreq;
    vgenConfig_.vpp.pwmMinDuty = kVppPwmMinDuty;
    vgenConfig_.vpp.pwmMaxDuty = kVppPwmMaxDuty;
    vgenConfig_.vpp.pwmSlowStepDuty = kVppPwmSlowStepDuty;
    vgenConfig_.vpp.pwmFastStepDuty = kVppPwmFastStepDuty;
    vgenConfig_.vpp.pwmToleranceToFast = kVppPwmToleranceToFast;
    vgenConfig_.vpp.adcChannel = kVppAdcChannel;
    vgenConfig_.vpp.adcVref = kVppAdcVRef;
    vgenConfig_.vpp.divider = kVppDivider;
    vgenConfig_.vpp.vTolerance = kVppVoutTolerance;
    vgenConfig_.vpp.ctrlPin = kVppCtrlPin;
    vgenConfig_.vpp.vcSinPin = kVppVcSinPin;
    vgenConfig_.vpp.vcClkPin = kVppVcClkPin;
    vgenConfig_.vpp.vcClrPin = kVppVcClrPin;
    vgenConfig_.vpp.vcRckPin = kVppVcRckPin;

    vgenConfig_.vdd.pwmPin = kVddPwmPin;
    vgenConfig_.vdd.pwmFreq = kVddPwmFreq;
    vgenConfig_.vdd.pwmMinDuty = kVddPwmMinDuty;
    vgenConfig_.vdd.pwmMaxDuty = kVddPwmMaxDuty;
    vgenConfig_.vdd.pwmSlowStepDuty = kVddPwmSlowStepDuty;
    vgenConfig_.vdd.pwmFastStepDuty = kVddPwmFastStepDuty;
    vgenConfig_.vdd.pwmToleranceToFast = kVddPwmToleranceToFast;
    vgenConfig_.vdd.adcChannel = kVddAdcChannel;
    vgenConfig_.vdd.adcVref = kVddAdcVRef;
    vgenConfig_.vdd.divider = kVddDivider;
    vgenConfig_.vdd.vTolerance = kVddVoutTolerance;
    vgenConfig_.vdd.ctrlPin = kVddCtrlPin;
    vgenConfig_.vdd.onVppPin = kVddOnVppPin;

    ctrlBusConfig_.cePin = kBusCEPin;
    ctrlBusConfig_.oePin = kBusOEPin;
    ctrlBusConfig_.wePin = kBusWEPin;

    dataBusConfig_.dSinPin = kBusDataSinPin;
    dataBusConfig_.dSoutPin = kBusDataSoutPin;
    dataBusConfig_.dClkPin = kBusDataClkPin;
    dataBusConfig_.dClrPin = kBusDataClrPin;
    dataBusConfig_.dRckPin = kBusDataRckPin;

    addrBusConfig_.aSinPin = kBusAddrSinPin;
    addrBusConfig_.aClkPin = kBusAddrClkPin;
    addrBusConfig_.aClrPin = kBusAddrClrPin;
    addrBusConfig_.aRckPin = kBusAddrRckPin;

    vgen_.configure(vgenConfig_);
    vgen_.vpp.setV(kVppInitial);
    vgen_.vdd.setV(kVddInitial);
    vgen_.start();

    dataBus_.configure(dataBusConfig_);
    addrBus_.configure(addrBusConfig_);
    ctrlBus_.configure(ctrlBusConfig_);
}

void Device::vddCtrl(bool value) {
    if (value) {
        vgen_.vdd.on();
    } else {
        vgen_.vdd.off();
    }
}

void Device::vddSetV(float value) {
    vgen_.vdd.setV(value);
}

float Device::vddGetV(void) {
    return vgen_.vdd.getV();
}

float Device::vddGetDuty(void) {
    return vgen_.vdd.getDuty();
}

float Device::vddGetCal(void) {
    return vgen_.vdd.getCalibration();
}

void Device::vddInitCal(void) {
    vgen_.vdd.initCalibration();
}

void Device::vddSaveCal(float value) {
    vgen_.vdd.saveCalibration(value);
    vgen_.vdd.setV(kVddInitial);
    sleep_ms(1);  // 1 ms
}

void Device::vddOnVpp(bool value) {
    vgen_.vdd.onVpp(value);
}

void Device::vppCtrl(bool value) {
    if (value) {
        vgen_.vpp.on();
    } else {
        vgen_.vpp.off();
    }
}

void Device::vppSetV(float value) {
    vgen_.vpp.setV(value);
}

float Device::vppGetV(void) {
    return vgen_.vpp.getV();
}

float Device::vppGetDuty(void) {
    return vgen_.vpp.getDuty();
}

float Device::vppGetCal(void) {
    return vgen_.vpp.getCalibration();
}

void Device::vppInitCal(void) {
    vgen_.vpp.initCalibration();
}

void Device::vppSaveCal(float value) {
    vgen_.vpp.saveCalibration(value);
    vgen_.vpp.setV(kVppInitial);
    sleep_ms(1);  // 1 ms
}

void Device::vppOnA9(bool value) {
    vgen_.vpp.onA9(value);
}

void Device::vppOnA18(bool value) {
    vgen_.vpp.onA18(value);
}

void Device::vppOnCE(bool value) {
    vgen_.vpp.onCE(value);
}

void Device::vppOnWE(bool value) {
    vgen_.vpp.onWE(value);
}

void Device::vppOnOE(bool value) {
    vgen_.vpp.onOE(value);
}

void Device::setCE(bool value) {
    ctrlBus_.setCE(value);
}

void Device::setWE(bool value) {
    ctrlBus_.setWE(value);
}

void Device::setOE(bool value) {
    ctrlBus_.setOE(value);
}

bool Device::addrClr() {
    return addrBus_.writeByte(0);
}

bool Device::addrInc() {
    return addrBus_.increment();
}

bool Device::addrSet(uint32_t value) {
    return addrBus_.writeDWord(value);
}

bool Device::addrSetB(uint8_t value) {
    return addrBus_.writeByte(value);
}

bool Device::addrSetW(uint16_t value) {
    return addrBus_.writeWord(value);
}

uint32_t Device::addrGet() const {
    return addrBus_.get();
}

bool Device::dataClr() {
    return dataBus_.writeByte(0);
}

bool Device::dataSet(uint8_t value) {
    return dataBus_.writeByte(value);
}

bool Device::dataSetW(uint16_t value) {
    return dataBus_.writeWord(value);
}

uint8_t Device::dataGet() {
    return dataBus_.readByte();
}

uint16_t Device::dataGetW() {
    return dataBus_.readWord();
}

void Device::setTwp(uint32_t value) {
    settings_.twp = value;
}

void Device::setTwc(uint32_t value) {
    settings_.twc = value;
}

void Device::configure(uint16_t value) {
    uint8_t flags = value & 0xFF;
    uint8_t algo = (value & 0xFF00) >> 8;
    // 0 = Skip Write 0xFF
    // 1 = Prog with VPP on
    // 2 = VPP/~OE Pin
    // 3 = ~PGM/~CE Pin
    // 4 = PGM positive
    // clang-format off
    settings_.flags.skipFF      = (flags & 0x01) != 0;
    settings_.flags.progWithVpp = (flags & 0x02) != 0;
    settings_.flags.vppOePin    = (flags & 0x04) != 0;
    settings_.flags.pgmCePin    = (flags & 0x08) != 0;
    settings_.flags.pgmPositive = (flags & 0x10) != 0;
    // clang-format on
    settings_.algo = algo;
}

bool Device::setupBus(uint8_t operation) {
    bool success = true;
    // reset bus
    // VDD off and VPP off
    vddCtrl(false);
    vppCtrl(false);
    vddOnVpp(false);
    // Clear AddrBus
    if (!addrClr()) success = false;
    // ~OE is HI
    setOE(false);
    // ~CE is HI
    setCE(false);
    if (settings_.flags.pgmPositive) {
        // PGM is LO (no prog pulse)
        setWE(true);
    } else {
        // ~PGM is HI (no prog pulse)
        setWE(false);
    }
    // VPP on xx disabled
    vppOnA9(false);
    vppOnA18(false);
    vppOnCE(false);
    vppOnOE(false);
    vppOnWE(false);
    // Clear DataBus
    if (!dataClr()) success = false;

    // setupbus
    switch (operation) {
        case kCmdDeviceOperationRead:
            // VDD Rd on
            vddCtrl(true);
            // VDD Rd on VPP
            vddOnVpp(true);
            if (settings_.flags.pgmCePin) {
                // PGM/~CE is LO
                setWE(true);
            } else {
                // ~PGM is HI
                setWE(false);
            }
            // ~CE is LO (if pin connected)
            setCE(true);
            break;
        case kCmdDeviceOperationProg:
            // VDD Wr on
            vddCtrl(true);
            if (settings_.flags.vppOePin) {
                // VDD Wr on VPP
                vddOnVpp(true);
            }
            if (settings_.flags.pgmPositive) {
                // PGM is LO
                setWE(true);
            } else {
                // ~PGM is HI
                setWE(false);
            }
            // ~CE is LO (if pin connected)
            setCE(true);
            break;
        case kCmdDeviceOperationGetId:
            // VDD Rd on
            vddCtrl(true);
            if (settings_.flags.pgmCePin) {
                // PGM/~CE is LO
                setWE(true);
            } else {
                // ~PGM is HI
                setWE(false);
            }
            // ~CE is LO (if pin connected)
            setCE(true);
            if (!settings_.flags.vppOePin) {
                // VDD Rd on VPP
                vddOnVpp(true);
            }
            // ~OE is LO
            setOE(true);
            // VPP on A9
            vppOnA9(true);
            break;
        case kCmdDeviceOperationReset:
        default:
            break;
    }
    return success;
}

Device::TByteArray Device::read(size_t count) {
    return readBuffer_(count);
}

Device::TByteArray Device::readW(size_t count) {
    return readBuffer_(count, true);
}

bool Device::write(const TByteArray& value, size_t count, bool verify) {
    return writeBuffer_(value, count, verify);
}

bool Device::writeW(const TByteArray& value, size_t count, bool verify) {
    return writeBuffer_(value, count, verify, true);
}

bool Device::writeSector(const TByteArray& sector, size_t count, bool verify) {
    return writeSector_(sector, count, verify);
}

bool Device::writeSectorW(const TByteArray& sector, size_t count, bool verify) {
    return writeSector_(sector, count, verify, true);
}

bool Device::verify(const TByteArray& value, size_t count) {
    return verifyBuffer_(value, count);
}

bool Device::verifyW(const TByteArray& value, size_t count) {
    return verifyBuffer_(value, count, true);
}

bool Device::blankCheck(size_t count) {
    return blankCheckBuffer_(count);
}

bool Device::blankCheckW(size_t count) {
    return blankCheckBuffer_(count, true);
}

bool Device::getId(uint32_t& id) {
    // Setup bus
    if (!setupBus(kCmdDeviceOperationGetId)) return false;
    bool success = true;
    uint16_t manufacturer, device;
    // Get manufacturer data (byte)
    manufacturer = dataGet();
    // Increment Address (0x01)
    if (!addrInc()) success = false;
    // Get device data (byte)
    device = dataGet();

    // Check if returned values is valid (not false positive)
    // Prepare to Read
    if (!setupBus(kCmdDeviceOperationRead)) success = false;
    uint16_t rd1 = 0xFF, rd2 = 0xFF;
    // Set Address 0x200 (A9 bit on)
    if (!addrSetW(0x200)) success = false;
    // Read Byte at Address 0x200
    rd1 = read_();
    // Read Byte at Address 0x201
    if (!addrInc()) success = false;
    rd2 = read_();
    // Check if equals data at address 0x200/0x201
    if ((rd1 & 0xFF) == manufacturer && (rd2 & 0xFF) == device) success = false;

    // return ID
    if (success) {
        id = manufacturer;
        id <<= 16;
        id |= device;
    }
    // Reset Bus
    setupBus(kCmdDeviceOperationReset);
    return success;
}

bool Device::erase() {
    // Erase entire chip
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmEPROM27:
            return erase27E_();
        default:
            return false;
    }
}

bool Device::erase27E_() {
    // 27E Algorithm
    bool success = true;
    // Addr = 0
    if (!addrClr()) success = false;
    // Data = 0xFF
    if (!dataSetW(0xFFFF)) success = false;
    // VPP on A9
    vppOnA9(true);
    if (settings_.flags.pgmPositive) {
        // PGM is HI (start erase pulse)
        setWE(false);
        sleep_ms(kErasePulseDuration);  // Erase Pulse
        // PGM is LO (end erase pulse)
        setWE(true);
    } else {
        // ~PGM is LO (start erase pulse)
        setWE(true);
        sleep_ms(kErasePulseDuration);  // Erase Pulse
        // ~PGM is HI (end erase pulse)
        setWE(false);
    }
    sleep_us(settings_.twc);  // tWC uS
    // VPP on A9 off
    vppOnA9(false);
    // PGM/~CE is LO
    if (settings_.flags.pgmCePin) setWE(true);
    return success;
}

bool Device::unprotect() {
    // Unprotect entire chip
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmEEPROM28C64:
            return protect28C_(false, false);
        case kCmdDeviceAlgorithmEEPROM28C256:
            return protect28C_(false, true);
        default:
            return false;
    }
}

bool Device::protect() {
    // Protect entire chip
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmEEPROM28C64:
            return protect28C_(true, false);
        case kCmdDeviceAlgorithmEEPROM28C256:
            return protect28C_(true, true);
        default:
            return false;
    }
}

bool Device::protect28C_(bool protect, bool is256) {
    // EEPROM 28C/X28/AT28 Algorithm
    bool success = true;
    // ~CE is LO
    setCE(true);
    // write command
    if (protect && !is256) {
        for (const TDeviceCommand& cmd : kDeviceCmdProtect28C64) {
            if (!writeAtAddr_(cmd.addr, cmd.data)) {
                success = false;
                break;
            }
        }
    } else if (protect && is256) {
        for (const TDeviceCommand& cmd : kDeviceCmdProtect28C256) {
            if (!writeAtAddr_(cmd.addr, cmd.data)) {
                success = false;
                break;
            }
        }
    } else if (!protect && !is256) {
        for (const TDeviceCommand& cmd : kDeviceCmdUnprotect28C64) {
            if (!writeAtAddr_(cmd.addr, cmd.data)) {
                success = false;
                break;
            }
        }
    } else if (!protect && is256) {
        for (const TDeviceCommand& cmd : kDeviceCmdUnprotect28C256) {
            if (!writeAtAddr_(cmd.addr, cmd.data)) {
                success = false;
                break;
            }
        }
    }
    // sleep tWC
    sleep_us(settings_.twc);
    // ~CE is HI
    setCE(false);
    return success;
}

bool Device::writeSector_(const TByteArray& sector, size_t count, bool verify,
                          bool is16bit) {
    bool success = true;
    // error getting data
    size_t required = (is16bit ? (sector.size() * 2) : sector.size());
    if (required < count) return false;
    // writes data to device, at current address
    // and increment address
    uint32_t startAddr = addrGet();
    uint32_t addr = startAddr;
    uint16_t data;
    int increment = (is16bit ? 2 : 1);
    for (int i = 0; i < sector.size(); i += increment) {
        data = (sector[i] & 0xFF);
        if (is16bit) {
            data <<= 8;
            data |= (sector[i + 1] & 0xFF);
        }
        // Write data and increment address
        if (!writeAtAddr_(addr, data, is16bit) || !addrInc()) {
            success = false;
            break;
        }
        addr++;
    }
    // sleep tWC
    sleep_us(settings_.twc);
    // error, exits
    if (!success) return false;
    // if not verify, exits
    if (!verify) return true;

    // reads and verify data from device, at current address
    // and increment address
    // Addr is start
    if (!addrSet(startAddr)) success = false;
    // read each word
    for (int i = 0; i < sector.size(); i += increment) {
        data = (sector[i] & 0xFF);
        if (is16bit) {
            data <<= 8;
            data |= (sector[i + 1] & 0xFF);
        }
        // verify and increment addr
        if (!verify_(data, is16bit) || !addrInc()) {
            success = false;
            break;
        }
    }
    return success;
}

bool Device::writeBuffer_(const TByteArray& value, size_t count, bool verify,
                          bool is16bit) {
    bool success = true;
    // error getting data
    size_t required = (is16bit ? (value.size() * 2) : value.size());
    if (required < count) return false;
    // writes data to device, at current address
    // and increment address
    uint16_t wr, rd;
    bool emptyData;
    int increment = (is16bit ? 2 : 1);
    for (int i = 0; i < value.size(); i += increment) {
        wr = (value[i] & 0xFF);
        emptyData = (wr == 0xFF);
        if (is16bit) {
            wr <<= 8;
            wr |= (value[i + 1] & 0xFF);
            emptyData = (wr == 0xFFFF);
        }
        // Write
        if (!settings_.flags.skipFF || !emptyData) {
            if (!write_(wr, is16bit)) {
                success = false;
                break;
            }
            sleep_us(settings_.twc);  // tWC uS
        }
        // Verify
        if (verify) {
            if (!verify_(wr, is16bit)) {
                success = false;
                break;
            }
        }
        // increment address
        if (!addrInc()) {
            success = false;
            break;
        }
    }
    return success;
}

Device::TByteArray Device::readBuffer_(size_t count, bool is16bit) {
    TByteArray result;
    if (!count) return result;
    uint16_t data;
    for (size_t i = 0; i < count; i++) {
        // read
        data = read_(is16bit);
        if (is16bit) {
            result.push_back((data & 0xFF00) >> 8);
            result.push_back(data & 0xFF);
        } else {
            result.push_back(data & 0xFF);
        }
        // increment address
        if (!addrInc()) {
            result.clear();
            break;
        }
    }
    return result;
}

bool Device::verifyBuffer_(const TByteArray& value, size_t count,
                           bool is16bit) {
    bool success = true;
    // error getting data
    size_t required = (is16bit ? (value.size() * 2) : value.size());
    if (required < count) return false;
    // verify data to device, at current address
    // and increment address
    uint16_t wr, rd;
    int increment = (is16bit ? 2 : 1);
    for (int i = 0; i < value.size(); i += increment) {
        wr = (value[i] & 0xFF);
        if (is16bit) {
            wr <<= 8;
            wr |= (value[i + 1] & 0xFF);
        }
        // Verify
        if (!verify_(wr, is16bit)) {
            success = false;
            break;
        }
        // increment address
        if (!addrInc()) {
            success = false;
            break;
        }
    }
    return success;
}

bool Device::blankCheckBuffer_(size_t count, bool is16bit) {
    bool success = true;
    // verify data to device, at current address
    // and increment address
    uint16_t rd, wr = (is16bit ? 0xFFFF : 0xFF);
    for (size_t i = 0; i < count; i++) {
        // Verify
        if (!verify_(wr, is16bit)) {
            success = false;
            break;
        }
        // increment address
        if (!addrInc()) {
            success = false;
            break;
        }
    }
    return success;
}

uint16_t Device::read_(bool is16bit) {
    uint16_t data;
    // ~OE/VPP is LO
    if (settings_.flags.vppOePin) vddOnVpp(false);
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
    if (settings_.flags.vppOePin) vddOnVpp(true);
    return data;
}

bool Device::write_(uint16_t data, bool is16bit) {
    bool success = true;
    if (settings_.flags.progWithVpp) {
        // VPP on
        vddOnVpp(false);
        vppCtrl(true);
    }
    // Set DataBus
    if (is16bit) {
        if (!dataSetW(data)) success = false;
    } else {
        if (!dataSet(data & 0xFF)) success = false;
    }
    if (settings_.flags.pgmPositive) {
        // PGM is HI (start prog pulse)
        setWE(false);
        sleep_us(settings_.twp);  // tWP uS
        // PGM is LO (end prog pulse)
        setWE(true);
    } else {
        // ~PGM is LO (start prog pulse)
        setWE(true);
        sleep_us(settings_.twp);  // tWP uS
        // ~PGM is HI (end prog pulse)
        setWE(false);
    }
    if (settings_.flags.progWithVpp) {
        // VPP off
        vppCtrl(false);
        vddOnVpp(true);
    }
    return success;
}

bool Device::verify_(uint16_t data, bool is16bit) {
    uint16_t rd, wr = data;
    // Read
    // PGM/~CE is LO
    if (settings_.flags.pgmCePin) setWE(true);
    // read
    rd = read_(is16bit);
    // PGM/~CE is HI
    if (settings_.flags.pgmCePin) setWE(false);
    // verify
    if (!is16bit) {
        wr &= 0xFF;
        rd &= 0xFF;
    }
    return (wr == rd);
}

bool Device::writeAtAddr_(uint32_t addr, uint16_t data, bool is16bit) {
    bool success = true;
    if (!addrSet(addr)) success = false;
    if (!write_(data, is16bit)) success = false;
    // sleep tWP
    sleep_us(settings_.twp);
    return success;
}
