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

/** @brief Enumeration of the param values. */
enum kCmdParamValueEnum {
    /** @brief CMD / PARAM : Defines a param with value OFF. */
    kCmdParamOff = 0x00,
    /** @brief CMD / PARAM : Defines a param with value ON. */
    kCmdParamOn = 0x01
};

// ---------------------------------------------------------------------------

/** @brief Enumeration of the response values. */
enum kCmdResponseValueEnum {
    /** @brief CMD / RESPONSE : Defines a response with value NOK. */
    kCmdResponseNok = 0xA0,
    /** @brief CMD / RESPONSE : Defines a response with value OK. */
    kCmdResponseOk = 0xA1
};

// ---------------------------------------------------------------------------

/** @brief Enumeration of the OpCodes. */
enum kCmdOpCodeEnum {
    /** @brief OPCODE / NOP : Opcode NOP. */
    kCmdNop = 0x00,

    /**
     * @brief OPCODE / VDD : Opcode VDD Control On/Off.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVddCtrl = 0x01,
    /**
     * @brief OPCODE / VDD : Opcode VDD Set Voltage.
     * @details The parameter (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVddSetV = 0x02,
    /**
     * @brief OPCODE / VDD : Opcode VDD Get Voltage.
     * @details The result (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVddGetV = 0x03,
    /**
     * @brief OPCODE / VDD : Opcode VDD Get PWM Duty Cycle.
     * @details The result (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVddGetDuty = 0x04,
    /**
     * @brief OPCODE / VDD : Opcode VDD Get Calibration.
     * @details The result (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVddGetCal = 0x05,
    /** @brief OPCODE / VDD : Opcode VDD Init Calibration. */
    kCmdVddInitCal = 0x06,
    /**
     * @brief OPCODE / VDD : Opcode VDD Save Calibration.
     * @details The parameter (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVddSaveCal = 0x07,
    /**
     * @brief OPCODE / VDD : Opcode VDD on VPP.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVddOnVpp = 0x08,

    /**
     * @brief OPCODE / VPP : Opcode VPP Control On/Off.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVppCtrl = 0x11,
    /**
     * @brief OPCODE / VPP : Opcode VPP Set Voltage.
     * @details The parameter (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVppSetV = 0x12,
    /**
     * @brief OPCODE / VPP : Opcode VPP Get Voltage.
     * @details The result (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVppGetV = 0x13,
    /**
     * @brief OPCODE / VPP : Opcode VPP Get PWM Duty Cycle.
     * @details The result (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVppGetDuty = 0x14,
    /**
     * @brief OPCODE / VPP : Opcode VPP Get Calibration.
     * @details The result (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVppGetCal = 0x15,
    /** @brief OPCODE / VPP : Opcode VPP Init Calibration. */
    kCmdVppInitCal = 0x16,
    /**
     * @brief OPCODE / VPP : Opcode VPP Save Calibration.
     * @details The parameter (two bytes) represents the value.
     *          MSB is integer part. LSB is fractional part.
     */
    kCmdVppSaveCal = 0x17,
    /**
     * @brief OPCODE / VPP : Opcode VPP on A9.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVppOnA9 = 0x18,
    /**
     * @brief OPCODE / VPP : Opcode VPP on A18.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVppOnA18 = 0x19,
    /**
     * @brief OPCODE / VPP : Opcode VPP on CE.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVppOnCE = 0x1A,
    /**
     * @brief OPCODE / VPP : Opcode VPP on OE.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVppOnOE = 0x1B,
    /**
     * @brief OPCODE / VPP : Opcode VPP on WE.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdVppOnWE = 0x1C,

    /**
     * @brief OPCODE / BUS : Opcode CE Ctrl.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdBusCE = 0x21,
    /**
     * @brief OPCODE / BUS : Opcode OE Ctrl.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdBusOE = 0x22,
    /**
     * @brief OPCODE / BUS : Opcode WE Ctrl.
     * @details The parameter (one byte) indicates on/off.
     * @see kCmdParamValueEnum
     */
    kCmdBusWE = 0x23,

    /** @brief OPCODE / BUS : Opcode Address Clear. */
    kCmdBusAddrClr = 0x31,
    /** @brief OPCODE / BUS : Opcode Address Increment. */
    kCmdBusAddrInc = 0x32,
    /**
     * @brief OPCODE / BUS : Opcode Address Set.
     * @details The parameter (four bytes) represents the
     *          address (MSB up to LSB).
     */
    kCmdBusAddrSet = 0x33,
    /**
     * @brief OPCODE / BUS : Opcode Address Set Byte.
     * @details The parameter (one byte) represents the address.
     */
    kCmdBusAddrSetB = 0x34,
    /**
     * @brief OPCODE / BUS : Opcode Address Set Word.
     * @details The parameter (two bytes) represents the
     *          address (MSB up to LSB).
     */
    kCmdBusAddrSetW = 0x35,

    /** @brief OPCODE / BUS : Opcode Data Clear. */
    kCmdBusDataClr = 0x41,
    /**
     * @brief OPCODE / BUS : Opcode Data Set Byte.
     * @details The parameter (one byte) represents the data.
     */
    kCmdBusDataSet = 0x42,
    /**
     * @brief OPCODE / BUS : Opcode Data Set Word.
     * @details The parameter (two bytes) represents the
     *          data (MSB up to LSB).
     */
    kCmdBusDataSetW = 0x43,
    /**
     * @brief OPCODE / BUS : Opcode Data Get Byte.
     * @details The result (one byte) represents the data.
     */
    kCmdBusDataGet = 0x44,
    /**
     * @brief OPCODE / BUS : Opcode Data Get Word.
     * @details The result (two bytes) represents the
     *          data (MSB up to LSB).
     */
    kCmdBusDataGetW = 0x45,

    /**
     * @brief OPCODE / DEVICE : Opcode Device Set tWP.
     * @details The parameter (four bytes) represents the tWP value
     *   in microseconds.
     */
    kCmdDeviceSetTwp = 0x81,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Set tWC.
     * @details The parameter (four bytes) represents the tWC value
     *   in microseconds.
     */
    kCmdDeviceSetTwc = 0x82,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Configure.
     * @details The first parameter (one byte) represents the device
     *   algorithm.<br/>
     *   The second parameter (one byte) represents the flags, and
     *   follows the table:
     * <pre>
     * +----------------------+
     * |Bit| Description      |
     * | 0 | Skip Write 0xFF  |
     * | 1 | Prog with VPP On |
     * | 2 | VPP/~OE Pin      |
     * | 3 | ~PGM/~CE Pin     |
     * | 4 | PGM positive     |
     * | 5 | 16-bit mode      |
     * +----------------------+
     * </pre>
     * @see kCmdDeviceAlgorithmEnum
     */
    kCmdDeviceConfigure = 0x83,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Setup Bus.
     * @details The parameter (one byte) represents the operation, and
     *   follows the table:
     * <pre>
     * +------------------------------+
     * |Operation| Description        |
     * |  0x00   | Reset Bus          |
     * |  0x01   | Prepare to Read    |
     * |  0x02   | Prepare to Program |
     * +------------------------------+
     * </pre>
     * @see kCmdDeviceOperationEnum
     */
    kCmdDeviceSetupBus = 0x84,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Read Byte/Word buffer
     *   and Increment Address.
     * @details The parameter (one byte) represents the buffer size,
     *   in bytes.<br/>
     *   The return is [size] bytes. MSB first.
     */
    kCmdDeviceRead = 0x85,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Write Byte/Word buffer,
     *   verify and Increment Address.
     * @details The first parameter (one byte) represents the buffer size,
     *   in bytes.<br/>
     *   The second parameter ([size] bytes) is the data to write. MSB first.
     */
    kCmdDeviceWrite = 0x86,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Write Byte/Word sector,
     *   verify and Increment Address.
     * @details The first parameter (two bytes) represents the sector size,
     *   in bytes. The following are data to write (size is specified).
     *   MSB first.
     */
    kCmdDeviceWriteSector = 0x87,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Verify Byte/Word buffer
     *   and Increment Address.
     * @details The first parameter (one byte) represents the buffer size,
     *   in bytes.<br/>
     *   The second parameter ([size] bytes) is the data to verify. MSB first.
     */
    kCmdDeviceVerify = 0x88,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Check if Byte/Word buffer is Blank
     *   and Increment Address.
     * @details The parameter (one byte) represents the buffer size,
     *   in bytes.
     */
    kCmdDeviceBlankCheck = 0x89,
    /**
     * @brief OPCODE / DEVICE : Opcode Device Get ID.
     * @details The result (four bytes) represents Manufacturer/Device ID,
     *  following the table:
     * <pre>
     * +--------------------------------------------+
     * |Response                   | Description    |
     * | First (MSB)/Second (LSB)  | Manufacurer ID |
     * | Third (MSB)/Fourth (LSB)  | Device ID      |
     * +--------------------------------------------+
     * </pre>
     */
    kCmdDeviceGetId = 0x8A,
    /** @brief OPCODE / DEVICE : Opcode Device Erase. */
    kCmdDeviceErase = 0x8B,
    /** @brief OPCODE / DEVICE : Opcode Device Unprotect. */
    kCmdDeviceUnprotect = 0x8C,
    /** @brief OPCODE / DEVICE : Opcode Device Protect. */
    kCmdDeviceProtect = 0x8D
};

// ---------------------------------------------------------------------------

/**
 * @brief Enumeration of the Device Operations.
 * @see kCmdDeviceSetupBus
 */
enum kCmdDeviceOperationEnum {
    /** @brief CMD / DEVICE : Defines a operation Reset. */
    kCmdDeviceOperationReset = 0x00,
    /** @brief CMD / DEVICE : Defines a operation Read. */
    kCmdDeviceOperationRead = 0x01,
    /** @brief CMD / DEVICE : Defines a operation Program. */
    kCmdDeviceOperationProg = 0x02,
    /** @brief CMD / DEVICE : Defines a operation Get ID. */
    kCmdDeviceOperationGetId = 0x03
};

// ---------------------------------------------------------------------------

/**
 * @brief Enumeration of the Device Algorithms.
 * @see kCmdDeviceConfigure
 */
// clang-format off
enum kCmdDeviceAlgorithmEnum {
    /** @brief CMD / DEVICE : Defines an unknown algorithm. */
    kCmdDeviceAlgorithmUnknown      = 0x00,  // 0000 0000
    /** @brief CMD / DEVICE : Defines an algorithm SRAM. */
    kCmdDeviceAlgorithmSRAM         = 0x04,  // 0000 0100
    /** @brief CMD / DEVICE : Defines an algorithm EPROM 27. */
    kCmdDeviceAlgorithmEPROM        = 0x08,  // 0000 1000
    /** @brief CMD / DEVICE : Defines an algorithm EEPROM 28C64. */
    kCmdDeviceAlgorithmEEPROM28C64  = 0x0C,  // 0000 1100
    /** @brief CMD / DEVICE : Defines an algorithm EEPROM 28C256 or upper. */
    kCmdDeviceAlgorithmEEPROM28C256 = 0x0D,  // 0000 1101
    /** @brief CMD / DEVICE : Defines an algorithm Flash 28F. */
    kCmdDeviceAlgorithmFlash28F     = 0x10,  // 0001 0000
    /** @brief CMD / DEVICE : Defines an algorithm Flash SST28SF. */
    kCmdDeviceAlgorithmFlashSST28SF = 0x11,  // 0001 0001
    /** @brief CMD / DEVICE : Defines an algorithm Flash Am28F. */
    kCmdDeviceAlgorithmFlashAm28F   = 0x12,  // 0001 0010
    /** @brief CMD / DEVICE : Defines an algorithm Flash i28F. */
    kCmdDeviceAlgorithmFlashI28F    = 0x13   // 0001 0011
};
// clang-format on

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
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
    TCmdOpCode &operator=(const TCmdOpCode &src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const TCmdOpCode &a, const TCmdOpCode &b);
} TCmdOpCode;

// ---------------------------------------------------------------------------

/** @brief Type of opcodes group (map). */
typedef std::map<kCmdOpCodeEnum, TCmdOpCode> TCmdOpCodeMap;

// ---------------------------------------------------------------------------

// clang-format off
/** @brief OPCODE : Opcodes group (map). */
static const TCmdOpCodeMap kCmdOpCodes = {
    {kCmdNop                  , {kCmdNop                  , "Nop"                    , 0, 0}},

    {kCmdVddCtrl              , {kCmdVddCtrl              , "Vdd Ctrl"               , 1, 0}},
    {kCmdVddSetV              , {kCmdVddSetV              , "Vdd SetV"               , 2, 0}},
    {kCmdVddGetV              , {kCmdVddGetV              , "Vdd GetV"               , 0, 2}},
    {kCmdVddGetDuty           , {kCmdVddGetDuty           , "Vdd GetDuty"            , 0, 2}},
    {kCmdVddGetCal            , {kCmdVddGetCal            , "Vdd GetCal"             , 0, 2}},
    {kCmdVddInitCal           , {kCmdVddInitCal           , "Vdd InitCal"            , 0, 0}},
    {kCmdVddSaveCal           , {kCmdVddSaveCal           , "Vdd SaveCal"            , 2, 0}},
    {kCmdVddOnVpp             , {kCmdVddOnVpp             , "Vdd On Vpp"             , 1, 0}},

    {kCmdVppCtrl              , {kCmdVppCtrl              , "Vpp Ctrl"               , 1, 0}},
    {kCmdVppSetV              , {kCmdVppSetV              , "Vpp SetV"               , 2, 0}},
    {kCmdVppGetV              , {kCmdVppGetV              , "Vpp GetV"               , 0, 2}},
    {kCmdVppGetDuty           , {kCmdVppGetDuty           , "Vpp GetDuty"            , 0, 2}},
    {kCmdVppGetCal            , {kCmdVppGetCal            , "Vpp GetCal"             , 0, 2}},
    {kCmdVppInitCal           , {kCmdVppInitCal           , "Vpp InitCal"            , 0, 0}},
    {kCmdVppSaveCal           , {kCmdVppSaveCal           , "Vpp SaveCal"            , 2, 0}},
    {kCmdVppOnA9              , {kCmdVppOnA9              , "Vpp On A9"              , 1, 0}},
    {kCmdVppOnA18             , {kCmdVppOnA18             , "Vpp On A18"             , 1, 0}},
    {kCmdVppOnCE              , {kCmdVppOnCE              , "Vpp On CE"              , 1, 0}},
    {kCmdVppOnOE              , {kCmdVppOnOE              , "Vpp On OE"              , 1, 0}},
    {kCmdVppOnWE              , {kCmdVppOnWE              , "Vpp On WE"              , 1, 0}},

    {kCmdBusCE                , {kCmdBusCE                , "Set CE"                 , 1, 0}},
    {kCmdBusOE                , {kCmdBusOE                , "Set OE"                 , 1, 0}},
    {kCmdBusWE                , {kCmdBusWE                , "Set WE"                 , 1, 0}},

    {kCmdBusAddrClr           , {kCmdBusAddrClr           , "Addr Clr"               , 0, 0}},
    {kCmdBusAddrInc           , {kCmdBusAddrInc           , "Addr Inc"               , 0, 0}},
    {kCmdBusAddrSet           , {kCmdBusAddrSet           , "Addr Set"               , 3, 0}},
    {kCmdBusAddrSetB          , {kCmdBusAddrSetB          , "Addr SetByte"           , 1, 0}},
    {kCmdBusAddrSetW          , {kCmdBusAddrSetW          , "Addr SetWord"           , 2, 0}},

    {kCmdBusDataClr           , {kCmdBusDataClr           , "Data Clr"               , 0, 0}},
    {kCmdBusDataSet           , {kCmdBusDataSet           , "Data Set"               , 1, 0}},
    {kCmdBusDataSetW          , {kCmdBusDataSetW          , "Data SetWord"           , 2, 0}},
    {kCmdBusDataGet           , {kCmdBusDataGet           , "Data Get"               , 0, 1}},
    {kCmdBusDataGetW          , {kCmdBusDataGetW          , "Data GetWord"           , 0, 2}},

    {kCmdDeviceSetTwp         , {kCmdDeviceSetTwp         , "Device Set Twp"         , 4, 0}},
    {kCmdDeviceSetTwc         , {kCmdDeviceSetTwc         , "Device Set Twc"         , 4, 0}},
    {kCmdDeviceConfigure      , {kCmdDeviceConfigure      , "Device Configure"       , 2, 0}},
    {kCmdDeviceSetupBus       , {kCmdDeviceSetupBus       , "Device Setup Bus"       , 1, 0}},
    {kCmdDeviceRead           , {kCmdDeviceRead           , "Device Read"            , 1, 0}},
    {kCmdDeviceWrite          , {kCmdDeviceWrite          , "Device Write"           , 1, 0}},
    {kCmdDeviceWriteSector    , {kCmdDeviceWriteSector    , "Device WriteSector"     , 2, 0}},
    {kCmdDeviceVerify         , {kCmdDeviceVerify         , "Device Verify"          , 1, 0}},
    {kCmdDeviceBlankCheck     , {kCmdDeviceBlankCheck     , "Device BlankCheck"      , 1, 0}},
    {kCmdDeviceGetId          , {kCmdDeviceGetId          , "Device GetID"           , 0, 4}},
    {kCmdDeviceErase          , {kCmdDeviceErase          , "Device Erase"           , 0, 0}},
    {kCmdDeviceUnprotect      , {kCmdDeviceUnprotect      , "Device Unprotect"       , 0, 0}},
    {kCmdDeviceProtect        , {kCmdDeviceProtect        , "Device Protect"         , 0, 0}}
};
// clang-format on

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
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

#endif  // MODULES_OPCODES_HPP_
