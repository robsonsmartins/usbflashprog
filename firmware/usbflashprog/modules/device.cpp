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

/* @brief Define the SEND CMD macro. */
#define SEND_CMD(x) sendCmd_(x, sizeof(x), false)
/* @brief Define the SEND RD CMD macro. */
#define SEND_RD_CMD(x) sendCmd_(x, sizeof(x), true)

/* @brief Define the CHECK STATUS macro. */
#define CHECK_STATUS checkStatus_()

// ---------------------------------------------------------------------------

Device::Device() {
    settings_.twp = 0;
    settings_.twc = 0;
    settings_.flags.skipFF = false;
    settings_.flags.progWithVpp = false;
    settings_.flags.vppOePin = false;
    settings_.flags.pgmCePin = false;
    settings_.flags.pgmPositive = false;
    settings_.flags.is16bit = false;
    settings_.algo = kCmdDeviceAlgorithmUnknown;
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

Device::TDeviceSettings Device::getSettings() const {
    return settings_;
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
    /* 0 = Skip Write 0xFF
       1 = Prog with VPP on
       2 = VPP/~OE Pin
       3 = ~PGM/~CE Pin
       4 = PGM positive
       5 = 16-bit mode */
    // clang-format off
    settings_.flags.skipFF      = (flags & 0x01) != 0;
    settings_.flags.progWithVpp = (flags & 0x02) != 0;
    settings_.flags.vppOePin    = (flags & 0x04) != 0;
    settings_.flags.pgmCePin    = (flags & 0x08) != 0;
    settings_.flags.pgmPositive = (flags & 0x10) != 0;
    settings_.flags.is16bit     = (flags & 0x20) != 0;
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
            // Disable Software Data Protection (if any)
            disableSDP_();
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
    // Read Buffer
    TByteArray result;
    if (!count) return result;
    uint16_t data;
    for (size_t i = 0; i < count; i++) {
        // read
        data = read_();
        if (settings_.flags.is16bit) {
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

bool Device::write(const TByteArray& value, size_t count, bool verify) {
    // Write Buffer
    bool success = true;
    // error getting data
    size_t required =
        (settings_.flags.is16bit ? (value.size() * 2) : value.size());
    if (required < count) return false;
    // writes data to device, at current address
    // and increment address
    uint16_t data;
    bool emptyData;
    int increment = (settings_.flags.is16bit ? 2 : 1);
    for (int i = 0; i < value.size(); i += increment) {
        data = (value[i] & 0xFF);
        emptyData = (data == 0xFF);
        if (settings_.flags.is16bit) {
            data <<= 8;
            data |= (value[i + 1] & 0xFF);
            emptyData = (data == 0xFFFF);
        }
        // Write
        if (!settings_.flags.skipFF || !emptyData) {
            if (!write_(data)) {
                success = false;
                break;
            }
            sleep_us(settings_.twc);  // tWC uS
        }
        // Verify
        if (verify) {
            if (!verify_(data, true)) {
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

bool Device::writeSector(const TByteArray& sector, size_t count, bool verify) {
    // Write Sector
    bool success = true;
    // error getting data
    size_t required =
        (settings_.flags.is16bit ? (sector.size() * 2) : sector.size());
    if (required < count) return false;
    // writes data to device, at current address
    // and increment address
    uint32_t startAddr = addrGet();
    uint32_t addr = startAddr;
    uint16_t data;
    int increment = (settings_.flags.is16bit ? 2 : 1);
    for (int i = 0; i < sector.size(); i += increment) {
        data = (sector[i] & 0xFF);
        if (settings_.flags.is16bit) {
            data <<= 8;
            data |= (sector[i + 1] & 0xFF);
        }
        // Write data and increment address
        if (!writeAtAddr_(addr, data) || !addrInc()) {
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
        if (settings_.flags.is16bit) {
            data <<= 8;
            data |= (sector[i + 1] & 0xFF);
        }
        // verify and increment addr
        if (!verify_(data) || !addrInc()) {
            success = false;
            break;
        }
    }
    return success;
}

bool Device::verify(const TByteArray& value, size_t count) {
    // Verify Buffer
    bool success = true;
    // error getting data
    size_t required =
        (settings_.flags.is16bit ? (value.size() * 2) : value.size());
    if (required < count) return false;
    // verify data to device, at current address
    // and increment address
    uint16_t data;
    int increment = (settings_.flags.is16bit ? 2 : 1);
    for (int i = 0; i < value.size(); i += increment) {
        data = (value[i] & 0xFF);
        if (settings_.flags.is16bit) {
            data <<= 8;
            data |= (value[i + 1] & 0xFF);
        }
        // Verify
        if (!verify_(data)) {
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

bool Device::erase() {
    // Erase entire chip
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmEPROM:
            return erase27E_();
        case kCmdDeviceAlgorithmFlash28F:
        case kCmdDeviceAlgorithmFlashSST28SF:
        case kCmdDeviceAlgorithmFlashAm28F:
        case kCmdDeviceAlgorithmFlashI28F:
            if (sendCmdErase_()) {
                sleep_ms(kDeviceEraseDelay28F);  // Erase delay
                return true;
            } else {
                return false;
            }
        default:
            return false;
    }
}

bool Device::blankCheck(size_t count) {
    // BlankCheck Buffer
    bool success = true;
    // verify data to device, at current address
    // and increment address
    for (size_t i = 0; i < count; i++) {
        // Verify
        if (!blankCheck_()) {
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

bool Device::getId(uint32_t& id) {
    // GetID

    // Setup bus
    if (!setupBus(kCmdDeviceOperationGetId)) return false;
    bool success = true;
    uint16_t manufacturer, device;
    // Send GetID command
    sendCmdGetId_();

    // Get manufacturer data
    manufacturer = settings_.flags.is16bit ? dataGetW() : dataGet();
    // Increment Address (0x01)
    if (!addrInc()) success = false;
    // Get device data
    device = settings_.flags.is16bit ? dataGetW() : dataGet();

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
    if (rd1 == manufacturer && rd2 == device) success = false;

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

uint16_t Device::read_(bool sendCmd) {
    // Read one byte/word
    uint16_t data;
    // Send read command (if in the algorithm)
    if (sendCmd) sendCmdRead_();
    // ~OE/VPP is LO
    if (settings_.flags.vppOePin) vddOnVpp(false);
    // ~OE is LO
    setOE(true);
    // get data
    if (settings_.flags.is16bit) {
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

bool Device::write_(uint16_t data, bool disableVpp, bool sendCmd) {
    // Write one byte/word
    bool success = true;
    if (settings_.flags.progWithVpp && !disableVpp) {
        // VPP on
        vddOnVpp(false);
        vppCtrl(true);
    }
    // Send write command (if in the algorithm)
    if (sendCmd) {
        if (!sendCmdWrite_()) success = false;
    }
    // Set DataBus
    if (settings_.flags.is16bit) {
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
    // check status (if in the algorithm)
    if (sendCmd) {
        if (!checkStatus_()) success = false;
    }
    if (settings_.flags.progWithVpp && !disableVpp) {
        // VPP off
        vppCtrl(false);
        vddOnVpp(true);
    }
    return success;
}

bool Device::verify_(uint16_t data, bool fromProg, bool sendCmd) {
    // Verify one byte/word
    bool success = true;
    uint16_t rd, wr = data;
    // Send verify/read command (if in the algorithm)
    if (sendCmd) {
        if (fromProg) {
            if (!sendCmdVerify_()) success = false;
        } else {
            if (!sendCmdRead_()) success = false;
        }
    }
    // Read
    // PGM/~CE is LO
    if (settings_.flags.pgmCePin) setWE(true);
    // read
    rd = read_();
    // PGM/~CE is HI
    if (settings_.flags.pgmCePin) setWE(false);
    // verify
    if (!settings_.flags.is16bit) {
        wr &= 0xFF;
        rd &= 0xFF;
    }
    if (wr != rd) success = false;
    return success;
}

bool Device::blankCheck_(bool sendCmd) {
    // Blank Check one byte/word
    bool success = true;
    uint16_t rd, wr = 0xFFFF;
    // Send verify/read command (if in the algorithm)
    if (sendCmd) {
        if (!sendCmdRead_()) success = false;
    }
    // Read
    // PGM/~CE is LO
    if (settings_.flags.pgmCePin) setWE(true);
    // read
    rd = read_();
    // PGM/~CE is HI
    if (settings_.flags.pgmCePin) setWE(false);
    // verify
    if (!settings_.flags.is16bit) {
        wr &= 0xFF;
        rd &= 0xFF;
    }
    if (wr != rd) success = false;
    return success;
}

bool Device::writeAtAddr_(uint32_t addr, uint16_t data, bool disableVpp,
                          bool sendCmd) {
    // Write one byte/word at address
    bool success = true;
    // set address
    if (!addrSet(addr)) success = false;
    // write data
    if (!write_(data, disableVpp, sendCmd)) success = false;
    // sleep tWP
    sleep_us(settings_.twp);
    return success;
}

uint16_t Device::readAtAddr_(uint32_t addr, bool sendCmd) {
    // Read one byte/word at address
    // set address
    if (!addrSet(addr)) return (settings_.flags.is16bit ? 0xFFFF : 0xFF);
    // read data
    return read_(sendCmd);
}

bool Device::sendCmd_(const TDeviceCommand* cmd, size_t size, bool rdCmd) {
    // Send a command to device
    bool success = true;
    // calculate cmd count items
    int count = size / sizeof(TDeviceCommand);
    uint32_t addr;
    for (int i = 0; i < count; i++) {
        // get addr (current or cmd defined)
        addr = (cmd[i].addr == ANY_ADDRESS) ? addrGet() : cmd[i].addr;
        // write at address, without call sendcmd itself
        if (!writeAtAddr_(addr, cmd[i].data, rdCmd, false)) {
            success = false;
            break;
        }
    }
    return success;
}

bool Device::checkStatus_() {
    // check status byte
    uint8_t status = kDeviceStatusByteOkI28F;
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlashI28F:
            // ~OE is LO
            setOE(true);
            // get status byte
            status = dataGet();
            // ~OE is HI
            setOE(false);
            // Status == OK
            return (status & 0xFE) == kDeviceStatusByteOkI28F;
        default:
            return true;
    }
}

bool Device::sendCmdRead_() {
    // Read CMD
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlash28F:
            return SEND_RD_CMD(kDeviceCmdRead28F);
        case kCmdDeviceAlgorithmFlashAm28F:
            return SEND_RD_CMD(kDeviceCmdReadAm28F);
        case kCmdDeviceAlgorithmFlashI28F:
            return SEND_RD_CMD(kDeviceCmdReadI28F);
        default:
            return true;
    }
}

bool Device::sendCmdWrite_() {
    // Write CMD
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlash28F:
            return SEND_CMD(kDeviceCmdWrite28F);
        case kCmdDeviceAlgorithmFlashSST28SF:
            return SEND_CMD(kDeviceCmdWriteSST28SF);
        case kCmdDeviceAlgorithmFlashAm28F:
            return SEND_CMD(kDeviceCmdWriteAm28F);
        case kCmdDeviceAlgorithmFlashI28F:
            return SEND_CMD(kDeviceCmdWriteI28F);
        default:
            return true;
    }
}

bool Device::sendCmdVerify_() {
    // Verify CMD (from Programming)
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlash28F:
            return SEND_CMD(kDeviceCmdVerify28F);
        case kCmdDeviceAlgorithmFlashAm28F:
            return SEND_CMD(kDeviceCmdReadAm28F);
        case kCmdDeviceAlgorithmFlashI28F:
            return SEND_CMD(kDeviceCmdReadI28F);
        default:
            return true;
    }
}

bool Device::sendCmdErase_() {
    // Erase CMD
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlash28F:
            return SEND_CMD(kDeviceCmdErase28F);
        case kCmdDeviceAlgorithmFlashSST28SF:
            return SEND_CMD(kDeviceCmdEraseSST28SF);
        case kCmdDeviceAlgorithmFlashAm28F:
            return SEND_CMD(kDeviceCmdEraseAm28F);
        case kCmdDeviceAlgorithmFlashI28F:
            return SEND_CMD(kDeviceCmdEraseI28F) && CHECK_STATUS;
        default:
            return true;
    }
}

bool Device::sendCmdGetId_() {
    // GetID CMD
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlash28F:
            return SEND_CMD(kDeviceCmdGetId28F);
        case kCmdDeviceAlgorithmFlashSST28SF:
            return SEND_CMD(kDeviceCmdGetIdSST28SF);
        case kCmdDeviceAlgorithmFlashAm28F:
            return SEND_CMD(kDeviceCmdGetIdAm28F);
        case kCmdDeviceAlgorithmFlashI28F:
            return SEND_CMD(kDeviceCmdGetIdI28F);
        default:
            return true;
    }
}

void Device::disableSDP_() {
    // Disable SDP
    switch (settings_.algo) {
        case kCmdDeviceAlgorithmFlashSST28SF:
            disableSdpFlashSST28SF_();
            return;
        default:
            return;
    }
}

bool Device::erase27E_() {
    // 27E Erase Algorithm
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
        sleep_ms(kDeviceErasePulseDuration27E);  // Erase Pulse
        // PGM is LO (end erase pulse)
        setWE(true);
    } else {
        // ~PGM is LO (start erase pulse)
        setWE(true);
        sleep_ms(kDeviceErasePulseDuration27E);  // Erase Pulse
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

bool Device::protect28C_(bool protect, bool is256) {
    // EEPROM 28C/X28/AT28 Protect/Unprotect Algorithm
    bool success = true;
    // ~CE is LO
    setCE(true);
    // write command
    if (protect && !is256) {
        success = SEND_CMD(kDeviceCmdProtect28C64);
    } else if (protect && is256) {
        success = SEND_CMD(kDeviceCmdProtect28C256);
    } else if (!protect && !is256) {
        success = SEND_CMD(kDeviceCmdUnprotect28C64);
    } else if (!protect && is256) {
        success = SEND_CMD(kDeviceCmdUnprotect28C256);
    }
    // sleep tWC
    sleep_us(settings_.twc);
    // ~CE is HI
    setCE(false);
    return success;
}

void Device::disableSdpFlashSST28SF_() {
    // calculate cmd count items
    int count = sizeof(kDeviceCmdDisableSdpSST28SF) / sizeof(TDeviceCommand);
    uint32_t addr;
    for (int i = 0; i < count; i++) {
        // get addr (cmd defined)
        addr = kDeviceCmdDisableSdpSST28SF[i].addr;
        // read at address, without call sendcmd itself
        readAtAddr_(addr, false);
    }
    addrClr();
}
