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

constexpr const char* kSerialPortDummyPort = "COM1";
constexpr uint16_t kSerialPortDummyVendorId = 0x2E8A;
constexpr uint16_t kSerialPortDummyProductId = 0x000A;
constexpr uint8_t kSerialPortDummyData[] = {0xA1, 20, 33};

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
        return kSerialPortDummyPort;
    }
    quint16 vendorIdentifier() const {
        return kSerialPortDummyVendorId;
    }
    quint16 productIdentifier() const {
        return kSerialPortDummyProductId;
    }
};

// ---------------------------------------------------------------------------

class QSerialPort: public QObject {
    Q_OBJECT

 public:
    static bool connected;
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
        connected = true;
        timer_->start(100);
        return true;
    }
    void close() {
        timer_->stop();
        portName_ = "";
        opened_ = false;
        connected = false;
    }
    bool isOpen() const {
        return opened_;
    }
    bool setDataTerminalReady(bool set) {
        (void)set;
        if (!connected) return false;
        return true;
    }
    qint64 bytesAvailable() const {
        if (!connected) return 0;
        return sizeof(kSerialPortDummyData);
    }
    QByteArray readAll() {
        QByteArray data;
        if (!connected) return data;
        for (const auto d: kSerialPortDummyData) {
            data.append(d);
        }
        return data;
    }
    bool clear(Directions directions = AllDirections) {
        (void)directions;
        return connected;
    }
    qint64 write(const QByteArray &data) {
        if (!connected) return 0;
        return data.size();
    }
    bool flush() {
        return connected;
    }
    bool waitForBytesWritten(int msecs = 30000) {
        (void)msecs;
        return connected;
    }
    bool waitForReadyRead(int msecs = 30000) {
        (void)msecs;
        return connected;
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
