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
 * @file modules/opcodes.hpp
 * @brief Opcodes for communication.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MODULES_OPCODES_HPP_
#define MODULES_OPCODES_HPP_

// ---------------------------------------------------------------------------

#include <string>
#include <map>

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines an opcode to run.
 * @details The purpose of this structure is to define the properties
 *  of an opcode.
 */
typedef struct TCmdOpCode {
    /** @brief Opcode number. */
    uint8_t opcode;
    /** @brief Opcode description. */
    std::string descr;
    /** @brief Number of bytes of the required parameters. */
    uint8_t params;
    /** @brief Number of bytes of the response. */
    uint8_t result;
} TCmdOpCode;

/** @brief Type of opcodes group (map). */
typedef std::map<std::string, TCmdOpCode> TCmdOpCodeMap;

// ---------------------------------------------------------------------------

/** @brief CMD / PARAM : Defines a param with value OFF. */
constexpr const char* kCmdParamOff    = "00";
/** @brief CMD / PARAM : Defines a param with value ON. */
constexpr const char* kCmdParamOn     = "01";

// ---------------------------------------------------------------------------

/** @brief CMD / RESPONSE : Defines a response with value NOK. */
constexpr const char* kCmdResponseNok = "00";
/** @brief CMD / RESPONSE : Defines a response with value OK. */
constexpr const char* kCmdResponseOk  = "01";

// ---------------------------------------------------------------------------

/** @brief OPCODE / VDD : Opcode VDD Control On/Off. */
constexpr uint8_t kCmdVddCtrl    = 0x01;
/** @brief OPCODE / VDD : Opcode VDD Set Voltage. */
constexpr uint8_t kCmdVddSetV    = 0x02;
/** @brief OPCODE / VDD : Opcode VDD Get Voltage. */
constexpr uint8_t kCmdVddGetV    = 0x03;
/** @brief OPCODE / VDD : Opcode VDD Get PWM Duty Cycle. */
constexpr uint8_t kCmdVddGetDuty = 0x04;
/** @brief OPCODE / VDD : Opcode VDD Get Calibration. */
constexpr uint8_t kCmdVddGetCal  = 0x05;
/** @brief OPCODE / VDD : Opcode VDD Init Calibration. */
constexpr uint8_t kCmdVddInitCal = 0x06;
/** @brief OPCODE / VDD : Opcode VDD Save Calibration. */
constexpr uint8_t kCmdVddSaveCal = 0x07;
/** @brief OPCODE / VDD : Opcode VDD on VPP. */
constexpr uint8_t kCmdVddOnVpp   = 0x08;

/** @brief OPCODE / VPP : Opcode VPP Control On/Off. */
constexpr uint8_t kCmdVppCtrl    = 0x11;
/** @brief OPCODE / VPP : Opcode VPP Set Voltage. */
constexpr uint8_t kCmdVppSetV    = 0x12;
/** @brief OPCODE / VPP : Opcode VPP Get Voltage. */
constexpr uint8_t kCmdVppGetV    = 0x13;
/** @brief OPCODE / VPP : Opcode VPP Get PWM Duty Cycle. */
constexpr uint8_t kCmdVppGetDuty = 0x14;
/** @brief OPCODE / VPP : Opcode VPP Get Calibration. */
constexpr uint8_t kCmdVppGetCal  = 0x15;
/** @brief OPCODE / VPP : Opcode VPP Init Calibration. */
constexpr uint8_t kCmdVppInitCal = 0x16;
/** @brief OPCODE / VPP : Opcode VPP Save Calibration. */
constexpr uint8_t kCmdVppSaveCal = 0x17;
/** @brief OPCODE / VPP : Opcode VPP on A9. */
constexpr uint8_t kCmdVppOnA9    = 0x18;
/** @brief OPCODE / VPP : Opcode VPP on A18. */
constexpr uint8_t kCmdVppOnA18   = 0x19;
/** @brief OPCODE / VPP : Opcode VPP on CE. */
constexpr uint8_t kCmdVppOnCE    = 0x1A;
/** @brief OPCODE / VPP : Opcode VPP on OE. */
constexpr uint8_t kCmdVppOnOE    = 0x1B;
/** @brief OPCODE / VPP : Opcode VPP on WE. */
constexpr uint8_t kCmdVppOnWE    = 0x1C;

// ---------------------------------------------------------------------------

/** @brief OPCODE : Opcodes group (map). */
static const TCmdOpCodeMap kCmdOpCodes = {
    { "01", { kCmdVddCtrl   , "VddCtrl"   , 1, 0} },  // Off = 00 | On = 01
    { "02", { kCmdVddSetV   , "VddSetV"   , 2, 0} },  // <XX.YY> format
    { "03", { kCmdVddGetV   , "VddGetV"   , 0, 2} },  // None
    { "04", { kCmdVddGetDuty, "VddGetDuty", 0, 2} },  // None
    { "05", { kCmdVddGetCal , "VddGetCal" , 0, 2} },  // None
    { "06", { kCmdVddInitCal, "VddInitCal", 0, 0} },  // None
    { "07", { kCmdVddSaveCal, "VddSaveCal", 2, 0} },  // <XX.YY> format
    { "08", { kCmdVddOnVpp  , "VddOnVpp"  , 1, 0} },  // Off = 00 | On = 01

    { "11", { kCmdVppCtrl   , "VppCtrl"   , 1, 0} },  // Off = 00 | On = 01
    { "12", { kCmdVppSetV   , "VppSetV"   , 2, 0} },  // <XX.YY> format
    { "13", { kCmdVppGetV   , "VppGetV"   , 0, 2} },  // None
    { "14", { kCmdVppGetDuty, "VppGetDuty", 0, 2} },  // None
    { "15", { kCmdVppGetCal , "VppGetCal" , 0, 2} },  // None
    { "16", { kCmdVppInitCal, "VppInitCal", 0, 0} },  // None
    { "17", { kCmdVppSaveCal, "VppSaveCal", 2, 0} },  // <XX.YY> format
    { "18", { kCmdVppOnA9   , "VppOnA9"   , 1, 0} },  // Off = 00 | On = 01
    { "19", { kCmdVppOnA18  , "VppOnA18"  , 1, 0} },  // Off = 00 | On = 01
    { "1A", { kCmdVppOnCE   , "VppOnCE"   , 1, 0} },  // Off = 00 | On = 01
    { "1B", { kCmdVppOnOE   , "VppOnOE"   , 1, 0} },  // Off = 00 | On = 01
    { "1C", { kCmdVppOnWE   , "VppOnWE"   , 1, 0} }   // Off = 00 | On = 01
};

#endif  // MODULES_OPCODES_HPP_
