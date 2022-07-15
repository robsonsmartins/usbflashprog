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
 * @file test/backend/runner_test.cpp
 * @brief Implementation of Unit Test for Runner Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "../mock/qserialport.hpp"
#include "runner_test.hpp"
#include "../../backend/runner.hpp"
#include <QThread>

// ---------------------------------------------------------------------------

TEST_F(RunnerTest, RunnerCommand) {
    TRunnerCommand r1, r2;
    QByteArray data;
    r1.opcode = kCmdOpCodes.at(kCmdVddCtrl);
    r1.params.append(kCmdParamOn);
    EXPECT_EQ(r1 == r2, false);
    r2 = r1;
    EXPECT_EQ(r1 == r2, true);
    r1.set(kCmdVppCtrl, true);
    EXPECT_EQ(r1.opcode, kCmdOpCodes.at(kCmdVppCtrl));
    data.clear();
    data.append(kCmdVppCtrl).append(kCmdParamOn);
    EXPECT_EQ(r1.params, data);
    r1.set(kCmdVddSetV, 4.20f);
    EXPECT_EQ(r1.opcode, kCmdOpCodes.at(kCmdVddSetV));
    data.clear();
    data.append(kCmdVddSetV).append(4).append(20);
    EXPECT_EQ(r1.params[0], data[0]);
    EXPECT_NEAR(r1.params[1], data[1], 1);
    EXPECT_NEAR(r1.params[2], data[2], 1);
    r1.set(kCmdVppCtrl, 1);
    EXPECT_EQ(r1.opcode, kCmdOpCodes.at(kCmdVppCtrl));
    data.clear();
    data.append(kCmdVppCtrl).append(1);
    EXPECT_EQ(r1.params, data);
    r1.response.append(kCmdResponseNok).append(21).append(33);
    EXPECT_EQ(r1.responseIsOk(), false);
    r1.response[0] = kCmdResponseOk;
    EXPECT_EQ(r1.responseIsOk(), true);
    EXPECT_EQ(r1.responseAsByte(), 21);
    EXPECT_NEAR(r1.responseAsFloat(), 21.33f, 0.1f);
    EXPECT_EQ(r1.responseAsBool(), true);
    r1.response[1] = 0;
    EXPECT_EQ(r1.responseAsBool(), false);
}

TEST_F(RunnerTest, Runner_list) {
    Runner r1;
    TSerialPortList l1 = r1.list();
    EXPECT_EQ(l1.size(), 1);
    EXPECT_EQ(l1[0].portName(), "COM1");
    EXPECT_EQ(l1[0].vendorIdentifier(), 0x2E8A);
    EXPECT_EQ(l1[0].productIdentifier(), 0x000A);
}

TEST_F(RunnerTest, Runner_open_close) {
    Runner r1;
    EXPECT_EQ(r1.isOpen(), false);
    EXPECT_EQ(r1.getPath(), QString(""));
    EXPECT_EQ(r1.open("COM5"), true);
    EXPECT_EQ(r1.isOpen(), true);
    EXPECT_EQ(r1.getPath(), QString("COM5"));
    r1.close();
    EXPECT_EQ(r1.isOpen(), false);
    EXPECT_EQ(r1.getPath(), QString(""));
}

TEST_F(RunnerTest, Runner_send) {
    Runner r1;
    EXPECT_EQ(r1.open("COM5"), true);
    QObject::connect(&r1, &Runner::resultReady,
        [=](const TRunnerCommand& command) {
            EXPECT_EQ(command.responseIsOk(), true);
            EXPECT_EQ(command.responseAsByte(), 20);
            EXPECT_EQ(command.responseAsBool(), true);
            EXPECT_NEAR(command.responseAsFloat(), 20.33f, 0.1f);
    });
    r1.send(kCmdVppInitCal);
    r1.send(kCmdVppCtrl, true);
    r1.send(kCmdVppCtrl, 1);
    r1.send(kCmdVppSetV, 12.33f);
    QThread::msleep(5000);
    r1.close();
}
