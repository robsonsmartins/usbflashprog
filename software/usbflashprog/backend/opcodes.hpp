// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup Software
 * @file backend/opcodes.hpp
 * @brief Opcodes for communication.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_OPCODES_HPP_
#define BACKEND_OPCODES_HPP_

// ---------------------------------------------------------------------------

#include <cstdint>
#include <string>
#include <map>

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Enumeration of the param values.
 */
enum kCmdParamValueEnum {
    /** @brief CMD / PARAM : Defines a param with value OFF. */
    kCmdParamOff = 0x00,
    /** @brief CMD / PARAM : Defines a param with value ON. */
    kCmdParamOn  = 0x01
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Enumeration of the response values.
 */
enum kCmdResponseValueEnum {
    /** @brief CMD / RESPONSE : Defines a response with value NOK. */
    kCmdResponseNok = 0xA0,
    /** @brief CMD / RESPONSE : Defines a response with value OK. */
    kCmdResponseOk  = 0xA1
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Enumeration of the OpCodes.
 */
enum kCmdOpCodeEnum {
    /** @brief OPCODE / NOP : Opcode NOP. */
    kCmdNop        = 0x00,

    /** @brief OPCODE / VDD : Opcode VDD Control On/Off. */
    kCmdVddCtrl    = 0x01,
    /** @brief OPCODE / VDD : Opcode VDD Set Voltage. */
    kCmdVddSetV    = 0x02,
    /** @brief OPCODE / VDD : Opcode VDD Get Voltage. */
    kCmdVddGetV    = 0x03,
    /** @brief OPCODE / VDD : Opcode VDD Get PWM Duty Cycle. */
    kCmdVddGetDuty = 0x04,
    /** @brief OPCODE / VDD : Opcode VDD Get Calibration. */
    kCmdVddGetCal  = 0x05,
    /** @brief OPCODE / VDD : Opcode VDD Init Calibration. */
    kCmdVddInitCal = 0x06,
    /** @brief OPCODE / VDD : Opcode VDD Save Calibration. */
    kCmdVddSaveCal = 0x07,
    /** @brief OPCODE / VDD : Opcode VDD on VPP. */
    kCmdVddOnVpp   = 0x08,

    /** @brief OPCODE / VPP : Opcode VPP Control On/Off. */
    kCmdVppCtrl    = 0x11,
    /** @brief OPCODE / VPP : Opcode VPP Set Voltage. */
    kCmdVppSetV    = 0x12,
    /** @brief OPCODE / VPP : Opcode VPP Get Voltage. */
    kCmdVppGetV    = 0x13,
    /** @brief OPCODE / VPP : Opcode VPP Get PWM Duty Cycle. */
    kCmdVppGetDuty = 0x14,
    /** @brief OPCODE / VPP : Opcode VPP Get Calibration. */
    kCmdVppGetCal  = 0x15,
    /** @brief OPCODE / VPP : Opcode VPP Init Calibration. */
    kCmdVppInitCal = 0x16,
    /** @brief OPCODE / VPP : Opcode VPP Save Calibration. */
    kCmdVppSaveCal = 0x17,
    /** @brief OPCODE / VPP : Opcode VPP on A9. */
    kCmdVppOnA9    = 0x18,
    /** @brief OPCODE / VPP : Opcode VPP on A18. */
    kCmdVppOnA18   = 0x19,
    /** @brief OPCODE / VPP : Opcode VPP on CE. */
    kCmdVppOnCE    = 0x1A,
    /** @brief OPCODE / VPP : Opcode VPP on OE. */
    kCmdVppOnOE    = 0x1B,
    /** @brief OPCODE / VPP : Opcode VPP on WE. */
    kCmdVppOnWE    = 0x1C,

    /** @brief OPCODE / BUS : Opcode CE Ctrl. */
    kCmdBusCE      = 0x21,
    /** @brief OPCODE / BUS : Opcode OE Ctrl. */
    kCmdBusOE      = 0x22,
    /** @brief OPCODE / BUS : Opcode WE Ctrl. */
    kCmdBusWE      = 0x23,

    /** @brief OPCODE / BUS : Opcode Address Clear. */
    kCmdBusAddrClr  = 0x31,
    /** @brief OPCODE / BUS : Opcode Address Increment. */
    kCmdBusAddrInc  = 0x32,
    /** @brief OPCODE / BUS : Opcode Address Set. */
    kCmdBusAddrSet  = 0x33,
    /** @brief OPCODE / BUS : Opcode Address Set Byte. */
    kCmdBusAddrSetB = 0x34,
    /** @brief OPCODE / BUS : Opcode Address Set Word. */
    kCmdBusAddrSetW = 0x35,

    /** @brief OPCODE / BUS : Opcode Data Clear. */
    kCmdBusDataClr  = 0x41,
    /** @brief OPCODE / BUS : Opcode Data Set. */
    kCmdBusDataSet  = 0x42,
    /** @brief OPCODE / BUS : Opcode Data Set Byte. */
    kCmdBusDataSetB = 0x43,
    /** @brief OPCODE / BUS : Opcode Data Get. */
    kCmdBusDataGet  = 0x44,
    /** @brief OPCODE / BUS : Opcode Data Get Byte. */
    kCmdBusDataGetB = 0x45
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Defines an opcode to run.
 * @details The purpose of this structure is to define the properties
 *  of an opcode.
 */
typedef struct TCmdOpCode {
    /** @brief OpCode. */
    kCmdOpCodeEnum code;
    /** @brief Opcode description. */
    std::string descr;
    /** @brief Number of bytes of the required parameters. */
    uint8_t params;
    /** @brief Number of bytes of the response. */
    uint8_t result;
    /**
     * @brief Assign Operator.
     * @param src TCmdOpCode source object.
     * @return Reference for this.
     */
    TCmdOpCode& operator=(const TCmdOpCode& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const TCmdOpCode& a, const TCmdOpCode& b);
} TCmdOpCode;

// ---------------------------------------------------------------------------

/** @brief Type of opcodes group (map). */
typedef std::map<kCmdOpCodeEnum, TCmdOpCode> TCmdOpCodeMap;

// ---------------------------------------------------------------------------

/** @brief OPCODE : Opcodes group (map). */
static const TCmdOpCodeMap kCmdOpCodes = {
    { kCmdNop       , { kCmdNop       , "Nop"       , 0, 0} },

    { kCmdVddCtrl   , { kCmdVddCtrl   , "VddCtrl"   , 1, 0} },
    { kCmdVddSetV   , { kCmdVddSetV   , "VddSetV"   , 2, 0} },
    { kCmdVddGetV   , { kCmdVddGetV   , "VddGetV"   , 0, 2} },
    { kCmdVddGetDuty, { kCmdVddGetDuty, "VddGetDuty", 0, 2} },
    { kCmdVddGetCal , { kCmdVddGetCal , "VddGetCal" , 0, 2} },
    { kCmdVddInitCal, { kCmdVddInitCal, "VddInitCal", 0, 0} },
    { kCmdVddSaveCal, { kCmdVddSaveCal, "VddSaveCal", 2, 0} },
    { kCmdVddOnVpp  , { kCmdVddOnVpp  , "VddOnVpp"  , 1, 0} },

    { kCmdVppCtrl   , { kCmdVppCtrl   , "VppCtrl"   , 1, 0} },
    { kCmdVppSetV   , { kCmdVppSetV   , "VppSetV"   , 2, 0} },
    { kCmdVppGetV   , { kCmdVppGetV   , "VppGetV"   , 0, 2} },
    { kCmdVppGetDuty, { kCmdVppGetDuty, "VppGetDuty", 0, 2} },
    { kCmdVppGetCal , { kCmdVppGetCal , "VppGetCal" , 0, 2} },
    { kCmdVppInitCal, { kCmdVppInitCal, "VppInitCal", 0, 0} },
    { kCmdVppSaveCal, { kCmdVppSaveCal, "VppSaveCal", 2, 0} },
    { kCmdVppOnA9   , { kCmdVppOnA9   , "VppOnA9"   , 1, 0} },
    { kCmdVppOnA18  , { kCmdVppOnA18  , "VppOnA18"  , 1, 0} },
    { kCmdVppOnCE   , { kCmdVppOnCE   , "VppOnCE"   , 1, 0} },
    { kCmdVppOnOE   , { kCmdVppOnOE   , "VppOnOE"   , 1, 0} },
    { kCmdVppOnWE   , { kCmdVppOnWE   , "VppOnWE"   , 1, 0} },

    { kCmdBusCE     , { kCmdBusCE     , "CE"        , 1, 0} },
    { kCmdBusOE     , { kCmdBusOE     , "OE"        , 1, 0} },
    { kCmdBusWE     , { kCmdBusWE     , "WE"        , 1, 0} },

    { kCmdBusAddrClr , { kCmdBusAddrClr , "AddrClr"    , 0, 0} },
    { kCmdBusAddrInc , { kCmdBusAddrInc , "AddrInc"    , 0, 0} },
    { kCmdBusAddrSet , { kCmdBusAddrSet , "AddrSet"    , 3, 0} },
    { kCmdBusAddrSetB, { kCmdBusAddrSetB, "AddrSetByte", 1, 0} },
    { kCmdBusAddrSetW, { kCmdBusAddrSetW, "AddrSetWord", 2, 0} },

    { kCmdBusDataClr , { kCmdBusDataClr , "DataClr"    , 0, 0} },
    { kCmdBusDataSet , { kCmdBusDataSet , "DataSet"    , 2, 0} },
    { kCmdBusDataSetB, { kCmdBusDataSetB, "DataSetByte", 1, 0} },
    { kCmdBusDataGet , { kCmdBusDataGet , "DataGet"    , 0, 2} },
    { kCmdBusDataGetB, { kCmdBusDataGetB, "DataGetByte", 0, 1} }
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Opcode Manager Class
 * @details The purpose of this class is to handle the opcodes of the
 *  communication (via USB-CDC).
 * @nosubgrouping
 */
class OpCode {
 public:
    /**
     * @brief Checks if the return code was OK.
     * @param buf Pointer to the buffer that contains the result
     *  of the communication.
     * @param size Size of buffer, in bytes.
     * @return True if return code is OK, false otherwise. 
     */
    static bool isOk(const void *buf, size_t size);
    /**
     * @brief Gets the opcode from communication frame.
     * @param buf Pointer to the buffer that contains the 
     *  communication frame.
     * @param size Size of buffer, in bytes.
     * @return Opcode of the operation, or kCmdNop if not found.
     */
    static TCmdOpCode getOpCode(const void *buf, size_t size);
    /**
     * @overload
     * @brief Gets the opcode from code (byte).
     * @param code Code (byte).
     * @return Opcode of the operation, or kCmdNop if not found.
     */
    static TCmdOpCode getOpCode(uint8_t code);
    /**
     * @brief Gets the param value as float.
     * @param buf Pointer to the buffer that contains the result
     *  of the communication.
     * @param size Size of buffer, in bytes.
     * @return Value of parameter as float, or 0.0f if an error occurs.
     */
    static float getValueAsFloat(const void *buf, size_t size);
    /**
     * @brief Gets the param value as byte.
     * @param buf Pointer to the buffer that contains the result
     *  of the communication.
     * @param size Size of buffer, in bytes.
     * @return Value of parameter as byte, or 0 if an error occurs.
     */
    static uint8_t getValueAsByte(const void *buf, size_t size);
    /**
     * @brief Gets the param value as bool.
     * @param buf Pointer to the buffer that contains the result
     *  of the communication.
     * @param size Size of buffer, in bytes.
     * @return Value of parameter as bool, or false if an error occurs.
     */
    static bool getValueAsBool(const void *buf, size_t size);
    /**
     * @brief Gets the param value as word.
     * @param buf Pointer to the buffer that contains the result
     *  of the communication.
     * @param size Size of buffer, in bytes.
     * @return Value of parameter as word, or 0 if an error occurs.
     */
    static uint16_t getValueAsWord(const void *buf, size_t size);
    /**
     * @brief Gets the param value as double word.
     * @param buf Pointer to the buffer that contains the result
     *  of the communication.
     * @param size Size of buffer, in bytes.
     * @return Value of parameter as double word, or 0 if an error occurs.
     */
    static uint32_t getValueAsDWord(const void *buf, size_t size);
    /**
     * @brief Sets the param value into the communication frame.
     * @param buf Pointer to the buffer that contains the 
     *  communication frame.
     * @param size Size of buffer, in bytes.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    static bool setFloat(void *buf, size_t size, float value);
    /**
     * @brief Sets the param value into the communication frame.
     * @param buf Pointer to the buffer that contains the 
     *  communication frame.
     * @param size Size of buffer, in bytes.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    static bool setByte(void *buf, size_t size, uint8_t value);
    /**
     * @brief Sets the param value into the communication frame.
     * @param buf Pointer to the buffer that contains the 
     *  communication frame.
     * @param size Size of buffer, in bytes.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    static bool setWord(void *buf, size_t size, uint16_t value);
    /**
     * @brief Sets the param value into the communication frame.
     * @param buf Pointer to the buffer that contains the 
     *  communication frame.
     * @param size Size of buffer, in bytes.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    static bool setDWord(void *buf, size_t size, uint32_t value);
    /**
     * @brief Sets the param value into the communication frame.
     * @param buf Pointer to the buffer that contains the 
     *  communication frame.
     * @param size Size of buffer, in bytes.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    static bool setBool(void *buf, size_t size, bool value);
};

#endif  // BACKEND_OPCODES_HPP_
