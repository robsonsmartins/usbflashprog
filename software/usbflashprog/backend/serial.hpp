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
 * @file backend/serial.hpp
 * @brief Header of the Serial Port Communication Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_SERIAL_HPP_
#define BACKEND_SERIAL_HPP_

#include <QString>
#include <QSerialPortInfo>
#include <QSerialPort>

// ---------------------------------------------------------------------------

class Serial {
 public:
    typedef QList<QSerialPortInfo> TSerialPortList;
    Serial();
    TSerialPortList list() const;
    bool open(const QString &path);
    void close();
    bool isOpen() const;
    QString getPath() const;
    void write(const void *src, size_t len, bool flush = false);
    size_t read(void *src, size_t len);

 private:
    QSerialPort port_;
};

#endif  // BACKEND_SERIAL_HPP_
