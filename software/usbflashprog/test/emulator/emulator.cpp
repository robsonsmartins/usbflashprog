// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/emulator/emulator.cpp
 * @brief Implementation of Emulator Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QRandomGenerator>
#include <chrono>
#include <thread>

#include "../../backend/devices/device.hpp"
#include "emulator.hpp"

// ---------------------------------------------------------------------------

static BaseParChip* globalEmuParChip_ = nullptr;

// ---------------------------------------------------------------------------

/* @brief Define the SEND CMD macro. */
#define SEND_CMD(x) deviceSendCmd_(x, sizeof(x), false)
/* @brief Define the SEND CMD (for READ) macro. */
#define SEND_CMD_RD(x) deviceSendCmd_(x, sizeof(x), true)

/* @brief Define the CHECK STATUS macro. */
#define CHECK_STATUS deviceCheckStatus_()

// ---------------------------------------------------------------------------

Emulator::Emulator(QObject* parent)
    : vdd_(5.0f),
      vpp_(12.0f),
      timeout_(200),
      running_(false),
      error_(false),
      address_(0),
      bufferSize_(1),
      twp_(1),
      twc_(1),
      algo_(kCmdDeviceAlgorithmUnknown) {
    // clang-format off
    flags_.skipFF      = false;
    flags_.progWithVpp = false;
    flags_.vppOePin    = false;
    flags_.pgmCePin    = false;
    flags_.pgmPositive = false;
    flags_.is16bit     = false;
    // clang-format on
}

Emulator::~Emulator() {
    close();
}

TSerialPortList Emulator::list() const {
    return QSerialPortInfo::availablePorts();
}

bool Emulator::open(const QString& path) {
    if (running_) close();
    if (path.isNull() || path.isEmpty()) return false;
    path_ = path;
    running_ = true;
    error_ = false;
    return true;
}

void Emulator::close() {
    running_ = false;
    error_ = false;
    path_.clear();
}

bool Emulator::isOpen() const {
    return running_;
}

bool Emulator::hasError() const {
    return error_;
}

QString Emulator::getPath() const {
    return path_;
}

uint32_t Emulator::getTimeOut() const {
    return timeout_;
}

void Emulator::setTimeOut(uint32_t value) {
    if (timeout_ == value) return;
    timeout_ = value;
}

uint8_t Emulator::getBufferSize() const {
    return bufferSize_;
}

void Emulator::setBufferSize(uint8_t value) {
    if (!value) value = 1;
    // rounds to next power of two
    if (value & (value - 1)) {
        uint32_t exp = 1;
        // clang-format off
        for (exp = 1; (1 << exp) < value; exp++) {}
        // clang-format on
        if (exp > 7) exp = 7;  // max 128
        value = 1 << exp;
    }
    if (flags_.is16bit && value == 1) value = 2;
    if (bufferSize_ == value) return;
    bufferSize_ = value;
}

bool Emulator::nop() {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vddCtrl(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setVDD(on);
    return true;
}

bool Emulator::vddSet(float value) {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    vdd_ = value;
    return true;
}

float Emulator::vddGet() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    return vdd_;
}

float Emulator::vddGetDuty() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return 50.0f;
}

bool Emulator::vddInitCal() {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vddSaveCal(float value) {
    (void)value;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

float Emulator::vddGetCal() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return vdd_;
}

bool Emulator::vppCtrl(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setVPP(on);
    return true;
}

bool Emulator::vppSet(float value) {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    vpp_ = value;
    return true;
}

float Emulator::vppGet() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    return vpp_;
}

float Emulator::vppGetDuty() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return 50.0f;
}

bool Emulator::vppInitCal() {
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppSaveCal(float value) {
    (void)value;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

float Emulator::vppGetCal() {
    if (error_ || !running_) {
        error_ = true;
        return -1.0f;
    }
    // fake
    return vpp_;
}

bool Emulator::vddOnVpp(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnA9(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnA18(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnCE(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnOE(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::vppOnWE(bool on) {
    (void)on;
    if (error_ || !running_) {
        error_ = true;
        return false;
    }
    return true;
}

bool Emulator::setCE(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setCE(on);
    return true;
}

bool Emulator::setOE(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setOE(on);
    return true;
}

bool Emulator::setWE(bool on) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setWE(on);
    return true;
}

bool Emulator::addrClr() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = 0;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrInc() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_++;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrSet(uint32_t value) {
    if (!value) return addrClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = value;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrSetB(uint8_t value) {
    if (!value) return addrClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = value;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

bool Emulator::addrSetW(uint16_t value) {
    if (!value) return addrClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    address_ = value;
    globalEmuParChip_->setAddrBus(address_);
    return true;
}

uint32_t Emulator::addrGet() const {
    return address_;
}

bool Emulator::dataClr() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setDataBus(0);
    return true;
}

bool Emulator::dataSet(uint8_t value) {
    if (!value) return dataClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setDataBus(value);
    return true;
}

bool Emulator::dataSetW(uint16_t value) {
    if (!value) return dataClr();
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    globalEmuParChip_->setDataBus(value);
    return true;
}

uint8_t Emulator::dataGet() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return 0xff;
    }
    uint16_t data = globalEmuParChip_->getDataBus();
    return static_cast<uint8_t>(data & 0xff);
}

uint16_t Emulator::dataGetW() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return 0xffff;
    }
    return globalEmuParChip_->getDataBus();
}

bool Emulator::deviceSetTwp(uint32_t value) {
    if (value == twp_) return true;
    twp_ = value;
    return true;
}

bool Emulator::deviceSetTwc(uint32_t value) {
    if (value == twc_) return true;
    twc_ = value;
    return true;
}

bool Emulator::deviceConfigure(kCmdDeviceAlgorithmEnum algo,
                               const TDeviceFlags& flags) {
    // clang-format off
    flags_.skipFF      = flags.skipFF     ;
    flags_.progWithVpp = flags.progWithVpp;
    flags_.vppOePin    = flags.vppOePin   ;
    flags_.pgmCePin    = flags.pgmCePin   ;
    flags_.pgmPositive = flags.pgmPositive;
    flags_.is16bit     = flags.is16bit    ;
    // clang-format on
    algo_ = algo;
    if (flags.is16bit && bufferSize_ == 1) setBufferSize(2);
    return true;
}

bool Emulator::deviceSetupBus(kCmdDeviceOperationEnum operation) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    return deviceSetupBus_(operation);
}

bool Emulator::deviceResetBus() {
    return deviceSetupBus(kCmdDeviceOperationReset);
}

QByteArray Emulator::deviceRead() {
    QByteArray result;
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return result;
    }
    uint16_t data;
    int increment = flags_.is16bit ? 2 : 1;
    for (int i = 0; i < bufferSize_; i += increment) {
        data = deviceRead_();
        // inc address
        addrInc();
        if (error_) {
            result.clear();
            break;
        }
        if (flags_.is16bit) result.append((data & 0xFF00) >> 8);
        result.append(data & 0xFF);
    }
    return result;
}

bool Emulator::deviceWrite(const QByteArray& data) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    if (data.size() != bufferSize_) return false;
    uint32_t startAddr = addrGet();
    uint16_t rd, wr;
    int increment = flags_.is16bit ? 2 : 1;
    for (int i = 0; i < bufferSize_; i += increment) {
        wr = data[i] & 0xFF;
        if (flags_.is16bit) {
            wr <<= 8;
            wr |= (data[i + 1] & 0xFF);
        }
        if (!deviceWrite_(wr)) return false;
        // PGM/~CE is LO
        if (flags_.pgmCePin) setWE(true);
        // read
        rd = deviceRead_(true);
        // verify
        if (!flags_.is16bit) {
            rd &= 0xFF;
            wr &= 0xFF;
        }
        if (error_ || rd != wr) return false;
        // inc address
        addrInc();
    }
    if (error_) return false;
    return true;
}

bool Emulator::deviceWriteSector(const QByteArray& data, uint16_t sectorSize) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    if (data.size() != sectorSize) return false;
    uint32_t startAddr = addrGet();
    uint16_t rd, wr;
    int increment = flags_.is16bit ? 2 : 1;
    for (int i = 0; i < sectorSize; i += increment) {
        wr = data[i] & 0xFF;
        if (flags_.is16bit) {
            wr <<= 8;
            wr |= (data[i + 1] & 0xFF);
        }
        if (!deviceWrite_(wr)) return false;
        // inc address
        addrInc();
    }
    if (error_) return false;
    // PGM/~CE is LO
    if (flags_.pgmCePin) setWE(true);
    addrSet(startAddr);
    for (int i = 0; i < sectorSize; i += increment) {
        wr = data[i] & 0xFF;
        if (flags_.is16bit) {
            wr <<= 8;
            wr |= (data[i + 1] & 0xFF);
        }
        // read
        rd = deviceRead_();
        // verify
        if (!flags_.is16bit) {
            rd &= 0xFF;
            wr &= 0xFF;
        }
        if (error_ || rd != wr) return false;
        // inc address
        addrInc();
    }
    if (error_) return false;
    return true;
}

bool Emulator::deviceVerify(const QByteArray& data) {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    if (data.size() != bufferSize_) return false;
    uint16_t rd, wr;
    int increment = flags_.is16bit ? 2 : 1;
    // PGM/~CE is LO
    if (flags_.pgmCePin) setWE(true);
    for (int i = 0; i < bufferSize_; i += increment) {
        wr = data[i] & 0xFF;
        if (flags_.is16bit) {
            wr <<= 8;
            wr |= (data[i + 1] & 0xFF);
        }
        // read
        rd = deviceRead_();
        // verify
        if (!flags_.is16bit) {
            rd &= 0xFF;
            wr &= 0xFF;
        }
        if (error_ || rd != wr) return false;
        // inc address
        addrInc();
    }
    if (error_) return false;
    return true;
}

bool Emulator::deviceBlankCheck() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    uint16_t rd, wr = 0xFFFF;
    int increment = flags_.is16bit ? 2 : 1;
    // PGM/~CE is LO
    if (flags_.pgmCePin) setWE(true);
    for (int i = 0; i < bufferSize_; i += increment) {
        // read
        rd = deviceRead_();
        if (!flags_.is16bit) {
            rd &= 0xFF;
            wr &= 0xFF;
        }
        // verify
        if (error_ || rd != wr) return false;
        // inc address
        addrInc();
    }
    if (error_) return false;
    return true;
}

TDeviceID Emulator::deviceGetId() {
    TDeviceID result;
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return result;
    }
    result = deviceGetId_();
    return result;
}

bool Emulator::deviceErase() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    return deviceErase_();
}

bool Emulator::deviceUnprotect() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    return deviceProtect_(false);
}

bool Emulator::deviceProtect() {
    if (error_ || !running_ || !globalEmuParChip_) {
        error_ = true;
        return false;
    }
    return deviceProtect_(true);
}

void Emulator::usDelay(uint64_t value) {
    (void)value;
    return;
}

void Emulator::msDelay(uint32_t value) {
    (void)value;
    return;
}

void Emulator::processEvents() {
    return;
}

void Emulator::setChip(BaseParChip* chip) {
    if (globalEmuParChip_ == chip) return;
    globalEmuParChip_ = chip;
}

void Emulator::randomizeBuffer(QByteArray& buffer, uint32_t size) {
    if (!size) return;
    buffer.resize(size);
    for (int i = 0; i < buffer.size(); ++i) {
        buffer[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
}

uint16_t Emulator::deviceRead_(bool fromProg, bool sendCmd) {
    uint16_t data;
    // Send read command (if in the algorithm)
    if (sendCmd) {
        if (fromProg) {
            deviceSendCmdVerify_();
        } else {
            deviceSendCmdRead_();
        }
    }
    // ~OE/VPP is LO
    if (flags_.vppOePin) vddOnVpp(false);
    // ~OE is LO
    setOE(true);
    // get data
    if (flags_.is16bit) {
        data = dataGetW();
    } else {
        data = dataGet();
    }
    // ~OE is HI
    setOE(false);
    // ~OE/VPP is VDD
    if (flags_.vppOePin) vddOnVpp(true);
    return data;
}

bool Emulator::deviceWrite_(uint16_t value, bool disableSkipFF, bool disableVpp,
                            bool sendCmd) {
    bool success = true;
    bool emptyData = flags_.is16bit ? (value == 0xFFFF) : (value == 0xFF);
    // write
    if (!flags_.skipFF || !emptyData || disableSkipFF) {
        if (flags_.progWithVpp && !disableVpp) {
            // VPP on
            vddOnVpp(false);
            vppCtrl(true);
        }
        // Send write command (if in the algorithm)
        if (sendCmd) deviceSendCmdWrite_();
        // Set DataBus
        if (flags_.is16bit) {
            dataSetW(value);
        } else {
            dataSet(value);
        }
        if (flags_.pgmPositive) {
            // PGM is HI (start prog pulse)
            setWE(false);
            usDelay(twp_);  // tWP uS
            // PGM is LO (end prog pulse)
            setWE(true);
        } else {
            // ~PGM is LO (start prog pulse)
            setWE(true);
            usDelay(twp_);  // tWP uS
            // ~PGM is HI (end prog pulse)
            setWE(false);
        }
        usDelay(twc_);  // tWC uS
        // check status (if in the algorithm)
        if (sendCmd) {
            if (!deviceCheckStatus_()) success = false;
        }
        if (flags_.progWithVpp && !disableVpp) {
            // VPP off
            vppCtrl(false);
            vddOnVpp(true);
        }
        if (error_) return false;
    }
    return success;
}

bool Emulator::writeAtAddr_(uint32_t addr, uint16_t data, bool disableVpp,
                            bool sendCmd) {
    bool success = true;
    if (!addrSet(addr)) success = false;
    if (!deviceWrite_(data, true, disableVpp, sendCmd)) success = false;
    // sleep tWP
    usDelay(twp_);
    return success;
}

uint16_t Emulator::readAtAddr_(uint32_t addr, bool sendCmd) {
    // Read one byte/word at address
    // set address
    if (!addrSet(addr)) return (flags_.is16bit ? 0xFFFF : 0xFF);
    // read data
    return deviceRead_(false, sendCmd);
}

bool Emulator::deviceSendCmd_(const TDeviceCommand* cmd, size_t size,
                              bool disableVpp) {
    // Send a command to device
    bool success = true;
    // calculate cmd count items
    int count = size / sizeof(TDeviceCommand);
    uint32_t addr;
    for (int i = 0; i < count; i++) {
        // get addr (current or cmd defined)
        addr = (cmd[i].addr == ANY_ADDRESS) ? addrGet() : cmd[i].addr;
        // write at address, without call sendcmd itself
        if (!writeAtAddr_(addr, cmd[i].data, disableVpp, false)) {
            success = false;
            break;
        }
    }
    return success;
}

bool Emulator::deviceCheckStatus_() {
    // check status byte
    uint8_t status = kDeviceStatusByteOkI28F;
    switch (algo_) {
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

bool Emulator::deviceSendCmdRead_() {
    switch (algo_) {
        case kCmdDeviceAlgorithmFlash28F:
            return SEND_CMD_RD(kDeviceCmdRead28F);
        case kCmdDeviceAlgorithmFlashAm28F:
            return SEND_CMD_RD(kDeviceCmdReadAm28F);
        case kCmdDeviceAlgorithmFlashI28F:
            return SEND_CMD_RD(kDeviceCmdReadI28F);
        default:
            return true;
    }
}

bool Emulator::deviceSendCmdWrite_() {
    switch (algo_) {
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

bool Emulator::deviceSendCmdVerify_() {
    switch (algo_) {
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

bool Emulator::deviceSendCmdErase_() {
    switch (algo_) {
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

bool Emulator::deviceSendCmdGetId_() {
    switch (algo_) {
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

bool Emulator::deviceSetupBus_(kCmdDeviceOperationEnum operation) {
    // reset bus
    // VDD off and VPP off
    vddCtrl(false);
    vppCtrl(false);
    vddOnVpp(false);
    // Clear AddrBus
    addrClr();
    // ~OE is HI
    setOE(false);
    // ~CE is HI
    setCE(false);
    if (flags_.pgmPositive) {
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
    dataClr();

    // setupbus
    switch (operation) {
        case kCmdDeviceOperationRead:
            // VDD Rd on
            vddCtrl(true);
            // VDD Rd on VPP
            vddOnVpp(true);
            if (flags_.pgmCePin) {
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
            if (flags_.pgmPositive) {
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
            if (flags_.pgmCePin) {
                // PGM/~CE is LO
                setWE(true);
            } else {
                // ~PGM is HI
                setWE(false);
            }
            // ~CE is LO (if pin connected)
            setCE(true);
            if (!flags_.vppOePin) {
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
    usDelay(150);  // 150 uS
    return !error_;
}

TDeviceID Emulator::deviceGetId_() {
    TDeviceID result;
    // Setup bus
    if (!deviceSetupBus_(kCmdDeviceOperationGetId)) return result;
    uint16_t manufacturer, device;
    // Send GetID command
    deviceSendCmdGetId_();

    // Get manufacturer data
    manufacturer = flags_.is16bit ? dataGetW() : dataGet();
    // Increment Address (0x01)
    addrInc();
    // Get device data
    device = flags_.is16bit ? dataGetW() : dataGet();

    // If success, return data
    if (!error_) {
        result.manufacturer = manufacturer;
        result.device = device;
    }
    // Close resources
    deviceSetupBus_(kCmdDeviceOperationReset);
    return result;
}

bool Emulator::deviceErase_() {
    // Erase entire chip
    switch (algo_) {
        case kCmdDeviceAlgorithmEPROM:
            return deviceErase27E_();
        case kCmdDeviceAlgorithmFlash28F:
        case kCmdDeviceAlgorithmFlashSST28SF:
        case kCmdDeviceAlgorithmFlashAm28F:
        case kCmdDeviceAlgorithmFlashI28F:
            if (deviceSendCmdErase_()) {
                msDelay(kDeviceEraseDelay28F);  // Erase delay
                return true;
            } else {
                return false;
            }
        default:
            return false;
    }
}

void Emulator::disableSDP_() {
    // Disable SDP
    switch (algo_) {
        case kCmdDeviceAlgorithmFlashSST28SF:
            disableSdpFlashSST28SF_();
            return;
        default:
            return;
    }
}

bool Emulator::deviceErase27E_() {
    // Erase entire chip
    // 27E Algorithm
    // Addr = 0
    addrClr();
    // Data = 0xFF
    dataSetW(0xFFFF);
    // VPP on A9
    vppOnA9(true);
    if (flags_.pgmPositive) {
        // PGM is HI (start erase pulse)
        setWE(false);
        msDelay(100);  // Erase Pulse
        // PGM is LO (end erase pulse)
        setWE(true);
    } else {
        // ~PGM is LO (start erase pulse)
        setWE(true);
        msDelay(100);  // Erase Pulse
        // ~PGM is HI (end erase pulse)
        setWE(false);
    }
    usDelay(twc_);  // tWC uS
    // VPP on A9 off
    vppOnA9(false);
    // PGM/~CE is LO
    if (flags_.pgmCePin) setWE(true);
    return !error_;
}

bool Emulator::deviceProtect_(bool protect) {
    switch (algo_) {
        case kCmdDeviceAlgorithmEEPROM28C64:
            return deviceProtect28C_(protect, false);
        case kCmdDeviceAlgorithmEEPROM28C256:
            return deviceProtect28C_(protect, true);
        default:
            return false;
    }
}

bool Emulator::deviceProtect28C_(bool protect, bool is256) {
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
    usDelay(twc_);
    // ~CE is HI
    setCE(false);
    return success;
}

void Emulator::disableSdpFlashSST28SF_() {
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