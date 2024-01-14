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
 * @file test/emulator/chip.cpp
 * @brief Implementation of Chip Emulation Base Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QRandomGenerator>
#include <QLoggingCategory>
#include <QString>

#include "chip.hpp"
#include "main.hpp"

// ---------------------------------------------------------------------------
// Logging

Q_LOGGING_CATEGORY(emulateChip, "emu.chip")

#define DEBUG qCDebug(emulateChip)
#define INFO qCInfo(emulateChip)
#define WARNING qCWarning(emulateChip)
#define CRITICAL qCCritical(emulateChip)
#define FATAL qCFatal(emulateChip)

// ---------------------------------------------------------------------------

BaseChip::BaseChip()
    : f_vdd(false), f_vpp(false), f_addr_bus(0), f_data_bus(0) {}

BaseChip::BaseChip(const BaseChip& src)
    : f_vdd(src.f_vdd),
      f_vpp(src.f_vpp),
      f_addr_bus(src.f_addr_bus),
      f_data_bus(src.f_data_bus) {
    f_memory_area = src.f_memory_area;
}

BaseChip& BaseChip::operator=(const BaseChip& src) {
    f_vdd = src.f_vdd;
    f_vpp = src.f_vpp;
    f_addr_bus = src.f_addr_bus;
    f_data_bus = src.f_data_bus;
    f_memory_area = src.f_memory_area;
    return *this;
}

BaseChip::~BaseChip() {}

void BaseChip::setSize(uint32_t size) {
    if (size == f_memory_area.size()) return;
    /* sets the chip size */
    f_memory_area.resize(size);
}

void BaseChip::setVDD(bool state) {
    if (state == f_vdd) return;
    writeToLog("SetVDD(%d)", static_cast<int>(state));
    f_vdd = state;
    emuChip();
}

void BaseChip::setVPP(bool state) {
    if (state == f_vpp) return;
    writeToLog("SetVPP(%d)", static_cast<int>(state));
    f_vpp = state;
    emuChip();
}

void BaseChip::randomizeData(void) {
    /* fills memory area with random data */
    for (int i = 0; i < f_memory_area.size(); ++i) {
        f_memory_area[i] =
            static_cast<uint16_t>(QRandomGenerator::global()->generate());
    }
}

void BaseChip::fillData(uint16_t data) {
    /* fills memory area with specified data */
    std::fill(f_memory_area.begin(), f_memory_area.end(), data);
}

void BaseChip::read(void) {
    /* checks the params */
    if (f_addr_bus >= f_memory_area.size()) {
        f_data_bus = 0xFFFF;
        writeToLog("Error in Read(addr=%06.6lX)", f_addr_bus);
        return;
    }
    static uint32_t last_addr = -1;
    uint16_t data = f_memory_area[f_addr_bus];
    if (f_data_bus == data && f_addr_bus == last_addr) return;
    /* returns the data from memory area */
    f_data_bus = data;
    /* update last_addr */
    last_addr = f_addr_bus;
    writeToLog("Read(addr=%06.6lX) = %04.4X", f_addr_bus, f_data_bus);
}

void BaseChip::write(void) {
    /* checks the params */
    if (f_addr_bus >= f_memory_area.size()) {
        writeToLog("Error in Write(addr=%06.6lX,data=%04.4X)", f_addr_bus,
                   f_data_bus);
        return;
    }
    /* writes the data to memory area */
    f_memory_area[f_addr_bus] = f_data_bus;
    writeToLog("Write(addr=%06.6lX,data=%04.4X)", f_addr_bus, f_data_bus);
}

void BaseChip::writeToLog(const char* msg, ...) {
    // logLevel == disabled
    if (kTestLogLevel == 0) return;
    /* checks the params */
    if (msg == NULL) return;
    /* writes the params to log */
    char log[256];
    va_list args;
    va_start(args, msg);
    vsnprintf(log, sizeof(log), msg, args);
    va_end(args);
    DEBUG << log;
}

// ---------------------------------------------------------------------------

BaseParChip::BaseParChip()
    : BaseChip(), f_oe(false), f_ce(false), f_we(false) {}

BaseParChip::~BaseParChip() {}

void BaseParChip::setOE(bool state) {
    if (state == f_oe) return;
    writeToLog("SetOE(%d)", static_cast<int>(state));
    f_oe = state;
    emuChip();
}

void BaseParChip::setCE(bool state) {
    if (state == f_ce) return;
    writeToLog("SetCE(%d)", static_cast<int>(state));
    f_ce = state;
    emuChip();
}

void BaseParChip::setWE(bool state) {
    if (state == f_we) return;
    writeToLog("SetWE(%d)", static_cast<int>(state));
    f_we = state;
    emuChip();
}

void BaseParChip::setAddrBus(uint32_t addr) {
    /* calculates the valid address (into memory area) */
    uint32_t addr_new = (addr & (f_memory_area.size() - 1));
    if (f_addr_bus == addr_new) return;
    /* set addr bus */
    f_addr_bus = addr_new;
}

void BaseParChip::setDataBus(uint16_t data) {
    if (f_data_bus == data) return;
    /* changes the data bus */
    f_data_bus = data;
    /* writes data */
    emuChip();
}

uint16_t BaseParChip::getDataBus(void) {
    /* reads data */
    emuChip();
    /* returns the data bus */
    return f_data_bus;
}
