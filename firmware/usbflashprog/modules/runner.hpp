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
#include "modules/bus.hpp"
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
    VGenConfig vgenConfig_;
    /* @brief Control Bus instance. */
    CtrlBus ctrlBus_;
    /* @brief Control Bus configuration. */
    CtrlBusConfig ctrlBusConfig_;
    /* @brief Data Bus instance. */
    DataBus dataBus_;
    /* @brief Address Bus instance. */
    AddrBus addrBus_;
    /* @brief Data Bus configuration. */
    DataBusConfig dataBusConfig_;
    /* @brief Address Bus configuration. */
    AddrBusConfig addrBusConfig_;
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
     * @brief Gets the parameters as float point.
     * @details The first parameter is the integer part of the number.
     *   The second is the frational part. Both are hexadecimal strings.
     * @return Parameters as float point.
     */
    float getParamAsFloat_();
    /*
     * @brief Gets the first parameter as byte.
     * @return Parameter as byte.
     */
    uint8_t getParamAsByte_();
    /*
     * @brief Gets two parameters as word.
     * @details The first parameter is the MSB. The second is the LSB.
     * @return Parameters as word.
     */
    u_int16_t getParamAsWord_();
    /*
     * @brief Gets three/four parameters as double word.
     * @details The first parameter is the MSB. The last is the LSB.
     * @return Parameters as double word.
     */
    u_int32_t getParamAsDWord_();
    /*
     * @brief Creates two parameters from a float point.
     * @param response Pointer to response bytes.
     * @param src Source value.
     */
    void createParamsFromFloat_(TByteArray *response, float src);
    /*
     * @brief Creates parameter from a byte.
     * @param response Pointer to response bytes.
     * @param src Source value.
     */
    void createParamsFromByte_(TByteArray *response, uint8_t src);
    /*
     * @brief Creates two parameters from a word.
     * @param response Pointer to response bytes.
     * @param src Source value.
     */
    void createParamsFromWord_(TByteArray *response, u_int16_t src);
    /*
     * @brief Creates three/four parameters from a double word.
     * @param response Pointer to response bytes.
     * @param src Source value.
     */
    void createParamsFromDWord_(TByteArray *response, u_int32_t src);
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
    /*
     * @brief Runs the received command, if it's a Ctrl Bus opcode.
     * @param opcode Opcode of the command.
     */
    void runCtrlBusCommand_(uint8_t opcode);
    /*
     * @brief Runs the received command, if it's a Data Bus opcode.
     * @param opcode Opcode of the command.
     */
    void runDataBusCommand_(uint8_t opcode);
    /*
     * @brief Runs the received command, if it's a Address Bus opcode.
     * @param opcode Opcode of the command.
     */
    void runAddrBusCommand_(uint8_t opcode);
};

#endif  // MODULES_RUNNER_HPP_
