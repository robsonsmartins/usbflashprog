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
 * @file backend/runner.hpp
 * @brief Header of the Runner Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_RUNNER_HPP_
#define BACKEND_RUNNER_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QMutex>
#include <QQueue>
#include <QByteArray>

#include "backend/opcodes.hpp"
#include "backend/serial.hpp"

// ---------------------------------------------------------------------------

typedef struct TRunnerCommand {
    TCmdOpCode opcode;
    QByteArray params;
    QByteArray response;
    bool responseIsOk() const;
    float responseAsFloat() const;
    int responseAsByte() const;
    bool responseAsBool() const;
    void set(kCmdOpCodeEnum code);
    void set(kCmdOpCodeEnum code, float param);
    void set(kCmdOpCodeEnum code, int param);
    void set(kCmdOpCodeEnum code, bool param);
    TRunnerCommand& operator=(const TRunnerCommand& src);
    friend bool operator==(const TRunnerCommand& a, const TRunnerCommand& b);
} TRunnerCommand;

// ---------------------------------------------------------------------------

class Runner: public QObject {
    Q_OBJECT

 public:
    explicit Runner(QObject *parent = nullptr);
    ~Runner();
    Serial::TSerialPortList list() const;
    bool open(const QString &path);
    void close();
    bool isOpen() const;
    QString getPath() const;

    void send(kCmdOpCodeEnum code);
    void send(kCmdOpCodeEnum code, int param);
    void send(kCmdOpCodeEnum code, bool param);
    void send(kCmdOpCodeEnum code, float param);

 signals:
    void resultReady(const TRunnerCommand& command);

 private slots:
    void onSerial_readyRead(qint64 bytesAvailable);

 private:
    Serial serial_;
    QQueue<TRunnerCommand> wrFifo_;
    QByteArray rdBuffer_;
    QMutex mutex_;
    qint64 aliveTick_;
    bool running_;

    void processData_();
    void write_(const TRunnerCommand &cmd);
    void checkAlive_();
};

#endif  // BACKEND_RUNNER_HPP_
