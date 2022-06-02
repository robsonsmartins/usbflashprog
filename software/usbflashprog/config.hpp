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
 * @file config.hpp
 * @brief Configuration of UsbFlashProg.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

// ---------------------------------------------------------------------------

#include <QtGlobal>

// ---------------------------------------------------------------------------

/** @brief COMM/USB : Vendor ID. */
constexpr quint16 kUsbVendorId = 0x2E8A;
/** @brief COMM/USB : Product ID. */
constexpr quint16 kUsbProductId = 0x000A;

/** @brief COMM/USB : Interval to enumerate USB, in milliseconds. */
constexpr int kUsbEnumerateInterval = 2000;

#endif  // CONFIG_HPP_
