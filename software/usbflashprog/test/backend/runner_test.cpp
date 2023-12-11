// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2023) Robson Martins
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

#include "runner_test.hpp"
#include "../../backend/runner.hpp"

#include <chrono>
#include <cstring>

// ---------------------------------------------------------------------------

TEST_F(RunnerTest, list_open_close) {
    Runner runner;
    TSerialPortList items = runner.list();
    EXPECT_EQ(items.size(), 1);
    EXPECT_EQ(items[0].portName().toStdString(), "COM1");
    EXPECT_EQ(runner.isOpen(), false);
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.getPath().toStdString(), "COM1");
    EXPECT_EQ(runner.isOpen(), true);
    runner.setTimeOut(400);
    EXPECT_EQ(runner.getTimeOut(), 400);
    runner.setTimeOut(200);
    EXPECT_EQ(runner.getTimeOut(), 200);
    runner.close();
    EXPECT_EQ(runner.isOpen(), false);
}

TEST_F(RunnerTest, disconnect) {
    Runner runner;
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.vddCtrl(), true);
    runner.setTimeOut(20);

    QSerialPort::connected = false;
    EXPECT_EQ(runner.vddCtrl(), false);

    runner.close();
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.vddCtrl(), true);
    runner.setTimeOut(20);

    QSerialPort::connected = false;
    EXPECT_EQ(runner.dataGetW(), 0xffff);
    Runner::msDelay(2000);
    EXPECT_EQ(runner.dataGetW(), 0xffff);

    runner.close();
}

TEST_F(RunnerTest, nop) {
    Runner runner;
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.nop(), true);
    runner.close();
}

TEST_F(RunnerTest, cmdVdd) {
    Runner runner;
    float expected = kSerialPortDummyData[1] + kSerialPortDummyData[2] / 100.0f;
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.vddCtrl(), true);
    EXPECT_EQ(runner.vddCtrl(false), true);
    EXPECT_EQ(runner.vddCtrl(true), true);
    EXPECT_EQ(runner.vddSet(expected), true);
    EXPECT_EQ(runner.vddGet(), expected);
    EXPECT_EQ(runner.vddGetDuty(), expected);
    EXPECT_EQ(runner.vddGetCal(), expected);
    EXPECT_EQ(runner.vddInitCal(), true);
    EXPECT_EQ(runner.vddSaveCal(expected), true);
    EXPECT_EQ(runner.vddOnVpp(), true);
    EXPECT_EQ(runner.vddOnVpp(false), true);
    EXPECT_EQ(runner.vddOnVpp(true), true);
    runner.close();
}

TEST_F(RunnerTest, cmdVpp) {
    Runner runner;
    float expected = kSerialPortDummyData[1] + kSerialPortDummyData[2] / 100.0f;
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.vppCtrl(), true);
    EXPECT_EQ(runner.vppCtrl(false), true);
    EXPECT_EQ(runner.vppCtrl(true), true);
    EXPECT_EQ(runner.vppSet(expected), true);
    EXPECT_EQ(runner.vppGet(), expected);
    EXPECT_EQ(runner.vppGetDuty(), expected);
    EXPECT_EQ(runner.vppGetCal(), expected);
    EXPECT_EQ(runner.vppInitCal(), true);
    EXPECT_EQ(runner.vppSaveCal(expected), true);
    EXPECT_EQ(runner.vppOnA18(), true);
    EXPECT_EQ(runner.vppOnA18(false), true);
    EXPECT_EQ(runner.vppOnA18(true), true);
    EXPECT_EQ(runner.vppOnA9(), true);
    EXPECT_EQ(runner.vppOnA9(false), true);
    EXPECT_EQ(runner.vppOnA9(true), true);
    EXPECT_EQ(runner.vppOnCE(), true);
    EXPECT_EQ(runner.vppOnCE(false), true);
    EXPECT_EQ(runner.vppOnCE(true), true);
    EXPECT_EQ(runner.vppOnOE(), true);
    EXPECT_EQ(runner.vppOnOE(false), true);
    EXPECT_EQ(runner.vppOnOE(true), true);
    EXPECT_EQ(runner.vppOnWE(), true);
    EXPECT_EQ(runner.vppOnWE(false), true);
    EXPECT_EQ(runner.vppOnWE(true), true);
    runner.close();
}

TEST_F(RunnerTest, cmdBusCtrl) {
    Runner runner;
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.setCE(), true);
    EXPECT_EQ(runner.setCE(false), true);
    EXPECT_EQ(runner.setCE(true), true);
    EXPECT_EQ(runner.setOE(), true);
    EXPECT_EQ(runner.setOE(false), true);
    EXPECT_EQ(runner.setOE(true), true);
    EXPECT_EQ(runner.setWE(), true);
    EXPECT_EQ(runner.setWE(false), true);
    EXPECT_EQ(runner.setWE(true), true);
    runner.close();
}

TEST_F(RunnerTest, cmdBusAddr) {
    Runner runner;
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.addrClr(), true);
    EXPECT_EQ(runner.addrInc(), true);
    EXPECT_EQ(runner.addrSet(0x12345678), true);
    EXPECT_EQ(runner.addrSetW(0x1234), true);
    EXPECT_EQ(runner.addrSetB(0x12), true);
    runner.close();
}

TEST_F(RunnerTest, cmdBusData) {
    Runner runner;
    uint16_t expected = kSerialPortDummyData[1] << 8 | kSerialPortDummyData[2];
    EXPECT_EQ(runner.open(QString("COM1")), true);
    EXPECT_EQ(runner.dataClr(), true);
    EXPECT_EQ(runner.dataSet(0x12), true);
    EXPECT_EQ(runner.dataSetW(0x1234), true);
    EXPECT_EQ(runner.dataGetW(), expected);
    EXPECT_EQ(runner.dataGet(), expected >> 8);
    runner.close();
}

TEST_F(RunnerTest, delay) {
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    int64_t elapsed = 0;

    start = std::chrono::steady_clock::now();
    Runner::usDelay(10);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 10, 100);

    start = std::chrono::steady_clock::now();
    Runner::usDelay(100);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 100, 100);

    start = std::chrono::steady_clock::now();
    Runner::usDelay(200);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 200, 100);

    start = std::chrono::steady_clock::now();
    Runner::usDelay(1000);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 1000, 100);

    start = std::chrono::steady_clock::now();
    Runner::usDelay(10000);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 10, 1);

    start = std::chrono::steady_clock::now();
    Runner::msDelay(5);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 5, 1);

    start = std::chrono::steady_clock::now();
    Runner::msDelay(50);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 50, 5);

    start = std::chrono::steady_clock::now();
    Runner::msDelay(500);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 500, 50);

    start = std::chrono::steady_clock::now();
    Runner::msDelay(1000);
    end = std::chrono::steady_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                  .count();
    EXPECT_NEAR(elapsed, 1000, 100);
}
