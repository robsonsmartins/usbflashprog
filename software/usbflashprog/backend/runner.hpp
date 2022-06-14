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

#include <QString>

#include "backend/opcodes.hpp"
#include "backend/serial.hpp"

// ---------------------------------------------------------------------------

class Runner {
 public:
    Serial::TSerialPortList list() const;
    bool open(const QString &path);
    void close();
    bool isOpen() const;
    QString getPath() const;
    kCmdResponseValueEnum send(kCmdOpCodeEnum code,
                               int *response = nullptr);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code,
                               bool *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code,
                               float *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, int param,
                               int *response = nullptr);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, int param,
                               bool *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, int param,
                               float *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, bool param,
                               int *response = nullptr);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, bool param,
                               bool *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, bool param,
                               float *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, float param,
                               int *response = nullptr);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, float param,
                               bool *response);
    kCmdResponseValueEnum send(kCmdOpCodeEnum code, float param,
                               float *response);

 private:
    Serial serial_;
    kCmdResponseValueEnum write_(const TCmdOpCode &code);
    kCmdResponseValueEnum write_(const TCmdOpCode &code, int param);
    kCmdResponseValueEnum write_(const TCmdOpCode &code, bool param);
    kCmdResponseValueEnum write_(const TCmdOpCode &code, float param);
    kCmdResponseValueEnum read_(const TCmdOpCode &code, int *response);
    kCmdResponseValueEnum read_(const TCmdOpCode &code, bool *response);
    kCmdResponseValueEnum read_(const TCmdOpCode &code, float *response);
};

#endif  // BACKEND_RUNNER_HPP_
