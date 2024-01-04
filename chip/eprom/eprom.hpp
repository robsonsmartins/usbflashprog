// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_eprom
 * @file    eprom.hpp
 * @brief   Header file (C++) for EPROM 27xxx Chip Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CHIP_EPROM_HPP_  // NOLINT
#define CHIP_EPROM_HPP_

// ---------------------------------------------------------------------------
/* headers */

#define NOGDI
#include <windows.h>
#include "basechip.hpp"

// ---------------------------------------------------------------------------
/* defines */

#define DLLEXPORT extern "C" __stdcall __declspec(dllexport)
#define DLLIMPORT __stdcall

// ---------------------------------------------------------------------------
/* exported functions */

/** @ingroup lib_chip_eprom
    Set Memory Size.
    @param[in] size Memory size (number of addressable positions)
 */
DLLEXPORT void SetSize(unsigned long size);

/** @ingroup lib_chip_eprom
    Set/Reset VDD (main power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetVDD(unsigned char state);

/** @ingroup lib_chip_eprom
    Set/Reset VPP (high voltage programming power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetVPP(unsigned char state);

/** @ingroup lib_chip_eprom
    Set/Reset OE (Output Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetOE(unsigned char state);

/** @ingroup lib_chip_eprom
    Set/Reset CE (Chip Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetCE(unsigned char state);

/** @ingroup lib_chip_eprom
    Set/Reset WE (Write Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetWE(unsigned char state);

/** @ingroup lib_chip_eprom
    Write a Address Bus value.
    @param[in] addr Address to be writed in bus
 */
DLLEXPORT void SetAddrBus(unsigned long addr);

/** @ingroup lib_chip_eprom
    Write a value to Data Bus.
    @param[in] data Data to be writed in bus
 */
DLLEXPORT void SetDataBus(unsigned short data);

/** @ingroup lib_chip_eprom
    Read a value from Data Bus.
    @return Data readed from bus. */
DLLEXPORT unsigned short GetDataBus(void);

/** @ingroup lib_chip_eprom
    Set/Reset Serial CS (Chip Select) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetCS(unsigned char state);

/** @ingroup lib_chip_eprom
    Set/Reset Serial Clock pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetClk(unsigned char state);

/** @ingroup lib_chip_eprom
    Set/Reset Serial Data pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetData(unsigned char state);

/** @ingroup lib_chip_eprom
    Read Serial Data pin current state (1 or 0) from chip.
    @return Pin level (high or low). */
DLLEXPORT unsigned char SerialGetData(void);

/** @ingroup lib_chip_eprom
    Library Entry point for MS Windows.
    @param hInst Library Instance
    @param reason Reason of the call
    @param reserved Not used
    @return TRUE if success, FALSE if error. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

// ---------------------------------------------------------------------------
/* internal classes */

/** @ingroup chip
    @brief   EPROM 27xxx Chip Emulator Class.
    @details Emulates a EPROM 27xxx Chip. */
class ChipEPROM : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipEPROM();
    /** Destructor. */
    virtual ~ChipEPROM();
    /* reimplemented */
    virtual void SetSize(unsigned long size);

  protected:
    /* count n writes 0xFF to addr zero */
    int f_numWriteFFAddrZero;
    /* count n writes another data */
    int f_numWriteAnother;
    /* reimplemented */
    virtual void Write(void);
    /* emulates the chip */
    virtual void EmuChip(void);
};

#endif  // CHIP_EPROM_HPP_
