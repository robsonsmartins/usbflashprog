// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup Software
 * @file backend/serial.cpp
 * @brief Implementation of the Serial Port Communication Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "backend/serial.hpp"
#include "config.hpp"

// ---------------------------------------------------------------------------

Serial::Serial() {}

Serial::TSerialPortList Serial::list() const {
    Serial::TSerialPortList result;
    for (const auto item : QSerialPortInfo::availablePorts()) {
        if (item.vendorIdentifier() == kUsbVendorId &&
                item.productIdentifier() == kUsbProductId) {
            result.push_back(item);
        }
    }
    return result;
}

bool Serial::open(const QString &path) {
    if (path.isNull() || path.isEmpty()) { return false; }
    if (port_.isOpen()) { port_.close(); }
    port_.setPortName(path);
    return port_.open(QIODevice::ReadWrite);
}

void Serial::close() {
    port_.close();
}

bool Serial::isOpen() const {
    return port_.isOpen();
}

QString Serial::getPath() const {
    return port_.portName();
}

void Serial::write(const void *src, size_t len, bool flush) {
    if (!src || !len || !port_.isOpen()) { return; }
    port_.write(static_cast<const char*>(src), len);
    if (flush) { port_.flush(); }
}

size_t Serial::read(void *src, size_t len) {
    if (!src || !len || !port_.isOpen()) { return 0; }
    qint64 result = port_.read(static_cast<char*>(src), len);
    return (result <= 0) ? 0 : result;
}
