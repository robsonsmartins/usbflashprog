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

#include "hal/multicore.hpp"
#include "modules/vgenerator.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a UsbFlashProg class.
 */
typedef struct UsbFlashProgConfig {
    /** @brief VppGenerator configuration. */
    VppConfig vpp;
    /** @brief VddGenerator configuration. */
    VddConfig vdd;
} UsbFlashProgConfig;

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
    /**
     * @brief Constructor.
     * @param entry Second CPU core entry point routine.
     */
    explicit UsbFlashProg(MultiCore::MultiCoreEntry entry);
    /** @brief Destructor. */
    ~UsbFlashProg();
    /**
     * @brief Initializes the system.
     * @details This method must be called at begin of the main routine
     *  (by first CPU core).
     */
    void init();
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    UsbFlashProgConfig getConfig() const;
    /**
     * @brief Routine to be called by first CPU core.
     * @details This method must be called periodically in a loop,
     *  by first CPU core.
     */
    void firstCore(void);
    /**
     * @brief Routine to be called by second CPU core.
     * @details This method must be called periodically in a loop,
     *  by second CPU core.
     * @param core Reference to MultiCore object.
     */
    void secondCore(MultiCore& core); // NOLINT

 private:
    /* @brief Multicore manager. */
    MultiCore multicore_;
    /* @brief VppGenerator instance. */
    VppGenerator vpp_;
    /* @brief VddGenerator instance. */
    VddGenerator vdd_;
    /* @brief Configuration data. */
    UsbFlashProgConfig config_;
};

#endif  // USBFLASHPROG_HPP_
