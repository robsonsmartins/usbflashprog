// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_prog_willem
 * @file    willem.hpp
 * @brief   Header file (C++) for Willem/Sivava Programmer Emulation Library
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef PROG_WILLEM_HPP_  // NOLINT
#define PROG_WILLEM_HPP_

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

/** @ingroup lib_prog_willem
    Wrapper for a hardware port write byte operation.
    @param addr Port Address
    @param data Writed data byte
  */
DLLEXPORT void SetPort(unsigned short int addr, unsigned char data);  // NOLINT

/** @ingroup lib_prog_willem
    Wrapper for a hardware port read byte operation.
    @param addr Port Address
    @return Data byte required by read operation. */
DLLEXPORT unsigned char GetPort(unsigned short int addr);  // NOLINT

/** @ingroup lib_prog_willem
    Library Entry point for MS Windows. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

// ---------------------------------------------------------------------------
/* internal classes */

/** @ingroup prog
    @brief   Willem/Sivava/EzoFlash+ Programmers Emulator Class.
    @details Emulates the Parallel Port PC Willem Standard programmers:<br>
             - Willem Standard 3.1, 4.0, 4.1 and 4.5
             - Sivava PCB3B
             - EzoFlash+ 4.4 */
class WillemProg : public BaseProg {
  public:
    /** Default Constructor. */
    WillemProg();
    /** Destructor. */
    virtual ~WillemProg();

  protected:
    /* emulates the programmer */
    /* data is a byte from/to register */
    virtual void EmuCtrlPort(unsigned char data);
    virtual void EmuDataPort(unsigned char data);
};

#endif  // PROG_WILLEM_HPP_
