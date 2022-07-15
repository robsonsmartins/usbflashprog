// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef QSERIALPORT_H // NOLINT
#define QSERIALPORT_H
#define QSERIALPORTINFO_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QList>
#include <QIODevice>
#include <QTimer>

// ---------------------------------------------------------------------------

class QSerialPortInfo: public QObject {
    Q_OBJECT

 public:
    explicit QSerialPortInfo(QObject *parent = nullptr): QObject(parent) {}
    QSerialPortInfo(const QSerialPortInfo &other): QObject() {
        operator=(other);
    }
    ~QSerialPortInfo() {}
    QSerialPortInfo& operator=(const QSerialPortInfo &other) {
        (void)other;
        return *this;
    }
    static QList<QSerialPortInfo> availablePorts() {
        QList<QSerialPortInfo> list;
        QSerialPortInfo info;
        list.push_back(info);
        return list;
    }
    QString portName() const {
        return "COM1";
    }
    quint16 vendorIdentifier() const {
        return 0x2E8A;
    }
    quint16 productIdentifier() const {
        return 0x000A;
    }
};

// ---------------------------------------------------------------------------

class QSerialPort: public QObject {
    Q_OBJECT

 public:
    enum Directions  {
        Input = 1,
        Output = 2,
        AllDirections = Input | Output
    };
    explicit QSerialPort(QObject *parent = nullptr)
            : QObject(parent), portName_("") {
        timer_ = new QTimer(this);
        connect(timer_, &QTimer::timeout,
                this, &QSerialPort::onTimer);
    }
    ~QSerialPort() { delete timer_; }
    void setPortName(const QString &name) {
        portName_ = name;
    }
    QString portName() const {
        return portName_;
    }
    bool open(QIODevice::OpenMode mode) {
        (void)mode;
        opened_ = true;
        timer_->start(100);
        return true;
    }
    void close() {
        timer_->stop();
        portName_ = "";
        opened_ = false;
    }
    bool isOpen() const {
        return opened_;
    }
    bool setDataTerminalReady(bool set) {
        (void)set;
        return true;
    }
    qint64 bytesAvailable() const {
        return 3;
    }
    QByteArray readAll() {
        QByteArray data;
        data.append(0xA1).append(20).append(33);
        return data;
    }
    bool clear(Directions directions = AllDirections) {
        (void)directions;
        return true;
    }
    qint64 write(const QByteArray &data) {
        return data.size();
    }
    bool flush() {
        return true;
    }

 signals:
    void readyRead();

 private slots:
    void onTimer() { emit readyRead(); }

 private:
    QString portName_;
    bool opened_;
    QTimer *timer_;
};

#endif  // QSERIALPORT_H // NOLINT
