// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup Firmware
 * @file usbflashprog.hpp
 * @brief Header of the UsbFlashProg Main Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef USBFLASHPROG_HPP_
#define USBFLASHPROG_HPP_

#include "modules/vgenerator.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Main class of system.
 * @details The purpose of this class is to provide the main methods to run
 *  the firmware.
 * @nosubgrouping
 */
class UsbFlashProg {
 public:
    /** @brief Constructor. */
    UsbFlashProg();
    /** @brief Destructor. */
    ~UsbFlashProg();
    /**
     * @brief Initializes the system.
     * @details This method must be called at begin of the main routine
     *  (by first CPU core).
     */
    void init();
    /**
     * @brief Routine to be called by first CPU core.
     * @details This method must be called periodically in a loop,
     *  by first CPU core.
     */
    void firstCore(void);

 private:
    /* @brief VGenerator instance. */
    VGenerator vgen_;
    /* @brief VGenerator configuration. */
    VGenConfig config_;
};

#endif  // USBFLASHPROG_HPP_
