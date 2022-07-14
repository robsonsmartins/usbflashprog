// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/backend/opcodes_test.cpp
 * @brief Implementation of Unit Test for OpCode Handling Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstring>
#include "opcodes_test.hpp"
#include "backend/opcodes.hpp"

// ---------------------------------------------------------------------------

void OpCodeTest::isOk() {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    QCOMPARE(OpCode::isOk(buf, sizeof(buf)), false);
    buf[0] = kCmdResponseNok;
    QCOMPARE(OpCode::isOk(buf, sizeof(buf)), false);
    buf[0] = kCmdResponseOk;
    QCOMPARE(OpCode::isOk(buf, sizeof(buf)), true);
}

void OpCodeTest::getOpCode() {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));
    TCmdOpCode op;

    op = OpCode::getOpCode(kCmdNop);
    QCOMPARE(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = kCmdVddCtrl;
    op = OpCode::getOpCode(kCmdVddCtrl);
    QCOMPARE(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = kCmdVppCtrl;
    op = OpCode::getOpCode(kCmdVppCtrl);
    QCOMPARE(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = 0xFF;
    op = OpCode::getOpCode(kCmdNop);
    QCOMPARE(OpCode::getOpCode(buf, sizeof(buf)), op);
}

void OpCodeTest::getValue() {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    QCOMPARE(OpCode::getValueAsByte(buf, sizeof(buf)), 0x00);
    QCOMPARE(OpCode::getValueAsBool(buf, sizeof(buf)), false);
    QCOMPARE(OpCode::getValueAsFloat(buf, sizeof(buf)), 0.0f);
    buf[1] = 0x33;  // 51
    buf[2] = 0x21;  // 33
    QCOMPARE(OpCode::getValueAsByte(buf, sizeof(buf)), 0x33);
    QCOMPARE(OpCode::getValueAsBool(buf, sizeof(buf)), true);
    QCOMPARE(OpCode::getValueAsFloat(buf, sizeof(buf)), 51.33f);
    buf[0] = kCmdResponseOk;
    QCOMPARE(OpCode::getValueAsByte(buf, sizeof(buf)), 0x33);
    QCOMPARE(OpCode::getValueAsBool(buf, sizeof(buf)), true);
    QCOMPARE(OpCode::getValueAsFloat(buf, sizeof(buf)), 51.33f);
    buf[0] = kCmdResponseNok;
    QCOMPARE(OpCode::getValueAsByte(buf, sizeof(buf)), 0x33);
    QCOMPARE(OpCode::getValueAsBool(buf, sizeof(buf)), true);
    QCOMPARE(OpCode::getValueAsFloat(buf, sizeof(buf)), 51.33f);
}

void OpCodeTest::setValue() {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    QCOMPARE(OpCode::setValue(nullptr, sizeof(buf), 0x00), false);
    QCOMPARE(OpCode::setValue(buf, 0, 0x00), false);

    QCOMPARE(buf[1], 0x00);
    QCOMPARE(OpCode::setValue(buf, sizeof(buf), 0x33), true);
    QCOMPARE(buf[1], 0x33);
    QCOMPARE(OpCode::setValue(buf, sizeof(buf), false), true);
    QCOMPARE(buf[1], kCmdParamOff);
    QCOMPARE(OpCode::setValue(buf, sizeof(buf), true), true);
    QCOMPARE(buf[1], kCmdParamOn);
    QCOMPARE(OpCode::setValue(buf, sizeof(buf), 51.33f), true);
    QCOMPARE(buf[1], 0x33);
    QCOMPARE(buf[2], 0x21);
}
