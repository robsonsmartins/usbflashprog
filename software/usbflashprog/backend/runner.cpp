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
 * @file backend/runner.cpp
 * @brief Implementation of the Runner Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QDateTime>
#include "backend/runner.hpp"

// ---------------------------------------------------------------------------

constexpr int kReadTimeOut = 5000;

// ---------------------------------------------------------------------------

bool TRunnerCommand::responseIsOk() const {
    return OpCode::isOk(response.data(), response.size());
}

float TRunnerCommand::responseAsFloat() const {
    return OpCode::getValueAsFloat(response.data(), response.size());
}

int TRunnerCommand::responseAsByte() const {
    return OpCode::getValueAsByte(response.data(), response.size());
}

bool TRunnerCommand::responseAsBool() const {
    return OpCode::getValueAsBool(response.data(), response.size());
}

void TRunnerCommand::set(kCmdOpCodeEnum code) {
    opcode = OpCode::getOpCode(code);
    params.resize(1);
    params[0] = opcode.code;
}

void TRunnerCommand::set(kCmdOpCodeEnum code, float param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setValue(params.data(), params.size(), param);
}

void TRunnerCommand::set(kCmdOpCodeEnum code, int param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setValue(params.data(), params.size(), param);
}

void TRunnerCommand::set(kCmdOpCodeEnum code, bool param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setValue(params.data(), params.size(), param);
}

TRunnerCommand& TRunnerCommand::operator=(const TRunnerCommand& src) {
    this->opcode   =   src.opcode;
    this->params   =   src.params;
    this->response = src.response;
    return *this;
}

bool operator==(const TRunnerCommand& a, const TRunnerCommand& b) {
    return (a.opcode   ==   b.opcode &&
            a.params   ==   b.params &&
            a.response == b.response);
}

// ---------------------------------------------------------------------------

Runner::Runner(QObject *parent): QObject(parent),
                                 serial_(this), running_(false) {
    connect(&serial_, &Serial::readyRead,
            this, &Runner::onSerial_readyRead);
}

Runner::~Runner() {
    close();
}

Serial::TSerialPortList Runner::list() const {
    return serial_.list();
}

bool Runner::open(const QString &path) {
    if (running_) { close(); }
    QMutexLocker locker(&mutex_);
    running_ = true;
    return serial_.open(path);
}

void Runner::close() {
    QMutexLocker locker(&mutex_);
    serial_.close();
    running_ = false;
}

bool Runner::isOpen() const {
    return running_;
}

QString Runner::getPath() const {
    return serial_.getPath();
}

void Runner::send(kCmdOpCodeEnum code) {
    if (wrFifo_.size() >= 1) { checkAlive_(); }
    if (!serial_.isOpen()) { return; }
    QMutexLocker locker(&mutex_);
    TRunnerCommand cmd;
    cmd.set(code);
    wrFifo_.enqueue(cmd);
    if (wrFifo_.size() == 1) { write_(cmd); }
}

void Runner::send(kCmdOpCodeEnum code, int param) {
    if (wrFifo_.size() >= 1) { checkAlive_(); }
    if (!serial_.isOpen()) { return; }
    QMutexLocker locker(&mutex_);
    TRunnerCommand cmd;
    cmd.set(code, param);
    wrFifo_.enqueue(cmd);
    if (wrFifo_.size() == 1) { write_(cmd); }
}

void Runner::send(kCmdOpCodeEnum code, bool param) {
    if (wrFifo_.size() >= 1) { checkAlive_(); }
    if (!serial_.isOpen()) { return; }
    QMutexLocker locker(&mutex_);
    TRunnerCommand cmd;
    cmd.set(code, param);
    wrFifo_.enqueue(cmd);
    if (wrFifo_.size() == 1) { write_(cmd); }
}

void Runner::send(kCmdOpCodeEnum code, float param) {
    if (wrFifo_.size() >= 1) { checkAlive_(); }
    if (!serial_.isOpen()) { return; }
    QMutexLocker locker(&mutex_);
    TRunnerCommand cmd;
    cmd.set(code, param);
    wrFifo_.enqueue(cmd);
    if (wrFifo_.size() == 1) { write_(cmd); }
}

void Runner::onSerial_readyRead(qint64 bytesAvailable) {
    if (!bytesAvailable) { return; }
    mutex_.lock();
    aliveTick_ = QDateTime::currentMSecsSinceEpoch();
    rdBuffer_ += serial_.readAll();
    mutex_.unlock();
    processData_();
}

void Runner::processData_() {
    if (rdBuffer_.isEmpty()) { return; }
    TRunnerCommand cmd = wrFifo_.head();
    if (!OpCode::isOk(rdBuffer_.data(), rdBuffer_.size())) {
        mutex_.lock();
        cmd = wrFifo_.dequeue();
        mutex_.unlock();
        cmd.response.clear();
        cmd.response.append(rdBuffer_.data(), 1);
        rdBuffer_.remove(0, 1);
        emit resultReady(cmd);
        if (!wrFifo_.isEmpty()) {
            cmd = wrFifo_.head();
            write_(cmd);
        }
        if (!rdBuffer_.isEmpty()) { processData_(); }
    } else if (rdBuffer_.size() >= cmd.opcode.result + 1) {
        mutex_.lock();
        cmd = wrFifo_.dequeue();
        mutex_.unlock();
        cmd.response.clear();
        cmd.response.append(rdBuffer_.data(), cmd.opcode.result + 1);
        rdBuffer_.remove(0, cmd.opcode.result + 1);
        emit resultReady(cmd);
        if (!wrFifo_.isEmpty()) {
            cmd = wrFifo_.head();
            write_(cmd);
        }
        if (!rdBuffer_.isEmpty()) { processData_(); }
    }
}

void Runner::write_(const TRunnerCommand &cmd) {
    serial_.purge();
    serial_.write(cmd.params.data(), cmd.params.size(), true);
    aliveTick_ = QDateTime::currentMSecsSinceEpoch();
}

void Runner::checkAlive_() {
    if (!running_) { return; }
    if (QDateTime::currentMSecsSinceEpoch() - aliveTick_ > kReadTimeOut) {
        running_ = false;
        if (serial_.isOpen()) { serial_.close(); }
        mutex_.lock();
        wrFifo_.clear();
        mutex_.unlock();
    }
}
