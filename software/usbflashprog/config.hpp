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
#include <QString>

// ---------------------------------------------------------------------------

const double kAppVersionArray[] = {
    #include "VERSION"
};

/** @brief GENERAL : App Version. */
#define kAppVersion QString::number(kAppVersionArray[0],'f',1).toStdString().c_str() // NOLINT
/** @brief GENERAL : Project Home Page. */
constexpr const char *kProjectHomePage =
    "https://usbflashprog.robsonmartins.com/";
/** @brief GENERAL : Author Home Page. */
constexpr const char *kAuthorHomePage = "https://robsonmartins.com/";
/** @brief GENERAL : Author Name. */
constexpr const char *kAuthorName = "Robson Martins";

/** @brief COMM/USB : Vendor ID. */
constexpr quint16 kUsbVendorId = 0x2E8A;
/** @brief COMM/USB : Product ID. */
constexpr quint16 kUsbProductId = 0x000A;

/** @brief COMM/USB : Interval to enumerate USB, in milliseconds. */
constexpr int kUsbEnumerateInterval = 2000;
/** @brief COMM/USB : Interval to refresh communication, in milliseconds. */
constexpr int kUsbRefreshInterval = 500;

#endif  // CONFIG_HPP_
