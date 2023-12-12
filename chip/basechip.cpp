// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup chip
 * @file    basechip.cpp
 * @brief   Chip Emulation Base Classes (C++ implementation file)
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#define NOGDI
#include <windows.h>
#include <shlobj.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <new>
#include <vector>
#include <string>
#include <algorithm>
#include "basechip.hpp"

// ---------------------------------------------------------------------------
/* defines */

/* Log filename */
#define CHIP_LOG_FILENAME "chip.log"

// ---------------------------------------------------------------------------
/* variables */

/* High precision timer frequency, in ticks/sec. */
static LONGLONG PerformanceFrequency = 0;

// ---------------------------------------------------------------------------
/* functions */

/* Gets the current System Tick (in usec).
   @return System Tick, in usec (64 bits wide). */
static unsigned long long usGetSystemTick(void);
/* Generates a random unsigned short number.
   @return Random number (in range: 0 to 65535). */
static unsigned short GenRandomUShort(void);
/* Creates the logfile. */
static FILE* CreateLogFile(void);

// ---------------------------------------------------------------------------

BaseChip::BaseChip()
    : f_vdd(false), f_vpp(false), f_addr_bus(0), f_data_bus(0) {
    /* logfile */
    f_logfile = CreateLogFile();
    f_last_tick = usGetSystemTick();
}

BaseChip::BaseChip(const BaseChip& src)
    : f_vdd(src.f_vdd),
      f_vpp(src.f_vpp),
      f_addr_bus(src.f_addr_bus),
      f_data_bus(src.f_data_bus) {
    f_logfile = CreateLogFile();
    f_last_tick = usGetSystemTick();
    f_memory_area = src.f_memory_area;
}

BaseChip& BaseChip::operator=(const BaseChip& src) {
    f_vdd = src.f_vdd;
    f_vpp = src.f_vpp;
    f_addr_bus = src.f_addr_bus;
    f_data_bus = src.f_data_bus;
    if (f_logfile) {
        fclose(f_logfile);
    }
    f_logfile = CreateLogFile();
    f_last_tick = usGetSystemTick();
    f_memory_area = src.f_memory_area;
    return *this;
}

BaseChip::~BaseChip() {
    /* logfile */
    if (f_logfile) {
        fclose(f_logfile);
    }
}

void BaseChip::SetSize(unsigned long size) {
    if (size == f_memory_area.size()) {
        return;
    }
    /* log */
    WriteToLog("SetSize(%lu)", size);
    /* sets the chip size */
    if (f_memory_area.size() != size) {
        f_memory_area.resize(size);
    }
}

void BaseChip::SetVDD(bool state) {
    if (state == f_vdd) {
        return;
    }
    /* log */
    WriteToLog("SetVDD(%d)", static_cast<int>(state));
    f_vdd = state;
    EmuChip();
}

void BaseChip::SetVPP(bool state) {
    if (state == f_vpp) {
        return;
    }
    /* log */
    WriteToLog("SetVPP(%d)", static_cast<int>(state));
    f_vpp = state;
    EmuChip();
}

void BaseChip::RandomizeData(void) {
    /* fills memory area with random data */
    srand(time(NULL));
    generate(f_memory_area.begin(), f_memory_area.end(), GenRandomUShort);
}

void BaseChip::FillData(unsigned short data) {
    /* fills memory area with specified data */
    fill(f_memory_area.begin(), f_memory_area.end(), data);
}

void BaseChip::Read(void) {
    /* checks the params */
    if (f_addr_bus >= f_memory_area.size()) {
        WriteToLog("Error in Read(addr=%06.6lX)", f_addr_bus);
        f_data_bus = 0xFFFF;
        return;
    }
    static unsigned long last_addr = f_addr_bus;
    unsigned short data = f_memory_area[f_addr_bus];
    if (f_data_bus == data && f_addr_bus == last_addr) {
        return;
    }
    /* returns the data from memory area */
    f_data_bus = data;
    /* update last_addr */
    last_addr = f_addr_bus;
    /* log */
    WriteToLog("Read(addr=%06.6lX) = %04.4X", f_addr_bus, f_data_bus);
}

void BaseChip::Write(void) {
    /* checks the params */
    if (f_addr_bus >= f_memory_area.size()) {
        WriteToLog("Error in Write(addr=%06.6lX,data=%04.4X)", f_addr_bus,
                   f_data_bus);
        return;
    }
    /* writes the data to memory area */
    f_memory_area[f_addr_bus] = f_data_bus;
    /* log */
    WriteToLog("Write(addr=%06.6lX,data=%04.4X)", f_addr_bus, f_data_bus);
}

void BaseChip::WriteToLog(const char* msg, ...) {
    /* checks the params */
    if (f_logfile == NULL || msg == NULL) {
        return;
    }
    unsigned long long current_tick = usGetSystemTick();
    /* writes a timestamp to log */
    fprintf(f_logfile, "%10.10llu ", current_tick - f_last_tick);
    f_last_tick = current_tick;
    /* writes the params to log */
    va_list args;
    va_start(args, msg);
    vfprintf(f_logfile, msg, args);
    va_end(args);
    /* writes the line terminator to log */
    fprintf(f_logfile, "\n");
    fflush(f_logfile);
}

// ---------------------------------------------------------------------------

BaseParChip::BaseParChip()
    : BaseChip(), f_oe(false), f_ce(false), f_we(false) {}

BaseParChip::~BaseParChip() {}

void BaseParChip::SetOE(bool state) {
    if (state == f_oe) {
        return;
    }
    /* log */
    WriteToLog("SetOE(%d)", static_cast<int>(state));
    f_oe = state;
    EmuChip();
}

void BaseParChip::SetCE(bool state) {
    if (state == f_ce) {
        return;
    }
    /* log */
    WriteToLog("SetCE(%d)", static_cast<int>(state));
    f_ce = state;
    EmuChip();
}

void BaseParChip::SetWE(bool state) {
    if (state == f_we) {
        return;
    }
    /* log */
    WriteToLog("SetWE(%d)", static_cast<int>(state));
    f_we = state;
    EmuChip();
}

void BaseParChip::SetAddrBus(unsigned long addr) {
    /* calculates the valid address (into memory area) */
    unsigned long addr_new = (addr & (f_memory_area.size() - 1));
    if (f_addr_bus == addr_new) {
        return;
    }
    /* set addr bus */
    f_addr_bus = addr_new;
}

void BaseParChip::SetDataBus(unsigned short data) {
    if (f_data_bus == data) {
        return;
    }
    /* changes the data bus */
    f_data_bus = data;
    /* writes data */
    EmuChip();
}

unsigned short BaseParChip::GetDataBus(void) {
    /* reads data */
    EmuChip();
    /* returns the data bus */
    return f_data_bus;
}

// ---------------------------------------------------------------------------

BaseSerChip::BaseSerChip()
    : BaseChip(), f_ser_cs(false), f_ser_clk(false), f_ser_data(false) {}

BaseSerChip::~BaseSerChip() {}

void BaseSerChip::SerialSetCS(bool state) {
    if (state == f_ser_cs) {
        return;
    }
    /* log */
    WriteToLog("SerialSetCS(%d)", static_cast<int>(state));
    f_ser_cs = state;
    EmuChip();
}

void BaseSerChip::SerialSetClk(bool state) {
    if (state == f_ser_clk) {
        return;
    }
    /* log */
    WriteToLog("SerialSetClk(%d)", static_cast<int>(state));
    f_ser_clk = state;
    EmuChip();
}

void BaseSerChip::SerialSetData(bool state) {
    /* changes the data pin state */
    f_ser_data = state;
}

bool BaseSerChip::SerialGetData(void) {
    /* returns the data pin current state */
    EmuChip();
    return f_ser_data;
}

// ---------------------------------------------------------------------------
/* internal functions */

unsigned long long usGetSystemTick(void) {
    unsigned long long result;
    LARGE_INTEGER PerFreq, Count;
    long double ld_result;
    /* gets the current counter */
    QueryPerformanceCounter(&Count);
    /* gets the timer frequency, if global variable not assigned */
    if (PerformanceFrequency == 0) {
        /* Get performance details for timing */
        if (!QueryPerformanceFrequency(&PerFreq)) {
            /* No high precision timer, then calls GetTickCount() */
            return static_cast<unsigned long long>(GetTickCount()) * 1000;
        }
        /* updates the global variable */
        PerformanceFrequency = PerFreq.QuadPart;
    }
    /* calculates counter to usec */
    ld_result = static_cast<long double>(Count.QuadPart) * 1000000.0L;
    ld_result /= static_cast<long double>(PerformanceFrequency);
    result = static_cast<unsigned long long>(ld_result);
    /* returns */
    return result;
}

unsigned short GenRandomUShort(void) {
    /* generates a random unsigned short number */
    return static_cast<unsigned short>(65536.0 * rand() / (RAND_MAX + 1.0));
}

FILE* CreateLogFile(void) {
    CHAR path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path))) {
        std::string filename = path + std::string("\\") + CHIP_LOG_FILENAME;
        return fopen(filename.c_str(), "w");
    }
    return NULL;
}
