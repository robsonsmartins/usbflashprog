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
 * @file    basechip.hpp
 * @brief   Chip Emulation Base Classes (C++ header file)
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CHIP_BASE_HPP_  // NOLINT
#define CHIP_BASE_HPP_

// ---------------------------------------------------------------------------
/* headers */

#include <vector>
#include <cstdio>
#include <cstdarg>

// ---------------------------------------------------------------------------

/** @ingroup chip
    @brief   Chip Emulator Base Abstract Class.
    @details This is a base class for Chip Emulator.<br>
    Provides all abstract methods need for a chip emulator. */
class BaseChip {
  public:
    /** Default Constructor. */
    BaseChip();
    /**
     * @brief Copy constructor.
     * @param src Another BaseChip object.
     */
    explicit BaseChip(const BaseChip& src);
    /**
     * @brief Assignment operator.
     * @param src Another BaseChip object.
     * @return Reference to this object.
     */
    BaseChip& operator=(const BaseChip& src);
    /** Destructor. */
    virtual ~BaseChip();
    /** Set Memory Size.
        @param[in] size Memory size (number of addressable positions)
     */
    virtual void SetSize(unsigned long size);
    /** Set/Reset VDD (main power) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void SetVDD(bool state);
    /** Set/Reset VPP (high voltage programming power) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void SetVPP(bool state);

  protected:
    /* control signals */
    bool f_vdd, f_vpp;
    /* address bus */
    unsigned long f_addr_bus;
    /* data bus */
    unsigned short f_data_bus;
    /* memory area */
    std::vector<unsigned short> f_memory_area;
    /* logfile stream */
    FILE* f_logfile;
    /* Writes the msg and variables to log file.
       @param[in] msg String message
       @param[in] ... Variables */
    virtual void WriteToLog(const char* msg, ...);
    /* Reads data from memory area to Data Bus. */
    virtual void Read(void);
    /* Writes data from Data Bus to memory area. */
    virtual void Write(void);

    /** Emulates the chip.
     */
    virtual void EmuChip(void) = 0;
    /** Fills the entire memory area with random data.
     */
    virtual void RandomizeData(void);
    /** Fills the entire memory area with a data.
        @param[in] data Data to be filled into memory
     */
    virtual void FillData(unsigned short data);
};

// ---------------------------------------------------------------------------
/** @ingroup chip
    @brief   Parallel Chip Emulator Base Abstract Class.
    @details This is a base class for Parallel Chip Emulator.<br>
    Provides all abstract methods need for emulate a chip
    (with parallel bus). */
class BaseParChip : public BaseChip {
  public:
    /** Default Constructor. */
    BaseParChip();
    /** Destructor. */
    virtual ~BaseParChip();
    /** Set/Reset OE (Output Enable) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void SetOE(bool state);
    /** Set/Reset CE (Chip Enable) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void SetCE(bool state);
    /** Set/Reset WE (Write Enable) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void SetWE(bool state);
    /** Write a Address Bus value.
        @param[in] addr Address to be writed in bus
     */
    virtual void SetAddrBus(unsigned long addr);
    /** Write a value to Data Bus.
        @param[in] data Data to be writed in bus
     */
    virtual void SetDataBus(unsigned short data);
    /** Read a value from Data Bus.
        @return Data readed from bus. */
    virtual unsigned short GetDataBus(void);

  protected:
    /* control signals */
    bool f_oe, f_ce, f_we;
};

// ---------------------------------------------------------------------------
/** @ingroup chip
    @brief   Serial Chip Emulator Base Abstract Class.
    @details This is a base class for Serial Chip Emulator.<br>
    Provides all abstract methods need for emulate a chip
    (with serial bus). */
class BaseSerChip : public BaseChip {
  public:
    /** Default Constructor. */
    BaseSerChip();
    /** Destructor. */
    virtual ~BaseSerChip();
    /** Set/Reset CS (Chip Select) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void SerialSetCS(bool state);
    /** Put 1 (high level) or 0 (low level) to chip Clock pin.
        @param[in] state If true (default), put 1 to pin; else, put 0 to pin
     */
    virtual void SerialSetClk(bool state);
    /** Put 1 (high level) or 0 (low level) to chip Data pin.
        @param[in] state If true (default), put 1 to pin; else, put 0 to pin
     */
    virtual void SerialSetData(bool state);
    /** Read Data pin current state (1 or 0) from chip.
        @return Pin level (high or low). */
    virtual bool SerialGetData(void);

  protected:
    /* serial signals */
    bool f_ser_cs, f_ser_clk, f_ser_data;
};

#endif  // CHIP_BASE_HPP_
