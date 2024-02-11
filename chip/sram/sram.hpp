// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_sram
 * @file    sram.hpp
 * @brief   Header file (C++) for SRAM Chip Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CHIP_SRAM_HPP_  // NOLINT
#define CHIP_SRAM_HPP_

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

/** @ingroup lib_chip_sram
    Set Memory Size.
    @param[in] size Memory size (number of addressable positions)
 */
DLLEXPORT void SetSize(uint32_t size);

/** @ingroup lib_chip_sram
    Set/Reset VDD (main power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetVDD(uint8_t state);

/** @ingroup lib_chip_sram
    Set/Reset VPP (high voltage programming power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetVPP(uint8_t state);

/** @ingroup lib_chip_sram
    Set/Reset OE (Output Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetOE(uint8_t state);

/** @ingroup lib_chip_sram
    Set/Reset CE (Chip Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetCE(uint8_t state);

/** @ingroup lib_chip_sram
    Set/Reset WE (Write Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetWE(uint8_t state);

/** @ingroup lib_chip_sram
    Write a Address Bus value.
    @param[in] addr Address to be writed in bus
 */
DLLEXPORT void SetAddrBus(uint32_t addr);

/** @ingroup lib_chip_sram
    Write a value to Data Bus.
    @param[in] data Data to be writed in bus
 */
DLLEXPORT void SetDataBus(uint16_t data);

/** @ingroup lib_chip_sram
    Read a value from Data Bus.
    @return Data readed from bus. */
DLLEXPORT uint16_t GetDataBus(void);

/** @ingroup lib_chip_sram
    Set/Reset Serial CS (Chip Select) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetCS(uint8_t state);

/** @ingroup lib_chip_sram
    Set/Reset Serial Clock pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetClk(uint8_t state);

/** @ingroup lib_chip_sram
    Set/Reset Serial Data pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetData(uint8_t state);

/** @ingroup lib_chip_sram
    Read Serial Data pin current state (1 or 0) from chip.
    @return Pin level (high or low). */
DLLEXPORT uint8_t SerialGetData(void);

/** @ingroup lib_chip_sram
    Library Entry point for MS Windows.
    @param hInst Library Instance
    @param reason Reason of the call
    @param reserved Not used
    @return TRUE if success, FALSE if error. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

// ---------------------------------------------------------------------------
/* internal classes */

/** @ingroup chip
    @brief   SRAM Chip Emulator Class.
    @details Emulates a SRAM Chip. */
class ChipSRAM : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipSRAM();
    /** Destructor. */
    virtual ~ChipSRAM();
    /* reimplemented */
    virtual void SetSize(uint32_t size);
    virtual void SetVDD(bool state);

  protected:
    /* emulates the chip */
    virtual void EmuChip(void);
};

#endif  // CHIP_SRAM_HPP_
