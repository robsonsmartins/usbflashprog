// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/parallel/pdevice.cpp
 * @brief Implementation of a Parallel Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QByteArray>
#include <QRandomGenerator>
#include <QLoggingCategory>

#include "backend/devices/parallel/pdevice.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(devicePar, "device.parallel")

#define DEBUG qCDebug(devicePar)
#define INFO qCInfo(devicePar)
#define WARNING qCWarning(devicePar)
#define CRITICAL qCCritical(devicePar)
#define FATAL qCFatal(devicePar)

// ---------------------------------------------------------------------------

ParDevice::ParDevice(QObject *parent) : Device(parent) {
    info_.deviceType = kDeviceParallelMemory;
    info_.name = "Parallel Device";
    size_ = 2048;
    DEBUG << info_.toString();
}

ParDevice::~ParDevice() {}

bool ParDevice::read(QByteArray &buffer) {
    INFO << "Reading device...";
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Read operation
    if (!initDevice(kDeviceOpRead)) {
        WARNING << "Error reading device";
        return false;
    }
    bool error = false;
    // Read the device
    if (!readDevice(buffer)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error reading device";
    } else {
        INFO << "Reading device OK";
    }
    return !error;
}

bool ParDevice::program(const QByteArray &buffer, bool verify) {
    INFO << "Programming device...";
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Prog operation
    if (!initDevice(kDeviceOpProg)) {
        WARNING << "Error programming device";
        return false;
    }
    bool error = false;
    // Program the device
    if (!programDevice(buffer)) error = true;
    // Close resources
    finalizeDevice();
    // If error, returns
    if (error) {
        WARNING << "Error programming device";
        return false;
    }
    // If no error and verify flag is disabled, return
    if (!verify) {
        emit onProgress(total, total, true);
        INFO << "Programming device OK";
        return true;
    }
    // If verify flag is enabled, verify device
    return this->verify(buffer);
}

bool ParDevice::verify(const QByteArray &buffer) {
    INFO << "Verifying device...";
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Read operation
    if (!initDevice(kDeviceOpRead)) {
        WARNING << "Error verifying device";
        return false;
    }
    bool error = false;
    // Read the device and verify (compare) buffer
    if (!verifyDevice(buffer)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error verifying device";
    } else {
        INFO << "Verifying device OK";
    }
    return !error;
}

bool ParDevice::erase(bool check) {
    INFO << "Erasing device...";
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Erase operation
    if (!initDevice(kDeviceOpErase)) {
        WARNING << "Error erasing device";
        return false;
    }
    bool error = false;
    // Erase the device
    if (!eraseDevice()) error = true;
    // Close resources
    finalizeDevice();
    // If error, returns
    if (error) {
        WARNING << "Error erasing device";
        return false;
    }
    // If no error and check flag is disabled, return
    if (!check) {
        emit onProgress(total, total, true);
        INFO << "Erasing device OK";
        return true;
    }
    // If check flag is enabled, verify device
    return blankCheck();
}

bool ParDevice::blankCheck() {
    INFO << "Blank checking device...";
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Read operation
    if (!initDevice(kDeviceOpRead)) {
        WARNING << "Error blank checking device";
        return false;
    }
    bool error = false;
    // Blank check the device
    if (!blankCheckDevice()) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error blank checking device";
    } else {
        INFO << "Blank checking device OK";
    }
    return !error;
}

bool ParDevice::getId(TDeviceID &result) {
    INFO << "Getting device ID...";
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Get ID operation
    if (!initDevice(kDeviceOpGetId)) {
        WARNING << "Error getting device ID";
        return false;
    }
    bool error = false;
    // Getting the device ID
    if (!getIdDevice(result)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error getting device ID";
    } else {
        INFO << "Getting device ID OK";
    }
    return !error;
}

bool ParDevice::unprotect() {
    INFO << "Unprotecting device...";
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Prog operation
    if (!initDevice(kDeviceOpProg)) {
        WARNING << "Error unprotecting device";
        return false;
    }
    bool error = false;
    // Unprotect the device
    if (!protectDevice(false)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error unprotecting device";
    } else {
        INFO << "Unprotecting device OK";
    }
    return !error;
}

bool ParDevice::protect() {
    INFO << "Protecting device...";
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    canceling_ = false;
    // Init pins/bus to Prog operation
    if (!initDevice(kDeviceOpProg)) {
        WARNING << "Error protecting device";
        return false;
    }
    bool error = false;
    // Protect the device
    if (!protectDevice(true)) error = true;
    if (!error) emit onProgress(total, total, true);
    // Close resources
    finalizeDevice();
    if (error) {
        WARNING << "Error protecting device";
    } else {
        INFO << "Protecting device OK";
    }
    return !error;
}

bool ParDevice::programDevice(const QByteArray &buffer) {
    DEBUG << "Programming data...";
    uint32_t current = 0;
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (flags_.is16bit) total /= 2;
    uint16_t data = 0xFFFF;
    int increment = (flags_.is16bit ? 2 : 1);
    if (!runner_.deviceSetTwp(twp_) || !runner_.deviceSetTwc(twc_)) {
        emit onProgress(current, total, true, false);
        WARNING << "Program error: setting tWP or tWC";
        return false;
    }
    QByteArray block;
    int blockSize = (sectorSize_ ? sectorSize_ : getBufferSize());
    uint32_t count = blockSize;
    if (flags_.is16bit && count >= 2) count /= 2;
    int i = 0;
    bool success;
    while (i < buffer.size()) {
        // Repeat for n max attempts
        for (int attempt = 1; attempt <= maxAttemptsProg_; attempt++) {
            if ((current % 0x100) == 0) emit onProgress(current, total);
            runner_.processEvents();
            if (canceling_) {
                emit onProgress(current, total, true, false, true);
                DEBUG << QString("Program canceled at 0x%1 of 0x%2")
                             .arg(current, 6, 16, QChar('0'))
                             .arg(total, 6, 16, QChar('0'));
                return false;
            }

            // Repeat for each byte/word in block size
            block.clear();
            do {
                data = buffer[i] & 0xFF;
                if (flags_.is16bit) {
                    data <<= 8;                      // MSB
                    data |= (buffer[i + 1] & 0xFF);  // LSB
                }
                // Insert data into block buffer
                if (flags_.is16bit) block.append((data & 0xFF00) >> 8);
                block.append(data & 0xFF);
                i += increment;
            } while (block.size() < blockSize);  // one block

            // Write data
            if (sectorSize_) {
                // Write (and verify) sector
                success = runner_.deviceWriteSector(block, sectorSize_);
            } else {
                // Write (and verify) block
                success = runner_.deviceWrite(block);
            }

            // increment address
            if (success) {
                current += count;
                break;
            } else {
                i -= blockSize;
            }

            // Error
            if (attempt == maxAttemptsProg_) {
                emit onProgress(current, total, true, false);
                data = buffer[i] & 0xFF;
                if (flags_.is16bit) {
                    data <<= 8;                      // MSB
                    data |= (buffer[i + 1] & 0xFF);  // LSB
                }
                WARNING << QString(
                               "Program error at 0x%1 of 0x%2. Data to "
                               "write 0x%3")
                               .arg(current, 6, 16, QChar('0'))
                               .arg(total, 6, 16, QChar('0'))
                               .arg(data, flags_.is16bit ? 4 : 2, 16,
                                    QChar('0'));
                return false;
            }
        }
    }
    DEBUG << "Program OK";
    return true;
}

bool ParDevice::verifyDevice(const QByteArray &buffer) {
    DEBUG << "Verifying data...";
    uint32_t current = 0;
    uint32_t total = qMin(size_, static_cast<uint32_t>(buffer.size()));
    if (flags_.is16bit) total /= 2;
    uint16_t data = 0xFFFF;
    int increment = (flags_.is16bit ? 2 : 1);
    QByteArray block;
    int blockSize = getBufferSize();
    uint32_t count = blockSize;
    if (flags_.is16bit && count >= 2) count /= 2;
    bool success;
    int i = 0;
    for (current = 0; current < total; current += count) {
        if ((current % 0x100) == 0) emit onProgress(current, total);
        runner_.processEvents();
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Verify canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }

        // Repeat for each byte/word in block size
        block.clear();
        do {
            if (i >= buffer.size()) break;
            data = buffer[i] & 0xFF;
            if (flags_.is16bit) {
                data <<= 8;                      // MSB
                data |= (buffer[i + 1] & 0xFF);  // LSB
            }
            // Insert data into block buffer
            if (flags_.is16bit) block.append((data & 0xFF00) >> 8);
            block.append(data & 0xFF);
            i += increment;
        } while (block.size() < blockSize);  // one block

        // Verify block
        success = runner_.deviceVerify(block);

        // Error
        if (!success) {
            emit onProgress(current, total, true, false);
            i -= blockSize;
            data = buffer[i] & 0xFF;
            if (flags_.is16bit) {
                data <<= 8;                      // MSB
                data |= (buffer[i + 1] & 0xFF);  // LSB
            }
            WARNING << QString(
                           "Verify error at 0x%1 of 0x%2. Expected data 0x%3")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'))
                           .arg(data, flags_.is16bit ? 4 : 2, 16, QChar('0'));
            return false;
        }
    }
    DEBUG << "Verify OK";
    return true;
}

bool ParDevice::readDevice(QByteArray &buffer) {
    DEBUG << "Reading data...";
    uint32_t current = 0;
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    int blockSize = getBufferSize();
    uint32_t count = blockSize;
    if (flags_.is16bit && count >= 2) count /= 2;
    QByteArray data;
    buffer.clear();
    bool success;
    for (current = 0; current < total; current += count) {
        if (current % 0x100 == 0) emit onProgress(current, total);
        runner_.processEvents();
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Read canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        // Read block
        data = runner_.deviceRead();
        success = (data.size() == blockSize);
        // Error
        if (!success) {
            emit onProgress(current, total, true, false);
            WARNING << QString("Read error at 0x%1 of 0x%2")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'));
            return false;
        }
        // Copy data
        buffer.append(data);
    }
    DEBUG << "Read OK";
    return true;
}

bool ParDevice::eraseDevice() {
    DEBUG << "Erasing data...";
    uint32_t current = 0;
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    uint32_t count = getBufferSize();
    if (flags_.is16bit && count >= 2) count /= 2;
    if (!runner_.deviceSetTwp(twp_) || !runner_.deviceSetTwc(twc_)) {
        emit onProgress(current, total, true, false);
        WARNING << "Erase error: setting tWP or tWC";
        return false;
    }
    // VPP on
    runner_.vppCtrl(true);
    // Repeat for n max attempts
    for (int attempt = 1; attempt <= maxAttemptsProg_; attempt++) {
        // Erase entire chip
        bool success = true;
        if (!runner_.deviceErase()) success = false;
        for (current = 0; current < total; current += count) {
            if (current % 0x100 == 0) emit onProgress(current, total);
            runner_.processEvents();
            if (canceling_) {
                emit onProgress(current, total, true, false, true);
                DEBUG << QString("Erase canceled at 0x%1 of 0x%2")
                             .arg(current, 6, 16, QChar('0'))
                             .arg(total, 6, 16, QChar('0'));
                return false;
            }
            // Verify data, if not in Fast Erase mode
            if (!fastProg_) {
                // Check block
                if (success && !runner_.deviceBlankCheck()) {
                    success = false;
                }
            }
            if (fastProg_ && success) break;
            if (!success && attempt == maxAttemptsProg_) {
                // Error
                emit onProgress(current, total, true, false);
                WARNING << QString(
                               "Erase error at 0x%1 of 0x%2. Expected data "
                               "0x%3")
                               .arg(current, 6, 16, QChar('0'))
                               .arg(total, 6, 16, QChar('0'))
                               .arg(flags_.is16bit ? "FFFF" : "FF");
                return false;
            }
            if (!success) break;
        }
        if (success) break;
    }
    // VPP off
    runner_.vppCtrl(false);
    DEBUG << "Erase OK";
    return true;
}

bool ParDevice::blankCheckDevice() {
    DEBUG << "Blank checking data...";
    uint32_t current = 0;
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    int increment = flags_.is16bit ? 2 : 1;
    uint32_t count = getBufferSize();
    if (flags_.is16bit && count >= 2) count /= 2;
    bool success;
    for (current = 0; current < total; current += count) {
        if ((current % 0x100) == 0) emit onProgress(current, total);
        runner_.processEvents();
        if (canceling_) {
            emit onProgress(current, total, true, false, true);
            DEBUG << QString("Blank Check canceled at 0x%1 of 0x%2")
                         .arg(current, 6, 16, QChar('0'))
                         .arg(total, 6, 16, QChar('0'));
            return false;
        }
        // Check block
        success = runner_.deviceBlankCheck();
        // Error
        if (!success) {
            emit onProgress(current, total, true, false);
            WARNING << QString("Blank Check error at 0x%1 of 0x%2")
                           .arg(current, 6, 16, QChar('0'))
                           .arg(total, 6, 16, QChar('0'));
            return false;
        }
    }
    DEBUG << "Blank Check OK";
    return true;
}

bool ParDevice::getIdDevice(TDeviceID &deviceId) {
    DEBUG << "Getting ID...";
    uint32_t current = 0;
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    // Getting ID
    deviceId = runner_.deviceGetId();
    if (!deviceId.manufacturer && !deviceId.device) {
        emit onProgress(current, total, true, false);
        WARNING << "Error getting ID";
        return false;
    }
    DEBUG << "Get ID OK";
    return true;
}

bool ParDevice::protectDevice(bool protect) {
    DEBUG << (protect ? "Protecting..." : "Unprotecting...");
    uint32_t current = 0;
    uint32_t total = size_;
    if (flags_.is16bit) total /= 2;
    if (!runner_.deviceSetTwp(twp_) || !runner_.deviceSetTwc(twc_)) {
        emit onProgress(current, total, true, false);
        WARNING << "Protect/Unprotect error: setting tWP or tWC";
        return false;
    }
    // Protect/Unprotect
    bool success;
    if (protect) {
        success = runner_.deviceProtect();
    } else {
        success = runner_.deviceUnprotect();
    }
    if (!success) {
        emit onProgress(current, total, true, false);
        WARNING << "Error protecting/unprotecting";
        return false;
    }
    DEBUG << (protect ? "Protect OK" : "Unprotect OK");
    return true;
}

bool ParDevice::initDevice(kDeviceOperation operation) {
    DEBUG << "Initializing...";
    if (!runner_.open(port_)) {
        emit onProgress(0, size_, true, false);
        WARNING << "Error opening serial port";
        return false;
    }
    if (!runner_.deviceConfigure(algo_, flags_)) {
        emit onProgress(0, size_, true, false);
        WARNING << "Error configuring device";
        return false;
    }
    bool success = true;
    switch (operation) {
        case kDeviceOpRead:
            // VDD Rd
            if (!runner_.vddSet(vddRd_) ||
                !runner_.deviceSetupBus(kCmdDeviceOperationRead)) {
                success = false;
            }
            break;
        case kDeviceOpProg:
            // VDD Wr / VPP
            if (!runner_.vddSet(vddWr_) || !runner_.vppSet(vpp_) ||
                !runner_.deviceSetupBus(kCmdDeviceOperationProg)) {
                success = false;
            }
            break;
        case kDeviceOpErase:
            // VDD Wr / VEE
            if (!runner_.vddSet(vddWr_) || !runner_.vppSet(vee_) ||
                !runner_.deviceSetupBus(kCmdDeviceOperationProg)) {
                success = false;
            }
            break;
        case kDeviceOpGetId:
            // VDD Rd / VEE
            if (!runner_.vddSet(vddRd_) || !runner_.vppSet(vee_) ||
                !runner_.deviceResetBus()) {
                success = false;
            }
            break;
        case kDeviceOpReset:
        default:
            if (!runner_.deviceResetBus()) success = false;
            break;
    }
    if (success) {
        DEBUG << "Initialize OK";
    } else {
        DEBUG << "Error initializing device";
    }
    return success;
}

void ParDevice::finalizeDevice() {
    DEBUG << "Finalizing...";
    runner_.deviceResetBus();
    runner_.close();
    DEBUG << "Finalize OK";
}

bool ParDevice::finalizeDevice(uint32_t current, uint32_t total, bool done,
                               bool success, bool canceled) {
    finalizeDevice();
    emit onProgress(current, total, done, success, canceled);
    return success;
}

QByteArray ParDevice::generateRandomData_() {
    DEBUG << "Generating Random Data...";
    QByteArray buffer(size_, 0);
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(QRandomGenerator::global()->generate());
    }
    return buffer;
}

QByteArray ParDevice::generatePatternData_() {
    DEBUG << "Generating Pattern Data...";
    QByteArray buffer(size_, 0);
    uint8_t data = 0x55;
    for (int i = 0; i < size_; ++i) {
        buffer[i] = static_cast<char>(data);
        data = ~data;
    }
    return buffer;
}
