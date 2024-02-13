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
 * @file backend/devices/device.cpp
 * @brief Implementation of the Base class of a Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QMap>
#include <QTextStream>
#include <QLoggingCategory>

#include "backend/devices/device.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(device, "device")

#define DEBUG qCDebug(device)
#define INFO qCInfo(device)
#define WARNING qCWarning(device)
#define CRITICAL qCCritical(device)
#define FATAL qCFatal(device)

// ---------------------------------------------------------------------------

// clang-format off
/* @brief List of Manufacturers. */
const QMap<uint8_t, QString> kManufacturerList = {
    {0x01, "AMD"                    },
    {0x02, "Macronix"               },
    {0x04, "Fujitsu"                },
    {0x07, "Maxwell"                },
    {0x0C, "Microchip"              },
    {0x29, "Microchip"              },
    {0x15, "NXP/Philips/Signetics"  },
    {0x1C, "EON"                    },
    {0x1E, "Atmel"                  },
    {0x1F, "Atmel"                  },
    {0x20, "ST"                     },
    {0x31, "Catalyst"               },
    {0x37, "AMIC"                   },
    {0x38, "Winbond"                },
    {0x40, "SyncMOS"                },
    {0x42, "SyncMOS"                },
    {0x49, "Toshiba"                },
    {0x4A, "Macronix/MXIC"          },
    {0xC2, "Macronix/MXIC"          },
    {0x50, "Spansion"               },
    {0x7F, "Adesto"                 },
    {0x89, "Intel/Texas Instruments"},
    {0x8F, "Fairchild/National"     },
    {0x9D, "Xicor"                  },
    {0xAD, "Hyundai"                },
    {0xB0, "Sharp"                  },
    {0xBF, "SST/Sanyo"              },
    {0xC8, "GigaDevice"             },
    {0xDA, "ASD/WinBond"            },
    {0xEF, "ISSI"                   }
};
// clang-format on

// ---------------------------------------------------------------------------

TDeviceID::TDeviceID() : manufacturer(0), device(0) {}

QString TDeviceID::getManufacturerName(void) const {
    if (!kManufacturerList.contains(manufacturer)) {
        return Device::tr("Unknown");
    }
    return kManufacturerList.value(manufacturer);
}

// ---------------------------------------------------------------------------

TDeviceCapabilities::TDeviceCapabilities()
    : hasProgram(false),
      hasVerify(false),
      hasErase(false),
      hasGetId(false),
      hasRead(false),
      hasBlankCheck(false),
      hasUnprotect(false),
      hasProtect(false),
      hasSectorSize(false),
      hasFastProg(false),
      hasSkipFF(false),
      hasVDD(false),
      hasVPP(false) {}

// ---------------------------------------------------------------------------

TDeviceInformation::TDeviceInformation()
    : deviceType(kDeviceParallelMemory), name("") {}

QString TDeviceInformation::toString() const {
    QString result;
    QTextStream ts(&result);
    ts << "Name: " << name;
    ts << "; Type: ";
    switch (deviceType) {
        case kDeviceParallelMemory:
            ts << "Parallel Memory";
            break;
        case kDeviceSerialMemory:
            ts << "Serial Memory";
            break;
        default:
            ts << "Unknown";
            break;
    }
    ts << "; Capabilities: [";
    ts << "hasProgram=" << (capability.hasProgram ? 1 : 0);
    ts << ", hasVerify=" << (capability.hasVerify ? 1 : 0);
    ts << ", hasErase=" << (capability.hasErase ? 1 : 0);
    ts << ", hasGetId=" << (capability.hasGetId ? 1 : 0);
    ts << ", hasRead=" << (capability.hasRead ? 1 : 0);
    ts << ", hasBlankCheck=" << (capability.hasBlankCheck ? 1 : 0);
    ts << ", hasUnprotect=" << (capability.hasUnprotect ? 1 : 0);
    ts << ", hasProtect=" << (capability.hasProtect ? 1 : 0);
    ts << ", hasSectorSize=" << (capability.hasSectorSize ? 1 : 0);
    ts << ", hasFastProg=" << (capability.hasFastProg ? 1 : 0);
    ts << ", hasSkipFF=" << (capability.hasSkipFF ? 1 : 0);
    ts << ", hasVDD=" << (capability.hasVDD ? 1 : 0);
    ts << ", hasVPP=" << (capability.hasVPP ? 1 : 0);
    ts << "]";
    return result;
}

// ---------------------------------------------------------------------------

Device::Device(QObject *parent)
    : QObject(parent),
      maxAttemptsProg_(1),
      canceling_(false),
      size_(0),
      twp_(1),
      twc_(1),
      vddRd_(5.0f),
      vddWr_(5.0f),
      vpp_(12.0f),
      vee_(12.0f),
      skipFF_(false),
      fastProg_(false),
      sectorSize_(0),
      algo_(kCmdDeviceAlgorithmUnknown),
      runner_(this) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "Device";
    info_.capability.hasProgram = false;
    info_.capability.hasVerify = false;
    info_.capability.hasErase = false;
    info_.capability.hasGetId = false;
    info_.capability.hasRead = false;
    info_.capability.hasBlankCheck = false;
    info_.capability.hasUnprotect = false;
    info_.capability.hasProtect = false;
    info_.capability.hasSectorSize = false;
    info_.capability.hasFastProg = false;
    info_.capability.hasSkipFF = false;
    info_.capability.hasVDD = true;
    info_.capability.hasVPP = false;
    flags_.skipFF = false;
    flags_.progWithVpp = false;
    flags_.vppOePin = false;
    flags_.pgmCePin = false;
    flags_.pgmPositive = false;
    flags_.is16bit = false;
}

Device::~Device() {}

bool Device::is16Bit() const {
    return flags_.is16bit;
}

void Device::setPort(const QString &path) {
    if (port_ == path) return;
    port_ = path;
}

QString Device::getPort() const {
    return port_;
}

uint8_t Device::getBufferSize() const {
    return runner_.getBufferSize();
}

void Device::setBufferSize(uint8_t value) {
    runner_.setBufferSize(value);
}

void Device::setSize(uint32_t value) {
    if (size_ != value) {
        if (value) size_ = value;
    }
    DEBUG << "Size: " << QString("%1").arg(size_);
}

uint32_t Device::getSize() const {
    return size_;
}

void Device::setTwp(uint32_t us) {
    if (twp_ != us) {
        if (us) twp_ = us;
    }
    DEBUG << "tWP: " << QString("%1").arg(twp_);
}

uint32_t Device::getTwp() const {
    return twp_;
}

void Device::setTwc(uint32_t us) {
    if (twc_ != us) {
        if (us) twc_ = us;
    }
    DEBUG << "tWC: " << QString("%1").arg(twc_);
}

uint32_t Device::getTwc() const {
    return twc_;
}

void Device::setVddRd(float value) {
    if (vddRd_ != value) {
        if (value >= 0.0f) vddRd_ = value;
    }
    DEBUG << "VDD Read: " << QString("%1").arg(vddRd_, 4, 'f', 2);
}

float Device::getVddRd() const {
    return vddRd_;
}

void Device::setVddWr(float value) {
    if (vddWr_ != value) {
        if (value >= 0.0f) vddWr_ = value;
    }
    DEBUG << "VDD Prog: " << QString("%1").arg(vddWr_, 4, 'f', 2);
}

float Device::getVddWr() const {
    return vddWr_;
}

void Device::setVpp(float value) {
    if (vpp_ != value) {
        if (value >= 0.0f) vpp_ = value;
    }
    DEBUG << "VPP: " << QString("%1").arg(vpp_, 4, 'f', 2);
}

float Device::getVpp() const {
    return vpp_;
}

void Device::setVee(float value) {
    if (vee_ != value) {
        if (value >= 0.0f) vee_ = value;
    }
    DEBUG << "VEE: " << QString("%1").arg(vee_, 4, 'f', 2);
}

float Device::getVee() const {
    return vee_;
}

void Device::setSkipFF(bool value) {
    if (skipFF_ != value) skipFF_ = value;
    DEBUG << "Skip 0xFF: " << QString("%1").arg(skipFF_ ? 1 : 0);
}

bool Device::getSkipFF() const {
    return skipFF_;
}

void Device::setFastProg(bool value) {
    if (fastProg_ != value) fastProg_ = value;
    DEBUG << "Fast Prog/Erase: " << QString("%1").arg(fastProg_ ? 1 : 0);
}

bool Device::getFastProg() const {
    return fastProg_;
}

void Device::setSectorSize(uint16_t value) {
    if (sectorSize_ != value) sectorSize_ = value;
    DEBUG << "Sector Size: " << QString("%1").arg(sectorSize_);
}

uint16_t Device::getSectorSize() const {
    return sectorSize_;
}

TDeviceInformation Device::getInfo() const {
    return info_;
}

void Device::cancel() {
    if (canceling_) return;
    canceling_ = true;
}

bool Device::getId(TDeviceID &result) {
    (void)result;
    return false;
}

bool Device::read(QByteArray &buffer) {
    (void)buffer;
    return false;
}

bool Device::program(const QByteArray &buffer, bool verify) {
    (void)buffer;
    (void)verify;
    return false;
}

bool Device::verify(const QByteArray &buffer) {
    (void)buffer;
    return false;
}

bool Device::erase(bool check) {
    (void)check;
    return false;
}

bool Device::blankCheck() {
    return false;
}

bool Device::unprotect() {
    return false;
}

bool Device::protect() {
    return false;
}
