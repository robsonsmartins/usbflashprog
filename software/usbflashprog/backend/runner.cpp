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

#include "backend/runner.hpp"

// ---------------------------------------------------------------------------

constexpr int kReadTimeOut = 10000;

// ---------------------------------------------------------------------------

Serial::TSerialPortList Runner::list() const {
    return serial_.list();
}

bool Runner::open(const QString &path) {
    return serial_.open(path);
}

void Runner::close() {
    serial_.close();
}

bool Runner::isOpen() const {
    return serial_.isOpen();
}

QString Runner::getPath() const {
    return serial_.getPath();
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code,
                                   int *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code,
                                   bool *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code,
                                   float *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, int param,
                                   int *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, int param,
                                   bool *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, int param,
                                   float *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, bool param,
                                   int *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, bool param,
                                   bool *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, bool param,
                                   float *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, float param,
                                   int *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, float param,
                                   bool *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::send(kCmdOpCodeEnum code, float param,
                                   float *response) {
    if (!serial_.isOpen()) { return kCmdResponseNok; }
    TCmdOpCode opcode = OpCode::getOpCode(code);
    if (write_(opcode, param) != kCmdResponseOk) {
        return kCmdResponseNok;
    }
    return read_(opcode, response);
}

kCmdResponseValueEnum Runner::write_(const TCmdOpCode &code) {
    if (code.params != 0) { return kCmdResponseNok; }
    uint8_t wbuf = code.code;
    serial_.purge();
    serial_.write(&wbuf, 1, true);
    return kCmdResponseOk;
}

kCmdResponseValueEnum Runner::write_(const TCmdOpCode &code, int param) {
    if (code.params != 1) { return kCmdResponseNok; }
    size_t wlen = 1 + code.params;
    uint8_t *wbuf = new uint8_t[wlen];
    wbuf[0] = code.code;
    if (!OpCode::setValue(wbuf, wlen, param)) {
        delete[] wbuf;
        return kCmdResponseNok;
    }
    serial_.purge();
    serial_.write(wbuf, wlen, true);
    delete[] wbuf;
    return kCmdResponseOk;
}

kCmdResponseValueEnum Runner::write_(const TCmdOpCode &code, bool param) {
    if (code.params != 1) { return kCmdResponseNok; }
    size_t wlen = 1 + code.params;
    uint8_t *wbuf = new uint8_t[wlen];
    wbuf[0] = code.code;
    if (!OpCode::setValue(wbuf, wlen, param)) {
        delete[] wbuf;
        return kCmdResponseNok;
    }
    serial_.purge();
    serial_.write(wbuf, wlen, true);
    delete[] wbuf;
    return kCmdResponseOk;
}

kCmdResponseValueEnum Runner::write_(const TCmdOpCode &code, float param) {
    if (code.params != 2) { return kCmdResponseNok; }
    size_t wlen = 1 + code.params;
    uint8_t *wbuf = new uint8_t[wlen];
    wbuf[0] = code.code;
    if (!OpCode::setValue(wbuf, wlen, param)) {
        delete[] wbuf;
        return kCmdResponseNok;
    }
    serial_.purge();
    serial_.write(wbuf, wlen, true);
    delete[] wbuf;
    return kCmdResponseOk;
}

kCmdResponseValueEnum Runner::read_(const TCmdOpCode &code, int *response) {
    size_t rlen = 1 + code.result;
    uint8_t *rbuf = new uint8_t[rlen];
    if (serial_.read(rbuf, 1, kReadTimeOut) == 0 || !OpCode::isOk(rbuf, 1)) {
        delete[] rbuf;
        return kCmdResponseNok;
    }
    if (code.result) {
        if (serial_.read(rbuf + 1, code.result, kReadTimeOut) < code.result) {
            delete[] rbuf;
            return kCmdResponseNok;
        }
        if (response) {
            *response = OpCode::getValueAsByte(rbuf, rlen);
        }
    }
    delete[] rbuf;
    return kCmdResponseOk;
}

kCmdResponseValueEnum Runner::read_(const TCmdOpCode &code, bool *response) {
    size_t rlen = 1 + code.result;
    uint8_t *rbuf = new uint8_t[rlen];
    if (serial_.read(rbuf, 1, kReadTimeOut) == 0 || !OpCode::isOk(rbuf, 1)) {
        delete[] rbuf;
        return kCmdResponseNok;
    }
    if (code.result) {
        if (serial_.read(rbuf + 1, code.result, kReadTimeOut) < code.result) {
            delete[] rbuf;
            return kCmdResponseNok;
        }
        if (response) {
            *response = OpCode::getValueAsBool(rbuf, rlen);
        }
    }
    delete[] rbuf;
    return kCmdResponseOk;
}

kCmdResponseValueEnum Runner::read_(const TCmdOpCode &code, float *response) {
    size_t rlen = 1 + code.result;
    uint8_t *rbuf = new uint8_t[rlen];
    if (serial_.read(rbuf, 1, kReadTimeOut) == 0 || !OpCode::isOk(rbuf, 1)) {
        delete[] rbuf;
        return kCmdResponseNok;
    }
    if (code.result) {
        if (serial_.read(rbuf + 1, code.result, kReadTimeOut) < code.result) {
            delete[] rbuf;
            return kCmdResponseNok;
        }
        if (response) {
            *response = OpCode::getValueAsFloat(rbuf, rlen);
        }
    }
    delete[] rbuf;
    return kCmdResponseOk;
}
