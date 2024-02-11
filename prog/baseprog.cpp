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
 * @file    baseprog.cpp
 * @brief   Programmer Emulation Base Classes (C++ implementation file)
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "baseprog.hpp"

// ---------------------------------------------------------------------------
/* Offset addresses */

/* Data Port */
#define DATA_PORT 0x00
/* Status Port */
#define STATUS_PORT DATA_PORT + 1
/* Control Port */
#define CONTROL_PORT DATA_PORT + 2

/* Base I/O addresses */

/* LPT1 */
#define PORT_BASE_1 0x378
/* LPT2 */
#define PORT_BASE_2 0x278
/* LPT3 */
#define PORT_BASE_3 0x3BC

// ---------------------------------------------------------------------------

BaseProg::BaseProg()
    : f_data_port(0xFB),
      f_status_port(0xFF),
      f_control_port(0xFE),
      f_addr_serial_sr(0xFFFFFFFF),
      f_data_serial_sr(0xFF) {}

BaseProg::~BaseProg() {}

void BaseProg::SetPort(uint16_t addr, uint8_t data) {  // NOLINT
    /* evaluates the address */
    switch (addr) {
        /* data port */
        case PORT_BASE_1 + DATA_PORT:
        case PORT_BASE_2 + DATA_PORT:
        case PORT_BASE_3 + DATA_PORT:
            EmuDataPort(data);
            break;
        /* status port */
        case PORT_BASE_1 + STATUS_PORT:
        case PORT_BASE_2 + STATUS_PORT:
        case PORT_BASE_3 + STATUS_PORT:
            /* write not allowed */
            break;
        /* control port */
        case PORT_BASE_1 + CONTROL_PORT:
        case PORT_BASE_2 + CONTROL_PORT:
        case PORT_BASE_3 + CONTROL_PORT:
            EmuCtrlPort(data);
            break;
        /* address not supported */
        default:
            break;
    }
}

uint8_t BaseProg::GetPort(uint16_t addr) {  // NOLINT
    /* evaluates the address */
    switch (addr) {
        /* data port */
        case PORT_BASE_1 + DATA_PORT:
        case PORT_BASE_2 + DATA_PORT:
        case PORT_BASE_3 + DATA_PORT:
            return f_data_port;
        /* status port */
        case PORT_BASE_1 + STATUS_PORT:
        case PORT_BASE_2 + STATUS_PORT:
        case PORT_BASE_3 + STATUS_PORT:
            return f_status_port;
        /* control port */
        case PORT_BASE_1 + CONTROL_PORT:
        case PORT_BASE_2 + CONTROL_PORT:
        case PORT_BASE_3 + CONTROL_PORT:
            return f_control_port;
        /* address not supported */
        default:
            return 0xFF;
    }
    /* returns */
    return 0xFF;
}
