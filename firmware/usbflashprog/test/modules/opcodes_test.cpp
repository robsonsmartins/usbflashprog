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

    op = OpCode::getOpCode(kCmdNop);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = kCmdVddCtrl;
    op = OpCode::getOpCode(kCmdVddCtrl);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = kCmdVppCtrl;
    op = OpCode::getOpCode(kCmdVppCtrl);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
    buf[0] = 0xFF;
    op = OpCode::getOpCode(kCmdNop);
    EXPECT_EQ(OpCode::getOpCode(buf, sizeof(buf)), op);
}

TEST_F(OpCodeTest, get_value) {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    EXPECT_EQ(OpCode::getValueAsByte(buf, 2), 0x00);
    EXPECT_EQ(OpCode::getValueAsBool(buf, 2), false);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, 3), 0.0f);
    EXPECT_EQ(OpCode::getValueAsWord(buf, 3), 0x0000);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 5), 0x00000000);
    buf[1] = 0x33;  // 51
    buf[2] = 0x21;  // 33
    buf[3] = 0x58;
    buf[4] = 0x84;
    EXPECT_EQ(OpCode::getValueAsByte(buf, 2), 0x33);
    EXPECT_EQ(OpCode::getValueAsBool(buf, 2), true);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, 3), 51.33f);
    EXPECT_EQ(OpCode::getValueAsWord(buf, 3), 0x3321);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 4), 0x332158);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 5), 0x33215884);
    buf[0] = kCmdResponseOk;
    EXPECT_EQ(OpCode::getValueAsByte(buf, 2), 0x33);
    EXPECT_EQ(OpCode::getValueAsBool(buf, 2), true);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, 3), 51.33f);
    EXPECT_EQ(OpCode::getValueAsWord(buf, 3), 0x3321);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 4), 0x332158);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 5), 0x33215884);
    buf[0] = kCmdResponseNok;
    EXPECT_EQ(OpCode::getValueAsByte(buf, 2), 0x33);
    EXPECT_EQ(OpCode::getValueAsBool(buf, 2), true);
    EXPECT_EQ(OpCode::getValueAsFloat(buf, 3), 51.33f);
    EXPECT_EQ(OpCode::getValueAsWord(buf, 3), 0x3321);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 4), 0x332158);
    EXPECT_EQ(OpCode::getValueAsDWord(buf, 5), 0x33215884);
}

TEST_F(OpCodeTest, set_value) {
    uint8_t buf[32];
    memset(buf, 0, sizeof(buf));

    EXPECT_EQ(OpCode::setByte(nullptr, sizeof(buf), 0x00), false);
    EXPECT_EQ(OpCode::setByte(buf, 0, 0x00), false);

    EXPECT_EQ(buf[1], 0x00);
    EXPECT_EQ(OpCode::setByte(buf, 2, 0x33), true);
    EXPECT_EQ(buf[1], 0x33);
    EXPECT_EQ(OpCode::setBool(buf, 2, false), true);
    EXPECT_EQ(buf[1], kCmdParamOff);
    EXPECT_EQ(OpCode::setBool(buf, 2, true), true);
    EXPECT_EQ(buf[1], kCmdParamOn);
    EXPECT_EQ(OpCode::setFloat(buf, 3, 51.33f), true);
    EXPECT_EQ(buf[1], 0x33);
    EXPECT_EQ(buf[2], 0x21);
    EXPECT_EQ(OpCode::setWord(buf, 3, 0xA1B2), true);
    EXPECT_EQ(buf[1], 0xA1);
    EXPECT_EQ(buf[2], 0xB2);
    EXPECT_EQ(OpCode::setDWord(buf, 4, 0xCFA1B2), true);
    EXPECT_EQ(buf[1], 0xCF);
    EXPECT_EQ(buf[2], 0xA1);
    EXPECT_EQ(buf[3], 0xB2);
    EXPECT_EQ(OpCode::setDWord(buf, 5, 0xB0CFA1B2), true);
    EXPECT_EQ(buf[1], 0xB0);
    EXPECT_EQ(buf[2], 0xCF);
    EXPECT_EQ(buf[3], 0xA1);
    EXPECT_EQ(buf[4], 0xB2);
}
