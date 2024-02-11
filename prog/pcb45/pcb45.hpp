// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_prog_pcb45
 * @file    pcb45.hpp
 * @brief   Header file (C++) for Sivava PCB 4.5 Programmer Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef PROG_PCB45_HPP_  // NOLINT
#define PROG_PCB45_HPP_

// ---------------------------------------------------------------------------
/* headers */

#define NOGDI
#include <windows.h>
#include "baseprog.hpp"

// ---------------------------------------------------------------------------
/* defines */
#define DLLEXPORT extern "C" __stdcall __declspec(dllexport)
#define DLLIMPORT __stdcall

// ---------------------------------------------------------------------------
/* exported functions */

/** @ingroup lib_prog_pcb45
    Wrapper for a hardware port write byte operation.
    @param addr Port Address
    @param data Writed data byte
  */
DLLEXPORT void SetPort(uint16_t addr, uint8_t data);  // NOLINT

/** @ingroup lib_prog_pcb45
    Wrapper for a hardware port read byte operation.
    @param addr Port Address
    @return Data byte required by read operation. */
DLLEXPORT uint8_t GetPort(uint16_t addr);  // NOLINT

/** @ingroup lib_prog_pcb45
    Library Entry point for MS Windows. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

// ---------------------------------------------------------------------------
/* internal classes */

/** @ingroup prog
    @brief   Sivava PCB4.5 Programmer Emulator Class.
    @details Emulates the Parallel Port PC Sivava PCB4.5 programmers:<br>
      - Sivava PCB4.5
      - Sivava PCB5.0 */
class SivavaPCB45Prog : public BaseProg {
  public:
    /** Default Constructor. */
    SivavaPCB45Prog();
    /** Destructor. */
    virtual ~SivavaPCB45Prog();

  protected:
    /* emulates the programmer */
    /* data is a byte from/to register */
    virtual void EmuCtrlPort(uint8_t data);
    virtual void EmuDataPort(uint8_t data);
};

#endif  // PROG_PCB45_HPP_
