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
 * @file backend/runner.cpp
 * @brief Implementation of the Runner Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QDateTime>
#include <QCoreApplication>

#include <chrono>
#include <thread>

#include "backend/runner.hpp"
#include "config.hpp"

// ---------------------------------------------------------------------------

#ifdef TEST_BUILD
bool QSerialPort::connected = false;
#endif

// ---------------------------------------------------------------------------

/* @brief Timeout (in milliseconds) to consider timeout. */
constexpr int kReadTimeOut = 1000;

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

int TRunnerCommand::responseAsWord() const {
    return OpCode::getValueAsWord(response.data(), response.size());
}

int TRunnerCommand::responseAsDWord() const {
    return OpCode::getValueAsDWord(response.data(), response.size());
}

bool TRunnerCommand::responseAsBool() const {
    return OpCode::getValueAsBool(response.data(), response.size());
}

void TRunnerCommand::set(kCmdOpCodeEnum code) {
    opcode = OpCode::getOpCode(code);
    params.resize(1);
    params[0] = opcode.code;
}

void TRunnerCommand::setFloat(kCmdOpCodeEnum code, float param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setFloat(params.data(), params.size(), param);
}

void TRunnerCommand::setByte(kCmdOpCodeEnum code, uint8_t param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setByte(params.data(), params.size(), param);
}

void TRunnerCommand::setWord(kCmdOpCodeEnum code, uint16_t param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setWord(params.data(), params.size(), param);
}

void TRunnerCommand::setDWord(kCmdOpCodeEnum code, uint32_t param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setDWord(params.data(), params.size(), param);
}

void TRunnerCommand::setBool(kCmdOpCodeEnum code, bool param) {
    opcode = OpCode::getOpCode(code);
    params.resize(opcode.params + 1);
    params[0] = opcode.code;
    OpCode::setBool(params.data(), params.size(), param);
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
        serial_(this), timeout_(200), running_(false) {
}

Runner::~Runner() {
    close();
}

TSerialPortList Runner::list() const {
    TSerialPortList result;
    for (const auto item : QSerialPortInfo::availablePorts()) {
        if (item.vendorIdentifier() == kUsbVendorId &&
                item.productIdentifier() == kUsbProductId) {
#ifdef Q_OS_MACOS
            if (item.portName().startsWith("cu.usbmodem")) { continue; }
#endif  // Q_OS_MACOS
#ifdef Q_OS_FREEBSD
            if (item.portName().startsWith("tty")) { continue; }
#endif  // Q_OS_FREEBSD
            result.push_back(item);
        }
    }
    return result;
}

bool Runner::open(const QString &path) {
    if (running_) { close(); }
    if (path.isNull() || path.isEmpty()) { return false; }
    serial_.setPortName(path);
    bool result = serial_.open(QIODevice::ReadWrite);
    if (result) {
        running_ = true;
        // need under Windows
        // https://community.platformio.org/t/
        //   serial-communication-micro-usb-on-pi-pico-c/27512/5
        serial_.setDataTerminalReady(true);
    }
    return result;
}

void Runner::close() {
    serial_.close();
    running_ = false;
}

bool Runner::isOpen() const {
    return running_;
}

QString Runner::getPath() const {
    return serial_.portName();
}

uint32_t Runner::getTimeOut() const {
    return timeout_;
}

void Runner::setTimeOut(uint32_t value) {
    timeout_ = value;
}

bool Runner::nop() {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.set(kCmdNop);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vddCtrl(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVddCtrl, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vddSet(float value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVddSetV, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

float Runner::vddGet() {
    if (!serial_.isOpen()) { return -1.0f; }
    TRunnerCommand cmd;
    cmd.set(kCmdVddGetV);
    if (!write_(cmd.params)) { return -1.0f; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return -1.0f; }
    if (!cmd.responseIsOk()) { return -1.0f; }
    return cmd.responseAsFloat();
}

float Runner::vddGetDuty() {
    if (!serial_.isOpen()) { return -1.0f; }
    TRunnerCommand cmd;
    cmd.set(kCmdVddGetDuty);
    if (!write_(cmd.params)) { return -1.0f; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return -1.0f; }
    if (!cmd.responseIsOk()) { return -1.0f; }
    return cmd.responseAsFloat();
}

bool Runner::vddInitCal() {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.set(kCmdVddInitCal);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vddSaveCal(float value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVddSaveCal, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

float Runner::vddGetCal() {
    if (!serial_.isOpen()) { return -1.0f; }
    TRunnerCommand cmd;
    cmd.set(kCmdVddGetCal);
    if (!write_(cmd.params)) { return -1.0f; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return -1.0f; }
    if (!cmd.responseIsOk()) { return -1.0f; }
    return cmd.responseAsFloat();
}

bool Runner::vppCtrl(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppCtrl, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppSet(float value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVppSetV, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

float Runner::vppGet() {
    if (!serial_.isOpen()) { return -1.0f; }
    TRunnerCommand cmd;
    cmd.set(kCmdVppGetV);
    if (!write_(cmd.params)) { return -1.0f; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return -1.0f; }
    if (!cmd.responseIsOk()) { return -1.0f; }
    return cmd.responseAsFloat();
}

float Runner::vppGetDuty() {
    if (!serial_.isOpen()) { return -1.0f; }
    TRunnerCommand cmd;
    cmd.set(kCmdVppGetDuty);
    if (!write_(cmd.params)) { return -1.0f; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return -1.0f; }
    if (!cmd.responseIsOk()) { return -1.0f; }
    return cmd.responseAsFloat();
}

bool Runner::vppInitCal() {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.set(kCmdVppInitCal);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppSaveCal(float value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVppSaveCal, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

float Runner::vppGetCal() {
    if (!serial_.isOpen()) { return -1.0f; }
    TRunnerCommand cmd;
    cmd.set(kCmdVppGetCal);
    if (!write_(cmd.params)) { return -1.0f; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return -1.0f; }
    if (!cmd.responseIsOk()) { return -1.0f; }
    return cmd.responseAsFloat();
}

bool Runner::vddOnVpp(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVddOnVpp, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppOnA9(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnA9, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppOnA18(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnA18, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppOnCE(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnCE, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppOnOE(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnOE, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::vppOnWE(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnWE, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::setCE(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdBusCE, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::setOE(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdBusOE, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::setWE(bool on) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setBool(kCmdBusWE, on);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::addrClr() {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.set(kCmdBusAddrClr);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::addrInc() {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.set(kCmdBusAddrInc);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::addrSet(uint32_t value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setDWord(kCmdBusAddrSet, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::addrSetB(uint8_t value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setByte(kCmdBusAddrSetB, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::addrSetW(uint16_t value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setWord(kCmdBusAddrSetW, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::dataClr() {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.set(kCmdBusDataClr);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::dataSet(uint8_t value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setByte(kCmdBusDataSetB, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

bool Runner::dataSetW(uint16_t value) {
    if (!serial_.isOpen()) { return false; }
    TRunnerCommand cmd;
    cmd.setWord(kCmdBusDataSet, value);
    if (!write_(cmd.params)) { return false; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return false; }
    return cmd.responseIsOk();
}

uint8_t Runner::dataGet() {
    if (!serial_.isOpen()) { return 0xff; }
    TRunnerCommand cmd;
    cmd.set(kCmdBusDataGetB);
    if (!write_(cmd.params)) { return 0xff; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return 0xff; }
    if (!cmd.responseIsOk()) { return 0xff; }
    return cmd.responseAsByte();
}

uint16_t Runner::dataGetW() {
    if (!serial_.isOpen()) { return 0xffff; }
    TRunnerCommand cmd;
    cmd.set(kCmdBusDataGet);
    if (!write_(cmd.params)) { return 0xffff; }
    if (!read_(&cmd.response, cmd.opcode.result + 1)) { return 0xffff; }
    if (!cmd.responseIsOk()) { return 0xffff; }
    return cmd.responseAsWord();
}

void Runner::usDelay(uint64_t value) {
    if (!value) { return; }
    if (value >= 10000) {
        msDelay(value / 1000);
        return;
    }
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    int64_t elapsed = 0;
    do {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        end = std::chrono::steady_clock::now();
        elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    } while (elapsed < value);
}

void Runner::msDelay(uint32_t value) {
    if (!value) { return; }
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    int64_t elapsed = 0;
    do {
        QCoreApplication::processEvents();
        end = std::chrono::steady_clock::now();
        elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    } while (elapsed < value);
}

bool Runner::write_(const QByteArray &data) {
    if (data.isEmpty()) { return true; }
    serial_.clear();
    serial_.write(data);
    serial_.flush();
    if (!serial_.waitForBytesWritten(timeout_)) {
        checkAlive_();
        return false;
    }
    return true;
}

bool Runner::read_(QByteArray *data, uint32_t size) {
    if (data == nullptr || !size) { return true; }
    data->clear();
    while (data->size() < size) {
        if (!serial_.waitForReadyRead(timeout_)) {
            checkAlive_();
            return false;
        }
        data->append(serial_.readAll());
        aliveTick_ = QDateTime::currentMSecsSinceEpoch();
    }
    if (data->size() > size) data->resize(size);
    return data->size() == size;
}

void Runner::checkAlive_() {
    if (!running_) { return; }
    if (QDateTime::currentMSecsSinceEpoch() - aliveTick_ > kReadTimeOut) {
        running_ = false;
        if (serial_.isOpen()) { serial_.close(); }
    }
}
