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
 * @file test/modules/opcodes_test.cpp
 * @brief Implementation of Unit Test for OpCode Handling Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstring>
#include "opcodes_test.hpp"
#include "modules/opcodes.hpp"

// ---------------------------------------------------------------------------

TEST_F(OpCodeTest, is_ok) {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    EXPECT_EQ(OpCode::isOk(buf, sizeof(buf)), false);
    buf[0] = kCmdResponseNok;
    EXPECT_EQ(OpCode::isOk(buf, sizeof(buf)), false);
    buf[0] = kCmdResponseOk;
    EXPECT_EQ(OpCode::isOk(buf, sizeof(buf)), true);
}

TEST_F(OpCodeTest, get_opcode) {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));
    TCmdOpCode op;

    op = kCmdOpCodes.at(kCmdNop);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = kCmdVddCtrl;
    op = kCmdOpCodes.at(kCmdVddCtrl);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = kCmdVppCtrl;
    op = kCmdOpCodes.at(kCmdVppCtrl);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = 0xFF;
    op = kCmdOpCodes.at(kCmdNop);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
}

TEST_F(OpCodeTest, get_value) {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    EXPECT_EQ(OpCode::getValueAsByte(buf, sizeof(buf)), 0x00);
    EXPECT_EQ(OpCode::getValueAsBool(buf, sizeof(buf)), false);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, sizeof(buf)), 0.0f);
    buf[1] = 0x33;  // 51
    buf[2] = 0x21;  // 33
    EXPECT_EQ(OpCode::getValueAsByte(buf, sizeof(buf)), 0x00);
    EXPECT_EQ(OpCode::getValueAsBool(buf, sizeof(buf)), false);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, sizeof(buf)), 0.0f);
    buf[0] = kCmdResponseOk;
    EXPECT_EQ(OpCode::getValueAsByte(buf, sizeof(buf)), 0x33);
    EXPECT_EQ(OpCode::getValueAsBool(buf, sizeof(buf)), true);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, sizeof(buf)), 51.33f);
    buf[0] = kCmdResponseNok;
    EXPECT_EQ(OpCode::getValueAsByte(buf, sizeof(buf)), 0x00);
    EXPECT_EQ(OpCode::getValueAsBool(buf, sizeof(buf)), false);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, sizeof(buf)), 0.0f);
}

TEST_F(OpCodeTest, set_value) {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    EXPECT_EQ(OpCode::setValue(nullptr, sizeof(buf), 0x00), false);
    EXPECT_EQ(OpCode::setValue(buf, 0, 0x00), false);

    EXPECT_EQ(buf[1], 0x00);
    EXPECT_EQ(OpCode::setValue(buf, sizeof(buf), 0x33), true);
    EXPECT_EQ(buf[1], 0x33);
    EXPECT_EQ(OpCode::setValue(buf, sizeof(buf), false), true);
    EXPECT_EQ(buf[1], kCmdParamOff);
    EXPECT_EQ(OpCode::setValue(buf, sizeof(buf), true), true);
    EXPECT_EQ(buf[1], kCmdParamOn);
    EXPECT_EQ(OpCode::setValue(buf, sizeof(buf), 51.33f), true);
    EXPECT_EQ(buf[1], 0x33);
    EXPECT_EQ(buf[2], 0x21);
}
