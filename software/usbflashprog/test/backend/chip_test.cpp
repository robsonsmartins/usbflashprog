// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/backend/chip_test.cpp
 * @brief Implementation of Unit Test for Device (Chip) Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QByteArray>
#include <QObject>
#include <cmath>

#include "chip_test.hpp"

#include "emulator/emulator.hpp"
#include "emulator/sram.hpp"
#include "emulator/eprom.hpp"

#include "../../backend/devices/parallel/sram.hpp"
#include "../../backend/devices/parallel/eprom.hpp"

// ---------------------------------------------------------------------------

/* GTest macro to print to console */
#define GTEST_COUT std::cerr << "[          ] ChipTest current: "

// ---------------------------------------------------------------------------
// private functions

/* Run the chip tests.
   @param emuChip Pointer to Chip Emulator object.
   @param device Pointer to Device object to test.
   @param size Size of the device, in bytes.
 */
void runChipTests(BaseChip *emuChip, Device *device, uint32_t size);

// ---------------------------------------------------------------------------

TEST_F(ChipTest, sram_test) {
    ChipSRAM *emuChip = new ChipSRAM();
    Emulator::setChip(emuChip);
    SRAM *device = new SRAM();

    // test "not open"
    QByteArray buffer;
    uint32_t size = 2048;
    emuChip->setSize(size);
    device->setSize(size);
    Emulator::randomizeBuffer(buffer, size);
    GTEST_COUT << "Program" << std::endl;
    EXPECT_EQ(device->program(buffer), false);

    // run another tests
    runChipTests(emuChip, device, 0x000800);  // 2KB
    runChipTests(emuChip, device, 0x001000);  // 4KB
    runChipTests(emuChip, device, 0x002000);  // 8KB
    delete device;
    delete emuChip;
}

TEST_F(ChipTest, eprom27_test) {
    ChipEPROM *emuChip = new ChipEPROM();
    Emulator::setChip(emuChip);
    M27xxx *device = new M27xxx();
    runChipTests(emuChip, device, 0x000800);  // 2KB
    runChipTests(emuChip, device, 0x001000);  // 4KB
    runChipTests(emuChip, device, 0x002000);  // 8KB
    delete emuChip;
    delete device;
}

TEST_F(ChipTest, eprom27C_test) {
    ChipEPROM *emuChip = new ChipEPROM();
    Emulator::setChip(emuChip);
    M27Cxxx *device = new M27Cxxx();
    runChipTests(emuChip, device, 0x000800);  // 2KB
    runChipTests(emuChip, device, 0x001000);  // 4KB
    runChipTests(emuChip, device, 0x002000);  // 8KB
    delete emuChip;
    delete device;
}

TEST_F(ChipTest, eprom27C16Bit_test) {
    ChipEPROM *emuChip = new ChipEPROM();
    Emulator::setChip(emuChip);
    M27C16Bit *device = new M27C16Bit();
    runChipTests(emuChip, device, 0x000800);  // 2KB
    runChipTests(emuChip, device, 0x001000);  // 4KB
    runChipTests(emuChip, device, 0x002000);  // 8KB
    runChipTests(emuChip, device, 0x200000);  // 2MB
    delete emuChip;
    delete device;
}

TEST_F(ChipTest, epromW27E_test) {
    ChipEPROM *emuChip = new ChipEPROM();
    Emulator::setChip(emuChip);
    W27Exxx *device = new W27Exxx();
    runChipTests(emuChip, device, 0x000800);  // 2KB
    runChipTests(emuChip, device, 0x001000);  // 4KB
    runChipTests(emuChip, device, 0x002000);  // 8KB
    delete emuChip;
    delete device;
}

// ---------------------------------------------------------------------------

void runChipTests(BaseChip *emuChip, Device *device, uint32_t size) {
    QByteArray buffer;
    device->setPort("COM1");
    emuChip->setSize(size);
    device->setSize(size);
    Emulator::randomizeBuffer(buffer, size);

    TDeviceInformation info = device->getInfo();
    TDeviceCapabilities cap = info.capability;
    GTEST_COUT << "Device: " << info.name.toStdString() << " Size: " << size
               << std::endl;

    EXPECT_EQ(device->getPort().toStdString(), "COM1");
    EXPECT_EQ(device->getSize(), size);
    device->setTwp(1);
    EXPECT_EQ(device->getTwp(), 1);
    device->setTwc(1);
    EXPECT_EQ(device->getTwc(), 1);
    device->setVddRd(7.0f);
    EXPECT_NEAR(device->getVddRd(), 7.0f, 0.1f);
    device->setVddWr(7.0f);
    EXPECT_NEAR(device->getVddWr(), 7.0f, 0.1f);
    device->setVpp(10.0f);
    EXPECT_NEAR(device->getVpp(), 10.0f, 0.1f);
    device->setVee(10.0f);
    EXPECT_NEAR(device->getVee(), 10.0f, 0.1f);
    device->setSkipFF();
    EXPECT_EQ(device->getSkipFF(), true);
    device->setSkipFF(false);
    device->setFastProg();
    EXPECT_EQ(device->getFastProg(), true);
    device->setFastProg(false);
    device->setSectorSize(128);
    EXPECT_EQ(device->getSectorSize(), 128);
    device->setSectorSize(0);
    device->cancel();

    if (cap.hasBlankCheck) {
        GTEST_COUT << "Blank Check" << std::endl;
        EXPECT_EQ(device->blankCheck(), true);
    }
    if (cap.hasProgram) {
        GTEST_COUT << "Program" << std::endl;
        EXPECT_EQ(device->program(buffer), true);
        if (cap.hasVerify) {
            GTEST_COUT << "Verify" << std::endl;
            EXPECT_EQ(device->verify(buffer), true);
        }
    }
    if (cap.hasRead) {
        GTEST_COUT << "Read" << std::endl;
        QByteArray rdBuffer;
        EXPECT_EQ(device->read(rdBuffer), true);
    }
    if (cap.hasProgram && cap.hasVerify) {
        GTEST_COUT << "Program and Verify" << std::endl;
        EXPECT_EQ(device->program(buffer, true), true);
    }
    if (cap.hasProgram && cap.hasBlankCheck) {
        GTEST_COUT << "Blank Check" << std::endl;
        EXPECT_EQ(device->blankCheck(), false);
    }
    if (cap.hasProgram && cap.hasVerify) {
        buffer[buffer.size() - 1] = ~buffer[buffer.size() - 1];
        GTEST_COUT << "Verify" << std::endl;
        EXPECT_EQ(device->verify(buffer), false);
    }
    if (cap.hasErase) {
        GTEST_COUT << "Erase" << std::endl;
        EXPECT_EQ(device->erase(), true);
        if (cap.hasBlankCheck) {
            GTEST_COUT << "Blank Check" << std::endl;
            EXPECT_EQ(device->blankCheck(), true);
        }
        if (cap.hasVerify) {
            buffer[buffer.size() - 1] = ~buffer[buffer.size() - 1];
            GTEST_COUT << "Verify" << std::endl;
            EXPECT_EQ(device->verify(buffer), false);
        }
        if (cap.hasProgram) {
            GTEST_COUT << "Program" << std::endl;
            EXPECT_EQ(device->program(buffer), true);
        }
        if (cap.hasBlankCheck) {
            GTEST_COUT << "Erase and Blank Check" << std::endl;
            EXPECT_EQ(device->erase(true), true);
        }
    }
    if (cap.hasSkipFF && cap.hasProgram && cap.hasErase) {
        GTEST_COUT << "Erase" << std::endl;
        EXPECT_EQ(device->erase(), true);
        device->setSkipFF();
        GTEST_COUT << "Program with skip 0xFF" << std::endl;
        EXPECT_EQ(device->program(buffer), true);
        device->setSkipFF(false);
    }
    if (cap.hasFastProg && cap.hasProgram && cap.hasErase) {
        device->setFastProg();
        GTEST_COUT << "Erase/Fast Erase" << std::endl;
        EXPECT_EQ(device->erase(), true);
        GTEST_COUT << "Fast Program" << std::endl;
        EXPECT_EQ(device->program(buffer), true);
        device->setFastProg(false);
    }
    if (cap.hasSectorSize && cap.hasProgram && cap.hasErase) {
        device->setSectorSize(64);
        GTEST_COUT << "Erase" << std::endl;
        EXPECT_EQ(device->erase(), true);
        GTEST_COUT << "Sector Program (64 bytes)" << std::endl;
        EXPECT_EQ(device->program(buffer), true);
        device->setSectorSize(0);
    }
}