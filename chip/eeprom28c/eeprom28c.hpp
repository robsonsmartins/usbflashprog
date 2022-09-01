// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup lib_chip_eeprom28c
 * @file    eeprom28c.hpp
 * @brief   Header file (C++) for EEPROM 28Cxxx Chip Emulation Library
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CHIP_EEPROM_28C_HPP_  // NOLINT
#define CHIP_EEPROM_28C_HPP_

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

/** @ingroup lib_chip_eeprom28c
    Set Memory Size.
    @param[in] size Memory size (number of addressable positions)
    @return None */
DLLEXPORT void SetSize(unsigned long size);

/** @ingroup lib_chip_eeprom28c
    Set/Reset VDD (main power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SetVDD(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Set/Reset VPP (high voltage programming power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SetVPP(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Set/Reset OE (Output Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SetOE(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Set/Reset CE (Chip Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SetCE(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Set/Reset WE (Write Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SetWE(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Write a Address Bus value.
    @param[in] addr Address to be writed in bus
    @return None */
DLLEXPORT void SetAddrBus(unsigned long addr);

/** @ingroup lib_chip_eeprom28c
    Write a value to Data Bus.
    @param[in] data Data to be writed in bus
    @return None */
DLLEXPORT void SetDataBus(unsigned short data);

/** @ingroup lib_chip_eeprom28c
    Read a value from Data Bus.
    @return Data readed from bus. */
DLLEXPORT unsigned short GetDataBus(void);

/** @ingroup lib_chip_eeprom28c
    Set/Reset Serial CS (Chip Select) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SerialSetCS(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Set/Reset Serial Clock pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SerialSetClk(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Set/Reset Serial Data pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
    @return None */
DLLEXPORT void SerialSetData(unsigned char state);

/** @ingroup lib_chip_eeprom28c
    Read Serial Data pin current state (1 or 0) from chip.
    @return Pin level (high or low). */
DLLEXPORT unsigned char SerialGetData(void);

/** @ingroup lib_chip_eeprom28c
    Library Entry point for MS Windows.
    @param hInst Library Instance
    @param reason Reason of the call
    @param reserved Not used
    @return TRUE if success, FALSE if error. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

// ---------------------------------------------------------------------------
/* internal classes */

/** @ingroup chip
    @brief   EEPROM 28Cxxx Chip Emulator Class.
    @details Emulates a EEPROM 28Cxxx Chip. */
class ChipEEPROM28C: public BaseParChip {
 public:
    /** Default Constructor. */
    ChipEEPROM28C();
    /** Destructor. */
    virtual ~ChipEEPROM28C();
    /* reimplemented */
    virtual void SetSize(unsigned long size);
 protected:
    /* emulates the chip */
    virtual void EmuChip(void);
};

#endif  // CHIP_EEPROM_28C_HPP_
