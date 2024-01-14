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
#include <QApplication>
#include <QLoggingCategory>

#include <chrono>
#include <thread>

#include "backend/runner.hpp"
#include "config.hpp"

#ifdef Q_OS_WINDOWS
#include <windows.h>
#endif

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(backendRunner, "backend.runner")

#define DEBUG qCDebug(backendRunner)
#define INFO qCInfo(backendRunner)
#define WARNING qCWarning(backendRunner)
#define CRITICAL qCCritical(backendRunner)
#define FATAL qCFatal(backendRunner)

// ---------------------------------------------------------------------------

#ifdef TEST_BUILD
bool QSerialPort::connected = false;
#endif

// ---------------------------------------------------------------------------

/* @brief Timeout (in milliseconds) to consider disconnect. */
constexpr int kDisconnectTimeOut = 5000;
/* @brief Timeout (in milliseconds) to read byte. */
constexpr int kReadTimeOut = 200;

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
    this->opcode = src.opcode;
    this->params = src.params;
    this->response = src.response;
    return *this;
}

bool operator==(const TRunnerCommand& a, const TRunnerCommand& b) {
    return (a.opcode == b.opcode && a.params == b.params &&
            a.response == b.response);
}

// ---------------------------------------------------------------------------

Runner::Runner(QObject* parent)
    : QObject(parent),
      serial_(this),
      timeout_(kReadTimeOut),
      running_(false),
      error_(false),
      address_(0) {}

Runner::~Runner() {
    close();
}

TSerialPortList Runner::list() const {
    TSerialPortList result;
    for (const auto item : QSerialPortInfo::availablePorts()) {
        if (item.vendorIdentifier() == kUsbVendorId &&
            item.productIdentifier() == kUsbProductId) {
#ifdef Q_OS_MACOS
            if (item.portName().startsWith("cu.usbmodem")) {
                continue;
            }
#endif  // Q_OS_MACOS
#ifdef Q_OS_FREEBSD
            if (item.portName().startsWith("tty")) {
                continue;
            }
#endif  // Q_OS_FREEBSD
            result.push_back(item);
        }
    }
    return result;
}

bool Runner::open(const QString& path) {
    if (running_) close();
    if (path.isNull() || path.isEmpty()) return false;
    DEBUG << "Opening serial port:" << path << "...";
    serial_.setPortName(path);
    bool result = serial_.open(QIODevice::ReadWrite);
    if (result) {
        running_ = true;
        // need under Windows
        // https://community.platformio.org/t/
        //   serial-communication-micro-usb-on-pi-pico-c/27512/5
        serial_.setDataTerminalReady(true);
        DEBUG << "Open serial port OK";
        DEBUG << "Setting timeout:" << QString("%1").arg(timeout_);
    } else {
        WARNING << "Error opening serial port";
    }
    error_ = !result;
    return result;
}

void Runner::close() {
    if (serial_.isOpen()) {
        DEBUG << "Closing serial port...";
    }
    serial_.close();
    running_ = false;
    error_ = false;
}

bool Runner::isOpen() const {
    return running_;
}

bool Runner::hasError() const {
    return error_;
}

QString Runner::getPath() const {
    return serial_.portName();
}

uint32_t Runner::getTimeOut() const {
    return timeout_;
}

void Runner::setTimeOut(uint32_t value) {
    if (timeout_ == value) return;
    timeout_ = value;
    DEBUG << "Setting timeout:" << QString("%1").arg(value);
}

bool Runner::nop() {
    TRunnerCommand cmd;
    cmd.set(kCmdNop);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vddCtrl(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVddCtrl, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vddSet(float value) {
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVddSetV, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

float Runner::vddGet() {
    TRunnerCommand cmd;
    cmd.set(kCmdVddGetV);
    if (!sendCommand_(cmd)) return -1.0f;
    return cmd.responseAsFloat();
}

float Runner::vddGetDuty() {
    TRunnerCommand cmd;
    cmd.set(kCmdVddGetDuty);
    if (!sendCommand_(cmd)) return -1.0f;
    return cmd.responseAsFloat();
}

bool Runner::vddInitCal() {
    TRunnerCommand cmd;
    cmd.set(kCmdVddInitCal);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vddSaveCal(float value) {
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVddSaveCal, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

float Runner::vddGetCal() {
    TRunnerCommand cmd;
    cmd.set(kCmdVddGetCal);
    if (!sendCommand_(cmd)) return -1.0f;
    return cmd.responseAsFloat();
}

bool Runner::vppCtrl(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppCtrl, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppSet(float value) {
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVppSetV, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

float Runner::vppGet() {
    TRunnerCommand cmd;
    cmd.set(kCmdVppGetV);
    if (!sendCommand_(cmd)) return -1.0f;
    return cmd.responseAsFloat();
}

float Runner::vppGetDuty() {
    TRunnerCommand cmd;
    cmd.set(kCmdVppGetDuty);
    if (!sendCommand_(cmd)) return -1.0f;
    return cmd.responseAsFloat();
}

bool Runner::vppInitCal() {
    TRunnerCommand cmd;
    cmd.set(kCmdVppInitCal);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppSaveCal(float value) {
    TRunnerCommand cmd;
    cmd.setFloat(kCmdVppSaveCal, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

float Runner::vppGetCal() {
    TRunnerCommand cmd;
    cmd.set(kCmdVppGetCal);
    if (!sendCommand_(cmd)) return -1.0f;
    return cmd.responseAsFloat();
}

bool Runner::vddOnVpp(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVddOnVpp, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppOnA9(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnA9, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppOnA18(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnA18, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppOnCE(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnCE, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppOnOE(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnOE, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::vppOnWE(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdVppOnWE, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::setCE(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdBusCE, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::setOE(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdBusOE, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::setWE(bool on) {
    TRunnerCommand cmd;
    cmd.setBool(kCmdBusWE, on);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::addrClr() {
    TRunnerCommand cmd;
    cmd.set(kCmdBusAddrClr);
    if (!sendCommand_(cmd)) return false;
    address_ = 0;
    return true;
}

bool Runner::addrInc() {
    TRunnerCommand cmd;
    cmd.set(kCmdBusAddrInc);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in addrInc(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet(). New address:"
              << QString("0x%1").arg(address_ + 1, 6, 16, QChar('0'));
        // error, use addrSet
        return addrSet(address_ + 1);
    }
    address_++;
    return true;
}

bool Runner::addrSet(uint32_t value) {
    if (!value) return addrClr();
    TRunnerCommand cmd;
    cmd.setDWord(kCmdBusAddrSet, value);
    if (!sendCommand_(cmd)) return false;
    address_ = value;
    return true;
}

bool Runner::addrSetB(uint8_t value) {
    if (!value) return addrClr();
    TRunnerCommand cmd;
    cmd.setByte(kCmdBusAddrSetB, value);
    if (!sendCommand_(cmd)) return false;
    address_ = value;
    return true;
}

bool Runner::addrSetW(uint16_t value) {
    if (!value) return addrClr();
    TRunnerCommand cmd;
    cmd.setWord(kCmdBusAddrSetW, value);
    if (!sendCommand_(cmd)) return false;
    address_ = value;
    return true;
}

uint32_t Runner::addrGet() const {
    return address_;
}

bool Runner::dataClr() {
    TRunnerCommand cmd;
    cmd.set(kCmdBusDataClr);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::dataSet(uint8_t value) {
    if (!value) return dataClr();
    TRunnerCommand cmd;
    cmd.setByte(kCmdBusDataSetB, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::dataSetW(uint16_t value) {
    if (!value) return dataClr();
    TRunnerCommand cmd;
    cmd.setWord(kCmdBusDataSet, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

uint8_t Runner::dataGet() {
    TRunnerCommand cmd;
    cmd.set(kCmdBusDataGetB);
    if (!sendCommand_(cmd)) return 0xFF;
    return cmd.responseAsByte();
}

uint16_t Runner::dataGetW() {
    TRunnerCommand cmd;
    cmd.set(kCmdBusDataGet);
    if (!sendCommand_(cmd)) return 0xFFFF;
    return cmd.responseAsWord();
}

bool Runner::deviceSetFlags(const TDeviceSettings& value) {
    TRunnerCommand cmd;
    uint8_t flags = 0;
    // clang-format off
    if (value.skipFF     ) flags |= 0x01;
    if (value.progWithVpp) flags |= 0x02;
    if (value.vppOePin   ) flags |= 0x04;
    if (value.pgmCePin   ) flags |= 0x08;
    if (value.pgmPositive) flags |= 0x10;
    // clang-format on
    cmd.setByte(kCmdDeviceSetFlags, flags);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::deviceSetTwp(uint32_t value) {
    TRunnerCommand cmd;
    cmd.setDWord(kCmdDeviceSetTwp, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

bool Runner::deviceSetTwc(uint32_t value) {
    TRunnerCommand cmd;
    cmd.setDWord(kCmdDeviceSetTwc, value);
    if (!sendCommand_(cmd)) return false;
    return true;
}

uint8_t Runner::deviceRead() {
    TRunnerCommand cmd;
    cmd.set(kCmdDeviceReadB);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in deviceRead(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet()";
        // error
        // use addrSet
        if (!addrSet(address_)) return 0xFF;
        // call deviceRead already
        if (!sendCommand_(cmd, 0)) return 0xFF;
    }
    address_++;
    return cmd.responseAsByte();
}

uint16_t Runner::deviceReadW() {
    TRunnerCommand cmd;
    cmd.set(kCmdDeviceRead);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in deviceReadW(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet()";
        // error
        // use addrSet
        if (!addrSet(address_)) return 0xFFFF;
        // call deviceReadW already
        if (!sendCommand_(cmd, 0)) return 0xFFFF;
    }
    address_++;
    return cmd.responseAsWord();
}

bool Runner::deviceWrite(uint8_t value) {
    TRunnerCommand cmd;
    cmd.setByte(kCmdDeviceWriteB, value);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in deviceWrite(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet()";
        // error
        // use addrSet
        if (!addrSet(address_)) return false;
        // call deviceWrite already
        if (!sendCommand_(cmd, 0)) return false;
    }
    address_++;
    return true;
}

bool Runner::deviceWriteW(uint16_t value) {
    TRunnerCommand cmd;
    cmd.setWord(kCmdDeviceWrite, value);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in deviceWriteW(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet()";
        // error
        // use addrSet
        if (!addrSet(address_)) return false;
        // call deviceWriteW already
        if (!sendCommand_(cmd, 0)) return false;
    }
    address_++;
    return true;
}

bool Runner::deviceVerify(uint8_t value) {
    TRunnerCommand cmd;
    cmd.setByte(kCmdDeviceVerifyB, value);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in deviceVerify(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet()";
        // error
        // use addrSet
        if (!addrSet(address_)) return false;
        // call deviceVerify already
        if (!sendCommand_(cmd, 0)) return false;
    }
    address_++;
    return true;
}

bool Runner::deviceVerifyW(uint16_t value) {
    TRunnerCommand cmd;
    cmd.setWord(kCmdDeviceVerify, value);
    // no retry
    if (!sendCommand_(cmd, 0)) {
        DEBUG << "Error in deviceVerifyW(). Last address:"
              << QString("0x%1").arg(address_, 6, 16, QChar('0'))
              << "Trying use addrSet()";
        // error
        // use addrSet
        if (!addrSet(address_)) return false;
        // call deviceVerifyW already
        if (!sendCommand_(cmd, 0)) return false;
    }
    address_++;
    return true;
}

void Runner::usDelay(uint64_t value) {
    if (!value) return;
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
            std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                .count();
    } while (elapsed < value);
}

void Runner::msDelay(uint32_t value) {
    if (!value) return;
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    int64_t elapsed = 0;
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        end = std::chrono::steady_clock::now();
        elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
        if (value > 50) processEvents();
    } while (elapsed < value);
}

void Runner::processEvents() {
    QApplication::processEvents();
#ifdef Q_OS_WINDOWS
    Sleep(1);
#endif
}

bool Runner::sendCommand_(TRunnerCommand& cmd, int retry) {
    if (!serial_.isOpen()) {
        WARNING << "Serial port not open. Error running command"
                << cmd.opcode.descr.c_str();
        error_ = true;
        return false;
    }
    DEBUG << "Running" << cmd.opcode.descr.c_str()
          << "[ 0x" + QString(cmd.params.toHex()) << "]"
          << "Current Address:"
          << QString("0x%1").arg(address_, 6, 16, QChar('0'));
    bool success = false;
    for (int i = 0; i < (retry + 1); i++) {
        if (!write_(cmd.params) ||
            !read_(&cmd.response, cmd.opcode.result + 1)) {
            DEBUG << "Retrying."
                  << "Command" << cmd.opcode.descr.c_str();
            continue;
        }
        error_ = false;
        success = true;
        break;
    }
    if (!success) {
        if (cmd.opcode.code != kCmdBusAddrInc) {
            WARNING << "Error writing to or reading from serial port."
                    << "Command" << cmd.opcode.descr.c_str();
        }
        error_ = true;
        return false;
    }
    if (!cmd.responseIsOk()) {
        WARNING << "Response NOK."
                << "Command" << cmd.opcode.descr.c_str();
        error_ = true;
        return false;
    } else {
        DEBUG << "Response"
              << "[ 0x" + QString(cmd.response.toHex()) << "]";
    }
    return true;
}

bool Runner::write_(const QByteArray& data) {
    if (data.isEmpty()) return true;
    serial_.clear();
    return serial_.write(data) == data.size();
}

bool Runner::read_(QByteArray* data, uint32_t size) {
    if (data == nullptr || !size) return true;
    data->clear();
    while (data->size() < size) {
        if (!serial_.waitForReadyRead(timeout_)) {
            DEBUG << "Error reading serial port: timeout";
            checkAlive_();
            return false;
        }
        data->append(serial_.readAll());
        aliveTick_ = QDateTime::currentMSecsSinceEpoch();
    }
    if (data->size() > size) data->resize(size);
    if (data->size() != size) {
        DEBUG << "Error reading serial port: sizes are different";
        return false;
    }
    return true;
}

void Runner::checkAlive_() {
    if (!running_) return;
    if (QDateTime::currentMSecsSinceEpoch() - aliveTick_ > kDisconnectTimeOut) {
        WARNING << "Serial port: disconnected by timeout";
        running_ = false;
        if (serial_.isOpen()) serial_.close();
    }
}
