// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file modules/devcmd.hpp
 * @brief Header of the Device Commands.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_DEVCMD_HPP_
#define TEST_EMULATOR_DEVCMD_HPP_

// ---------------------------------------------------------------------------

#include <cstdint>

// ---------------------------------------------------------------------------

/** @brief Defines a command to send to a device. */
typedef struct TDeviceCommand {
    /** @brief Address. */
    uint32_t addr;
    /** @brief Data value. */
    uint16_t data;
} TDeviceCommand;

// ---------------------------------------------------------------------------

/** @brief Represents Any Address. */
#define ANY_ADDRESS static_cast<uint32_t>(-1)
/** @brief Represents Any Data. */
#define ANY_DATA static_cast<uint16_t>(-1)

// ---------------------------------------------------------------------------
// EPROM 27
// ---------------------------------------------------------------------------

/** @brief EPROM 27E: Erase pulse duration, in milliseconds. */
constexpr uint32_t kDeviceErasePulseDuration27E = 100;

// ---------------------------------------------------------------------------
// EEPROM 28C
// ---------------------------------------------------------------------------

// clang-format off

/** @brief Command sequence to Unprotect an EEPROM 28C/X28 64
           (ST/Atmel/Xicor). */
constexpr TDeviceCommand kDeviceCmdUnprotect28C64[] = {
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0x80},
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0x20}
};

/** @brief Command sequence to Protect an EEPROM 28C/X28 64
           (ST/Atmel/Xicor). */
constexpr TDeviceCommand kDeviceCmdProtect28C64[] = {
    {0x1555, 0xAA}, {0x0AAA, 0x55}, {0x1555, 0xA0}
};

/** @brief Command sequence to Unprotect an EEPROM 28C/X28 256
           (ST/Atmel/Xicor). */
constexpr TDeviceCommand kDeviceCmdUnprotect28C256[] = {
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x80},
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x20}
};

/** @brief Command sequence to Protect an EEPROM 28C/X28 256
           (ST/Atmel/Xicor). */
constexpr TDeviceCommand kDeviceCmdProtect28C256[] = {
    {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0xA0}
};

// clang-format on

// ---------------------------------------------------------------------------
// Flash 28F
// ---------------------------------------------------------------------------

/** @brief Flash 28F: Erase delay, in milliseconds. */
constexpr uint32_t kDeviceEraseDelay28F = 10;

// clang-format off

/** @brief Command sequence to Read a Flash 28F. */
constexpr TDeviceCommand kDeviceCmdRead28F[] = {
    {ANY_ADDRESS, 0x00}
};

/** @brief Command sequence to Write a Flash 28F. */
constexpr TDeviceCommand kDeviceCmdWrite28F[] = {
    {ANY_ADDRESS, 0x40}
};

/** @brief Command sequence to Verify a Flash 28F. */
constexpr TDeviceCommand kDeviceCmdVerify28F[] = {
    {ANY_ADDRESS, 0xC0}
};

/** @brief Command sequence to Erase a Flash 28F. */
constexpr TDeviceCommand kDeviceCmdErase28F[] = {
    {ANY_ADDRESS, 0x20}, {ANY_ADDRESS, 0x20}
};

/** @brief Command sequence to GetID a Flash 28F. */
constexpr TDeviceCommand kDeviceCmdGetId28F[] = {
    {0x00, 0x90}
};

// clang-format on

// ---------------------------------------------------------------------------
// Flash SST28SF
// ---------------------------------------------------------------------------

// clang-format off

/** @brief Command sequence to Write a Flash SST28SF. */
constexpr TDeviceCommand kDeviceCmdWriteSST28SF[] = {
    {ANY_ADDRESS, 0x10}
};

/** @brief Command sequence to Erase a Flash SST28SF.
 *  @details This erase a sector (256), not the full device. */
constexpr TDeviceCommand kDeviceCmdEraseSST28SF[] = {
    {ANY_ADDRESS, 0x20}, {ANY_ADDRESS, 0xD0}
};

/** @brief Command sequence to GetID a Flash SST28SF. */
constexpr TDeviceCommand kDeviceCmdGetIdSST28SF[] = {
    {0x00, 0x90}
};

/** @brief Command sequence to disable SDP in a Flash SST28SF. */
constexpr TDeviceCommand kDeviceCmdDisableSdpSST28SF[] = {
    { 0x1823, ANY_DATA}, {0x1820, ANY_DATA}, {0x1822, ANY_DATA},
    { 0x0418, ANY_DATA}, {0x041B, ANY_DATA}, 
    { 0x0419, ANY_DATA}, {0x041A, ANY_DATA}
};

/** @brief Command sequence to enable SDP in a Flash SST28SF. */
constexpr TDeviceCommand kDeviceCmdEnableSdpSST28SF[] = {
    { 0x1823, ANY_DATA}, {0x1820, ANY_DATA}, {0x1822, ANY_DATA},
    { 0x0418, ANY_DATA}, {0x041B, ANY_DATA}, 
    { 0x0419, ANY_DATA}, {0x040A, ANY_DATA}
};

// clang-format on

// ---------------------------------------------------------------------------
// Flash Am28F(A)
// ---------------------------------------------------------------------------

// clang-format off

/** @brief Command sequence to Read a Flash Am28F(A). */
constexpr TDeviceCommand kDeviceCmdReadAm28F[] = {
    {ANY_ADDRESS, 0x00}
};

/** @brief Command sequence to Write a Flash Am28F(A). */
constexpr TDeviceCommand kDeviceCmdWriteAm28F[] = {
    {ANY_ADDRESS, 0x10}
};

/** @brief Command sequence to Erase a Flash Am28F(A). */
constexpr TDeviceCommand kDeviceCmdEraseAm28F[] = {
    {ANY_ADDRESS, 0x30}, {ANY_ADDRESS, 0x30}
};

/** @brief Command sequence to GetID a Flash Am28F(A). */
constexpr TDeviceCommand kDeviceCmdGetIdAm28F[] = {
    {0x00, 0x90}
};

// clang-format on

// ---------------------------------------------------------------------------
// Flash i28F
// ---------------------------------------------------------------------------

/** @brief Flash i28F Status Byte OK. */
constexpr uint8_t kDeviceStatusByteOkI28F = 0x80;

// clang-format off

/** @brief Command sequence to Read a Flash i28F. */
constexpr TDeviceCommand kDeviceCmdReadI28F[] = {
    {ANY_ADDRESS, 0xFF}
};

/** @brief Command sequence to Write a Flash i28F. */
constexpr TDeviceCommand kDeviceCmdWriteI28F[] = {
    {ANY_ADDRESS, 0x40}
};

/** @brief Command sequence to Erase a Flash i28F. 
 *  @details This erase a block (n bytes), not the full device. */
constexpr TDeviceCommand kDeviceCmdEraseI28F[] = {
    {ANY_ADDRESS, 0x20}, {ANY_ADDRESS, 0xD0}
};

/** @brief Command sequence to GetID a Flash i28F. */
constexpr TDeviceCommand kDeviceCmdGetIdI28F[] = {
    {0x00, 0x90}
};

// clang-format on

// ---------------------------------------------------------------------------

#endif  // TEST_EMULATOR_DEVCMD_HPP_