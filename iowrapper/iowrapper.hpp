// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup lib_iowrapper
 * @file    iowrapper.hpp
 * @brief   Header file (C++) for IO.DLL Wrapper
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef IOWRAPPER_HPP_  // NOLINT
#define IOWRAPPER_HPP_

// ---------------------------------------------------------------------------
/* headers */
#include <windows.h>
#include <cstdio>
#include <cstdlib>

// ---------------------------------------------------------------------------
/* defines */
#define DLLEXPORT extern "C" __stdcall __declspec(dllexport)
#define DLLIMPORT __stdcall

// ---------------------------------------------------------------------------
/* exported functions */
/* emulate the io.dll functions */

/** @ingroup lib_iowrapper
    Write a byte (8 bits) to a port.
    @param[in] Port Port Address
    @param[in] Data Byte to write */
DLLEXPORT void PortOut(short int Port, char Data);  // NOLINT

/** @ingroup lib_iowrapper
    Write a word (16 bits) to a port.
    @param[in] Port Port Address
    @param[in] Data Word to write */
DLLEXPORT void PortWordOut(short int Port, short int Data);  // NOLINT

/** @ingroup lib_iowrapper
    Write a dword (32 bits) to a port.
    @param[in] Port Port Address
    @param[in] Data DWord to write */
DLLEXPORT void PortDWordOut(short int Port, int Data);  // NOLINT

/** @ingroup lib_iowrapper
    Read a byte (8 bits) from a port.
    @param[in] Port Port Address
    @return Byte readed from port. */
DLLEXPORT char PortIn(short int Port);  // NOLINT

/** @ingroup lib_iowrapper
    Read a word (16 bits) from a port.
    @param[in] Port Port Address
    @return Word readed from port. */
DLLEXPORT short int PortWordIn(short int Port);  // NOLINT

/** @ingroup lib_iowrapper
    Read a dword (32 bits) from a port.
    @param[in] Port Port Address
    @return DWord readed from port. */
DLLEXPORT int PortDWordIn(short int Port);  // NOLINT

/** @ingroup lib_iowrapper
    Set a port bit.
    @param[in] Port Port Address
    @param[in] Bit Bit number to set (0..7) */
DLLEXPORT void SetPortBit(short int Port, char Bit);  // NOLINT

/** @ingroup lib_iowrapper
    Clear a port bit.
    @param[in] Port Port Address
    @param[in] Bit Bit number to reset (0..7) */
DLLEXPORT void ClrPortBit(short int Port, char Bit);  // NOLINT

/** @ingroup lib_iowrapper
    Invert a port bit.
    @param[in] Port Port Address
    @param[in] Bit Bit number to invert (0..7) */
DLLEXPORT void NotPortBit(short int Port, char Bit);  // NOLINT

/** @ingroup lib_iowrapper
    Read a bit from port.
    @param[in] Port Port Address
    @param[in] Bit Bit number to get (0..7)
    @return Bit value (1 or 0). */
DLLEXPORT short int GetPortBit(short int Port, char Bit);  // NOLINT

/** @ingroup lib_iowrapper
    Right Shift the data in a port.
    @param[in] Port Port Address
    @param[in] Val Number of bits to shift
    @return Port shifted byte value. */
DLLEXPORT short int RightPortShift(short int Port, short int Val);  // NOLINT

/** @ingroup lib_iowrapper
    Left Shift the data in a port.
    @param[in] Port Port Address
    @param[in] Val Number of bits to shift
    @return Port shifted byte value. */
DLLEXPORT short int LeftPortShift(short int Port, short int Val);  // NOLINT

/** @ingroup lib_iowrapper
    Verify the SYS driver is installed.
    @return TRUE if driver is installed, FALSE otherwise. */
DLLEXPORT short int IsDriverInstalled(void);  // NOLINT

// ---------------------------------------------------------------------------

/** @ingroup lib_iowrapper
    Library Entry point for Micrososft Windows.
    @param[in] hInst Library Instance
    @param[in] reason Reason of the call
    @param[in] reserved Not used
    @return TRUE if success, FALSE if error. */
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved);

#endif  // IOWRAPPER_HPP_    // NOLINT
