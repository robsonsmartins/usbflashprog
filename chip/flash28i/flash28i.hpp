// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_chip_flash28i
 * @file    flash28i.hpp
 * @brief   Header file (C++) for Flash Intel 28Fxxx Chip Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CHIP_FLASH_28I_HPP_  // NOLINT
#define CHIP_FLASH_28I_HPP_

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

/** @ingroup lib_chip_flash28i
    Set Memory Size.
    @param[in] size Memory size (number of addressable positions)
 */
DLLEXPORT void SetSize(uint32_t size);

/** @ingroup lib_chip_flash28i
    Set/Reset VDD (main power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetVDD(uint8_t state);

/** @ingroup lib_chip_flash28i
    Set/Reset VPP (high voltage programming power) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetVPP(uint8_t state);

/** @ingroup lib_chip_flash28i
    Set/Reset OE (Output Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetOE(uint8_t state);

/** @ingroup lib_chip_flash28i
    Set/Reset CE (Chip Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetCE(uint8_t state);

/** @ingroup lib_chip_flash28i
    Set/Reset WE (Write Enable) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SetWE(uint8_t state);

/** @ingroup lib_chip_flash28i
    Write a Address Bus value.
    @param[in] addr Address to be writed in bus
 */
DLLEXPORT void SetAddrBus(uint32_t addr);

/** @ingroup lib_chip_flash28i
    Write a value to Data Bus.
    @param[in] data Data to be writed in bus
 */
DLLEXPORT void SetDataBus(uint16_t data);

/** @ingroup lib_chip_flash28i
    Read a value from Data Bus.
    @return Data readed from bus. */
DLLEXPORT uint16_t GetDataBus(void);

/** @ingroup lib_chip_flash28i
    Set/Reset Serial CS (Chip Select) pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetCS(uint8_t state);

/** @ingroup lib_chip_flash28i
    Set/Reset Serial Clock pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetClk(uint8_t state);

/** @ingroup lib_chip_flash28i
    Set/Reset Serial Data pin.
    @param[in] state If different of zero (default), set pin; else, clear pin
 */
DLLEXPORT void SerialSetData(uint8_t state);

/** @ingroup lib_chip_flash28i
    Read Serial Data pin current state (1 or 0) from chip.
    @return Pin level (high or low). */
DLLEXPORT uint8_t SerialGetData(void);

/** @ingroup lib_chip_flash28i
    Library Entry point for MS Windows.
    @param hInst Library Instance
    @param reason Reason of the call
    @param reserved Not used
    @return TRUE if success, FALSE if error. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

// ---------------------------------------------------------------------------
/* internal classes */

/** @ingroup chip
    @brief   Flash Intel 28Fxxx Chip Emulator Class.
    @details Emulates a Flash Intel 28Fxxx Chip. */
class ChipFlashIntel28F : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipFlashIntel28F();
    /** Destructor. */
    virtual ~ChipFlashIntel28F();
    /* reimplemented */
    virtual void SetSize(uint32_t size);
    /* reimplemented */
    virtual uint16_t GetDataBus(void);

  protected:
    /* stores the step of current special command */
    int f_commandStep;
    /* stores what is the special command */
    TChipCommandOperation f_operation;
    /* emulates the chip */
    virtual void EmuChip(void);
    /* check if received special command */
    virtual bool SpecialCommand(void);
    /* update the data bus (if necessary) */
    virtual bool UpdateDataBus(bool readMemory = true);
};

#endif  // CHIP_FLASH_28I_HPP_
