// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup prog
 * @file    baseprog.hpp
 * @brief   Programmer Emulation Base Classes (C++ header file)
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef PROG_BASE_HPP_  // NOLINT
#define PROG_BASE_HPP_

// ---------------------------------------------------------------------------
/** @ingroup prog
    @brief   Programmer Emulator Base Abstract Class.
    @details This is a base class for Parallel Port Programmer Emulator.<br>
    Provides all abstract methods need for a programmer emulator, connected at
    PC standard Parallel Port. */
class BaseProg {
 public:
    /** Default Constructor. */
    BaseProg();
    /** Destructor. */
    virtual ~BaseProg();
    /** Write byte into port by address.
        @param[in] addr I/O Port Address
        @param[in] data Data to write
        @return None
    */
    virtual void SetPort(unsigned short int addr, unsigned char data);  // NOLINT
    /** Read byte from port by address.
        @param[in] addr I/O Port Address
        @return Readed Data. */
    virtual unsigned char GetPort(unsigned short int addr);  // NOLINT

 protected:
    /* parallel port mirror registers */
    unsigned char f_data_port;
    unsigned char f_status_port;
    unsigned char f_control_port;
    /* address bus serial shift register */
    unsigned long f_addr_serial_sr;
    /* data bus serial shift register */
    unsigned char f_data_serial_sr;
    /** Emulates hardware Control Port response.
        @param[in] data Data written to Control Port register
        @return None
    */
    virtual void EmuCtrlPort(unsigned char data) = 0;
    /** Emulates hardware Data Port response.
        @param[in] data Data written to Data Port register
        @return None
    */
    virtual void EmuDataPort(unsigned char data) = 0;
};

#endif  // PROG_BASE_HPP_
