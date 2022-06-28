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

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QSerialPortInfo>
#include <QSerialPort>

// ---------------------------------------------------------------------------

class Serial: public QObject {
    Q_OBJECT

 public:
    typedef QList<QSerialPortInfo> TSerialPortList;
    explicit Serial(QObject *parent = nullptr);
    ~Serial();
    TSerialPortList list() const;
    bool open(const QString &path);
    void close();
    bool isOpen() const;
    QString getPath() const;
    void write(const void *src, size_t len, bool flush = false);
    void write(const QByteArray &src, bool flush = false);
    size_t read(void *src, size_t len, int msecs = 0);
    QByteArray read(size_t len, int msecs = 0);
    QByteArray readAll();
    void purge();

 signals:
    void readyRead(qint64 bytesAvailable);

 private slots:
    void onPort_readyRead();
 private:
    QSerialPort port_;
};

#endif  // BACKEND_SERIAL_HPP_
