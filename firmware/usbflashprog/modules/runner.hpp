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
 * @file modules/runner.hpp
 * @brief Header of the Communication Runner Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MODULES_RUNNER_HPP_
#define MODULES_RUNNER_HPP_

#include <string>
#include <vector>
#include "hal/serial.hpp"
#include "hal/gpio.hpp"
#include "modules/vgenerator.hpp"
#include "modules/opcodes.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Communication Runner Class
 * @details The purpose of this class is to handle the communication
 *  (via USB-CDC) and run commands.
 * @nosubgrouping
 */
class Runner {
 public:
    /** @brief Constructor. */
    Runner();
    /** @brief Starts the runner. */
    void init();
    /** 
     * @brief Main loop method.
     * @details This method must be called periodically in a loop,
     *  so that the Runner works properly.
     */
    void loop();

 private:
    /* @brief Type of byte array. */
    typedef std::vector<uint8_t> TByteArray;
    /* @brief VGenerator instance. */
    VGenerator vgen_;
    /* @brief VGenerator configuration. */
    VGenConfig config_;
    /* @brief Gpio Handler instance. */
    Gpio gpio_;
    /* @brief Serial Comm instance. */
    Serial serial_;
    /* @brief Received command. */
    TByteArray command_;
    /*
     * @brief Reads bytes from serial.
     * @param len Number of bytes (default is one).
     * @return Byte array.
     */
    TByteArray readByte_(size_t len = 1);
    /*
     * @brief Finds the opcode into the command string.
     * @return Constant iterator to opcode map. 
     *  TCmdOpCodeMap::end() if not found.
     */
    TCmdOpCodeMap::const_iterator findOpCode_();
    /*
     * @brief Gets the first parameter as boolean.
     * @return Parameter as boolean.
     */
    bool getParamAsBool_();
    /*
     * @brief Gets two parameters as float point.
     * @details The first parameter is the integer part of the number.
     *   The second is the frational part. Both are hexadecimal strings.
     * @return Parameters as float point.
     */
    float getParamsAsFloat_();
    /*
     * @brief Creates two parameters from a float point.
     * @param response Pointer to response bytes.
     * @param src Source value.
     */
    void createParamsFromFloat_(TByteArray *response, float src);
    /* @brief Runs the received command. */
    void runCommand_();
    /*
     * @brief Runs the received command, if it's a VDD Generator opcode.
     * @param opcode Opcode of the command.
     */
    void runVddCommand_(uint8_t opcode);
    /*
     * @brief Runs the received command, if it's a VPP Generator opcode.
     * @param opcode Opcode of the command.
     */
    void runVppCommand_(uint8_t opcode);
};

#endif  // MODULES_RUNNER_HPP_
