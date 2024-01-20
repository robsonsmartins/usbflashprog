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
#include <QtCore>
#include <QString>

// ---------------------------------------------------------------------------

// clang-format off
const double kAppVersionArray[] = {
#   include "VERSION"
};
// clang-format on

/** @brief GENERAL : App Version. */
// clang-format off
#define kAppVersion QString::number(kAppVersionArray[0], 'f', 1).toStdString().c_str() // NOLINT
// clang-format on
/** @brief GENERAL : Project Home Page. */
constexpr const char *kProjectHomePage =
    "https://usbflashprog.robsonmartins.com/";
/** @brief GENERAL : Author Home Page. */
constexpr const char *kAuthorHomePage = "https://robsonmartins.com/";
/** @brief GENERAL : Author Name. */
constexpr const char *kAuthorName = "Robson Martins";
/** @brief GENERAL : Organization Name. */
constexpr const char *kOrganizationName = "RobsonMartins";
/** @brief GENERAL : Organization Domain. */
constexpr const char *kOrganizationDomain = "robsonmartins.com";
/** @brief GENERAL : App Name. */
constexpr const char *kApplicationName = "UsbFlashProg";
/** @brief GENERAL : App Full Name. */
constexpr const char *kApplicationFullName = "USB Flash/EPROM Programmer";

/** @brief COMM/USB : Vendor ID. */
constexpr quint16 kUsbVendorId = 0x2E8A;
/** @brief COMM/USB : Product ID. */
constexpr quint16 kUsbProductId = 0x000A;

/** @brief COMM/USB : Interval to enumerate USB, in milliseconds. */
constexpr int kUsbEnumerateInterval = 2000;
/** @brief COMM/USB : Interval to refresh communication, in milliseconds. */
constexpr int kUsbRefreshInterval = 500;

/** @brief GENERAL : Log filename. */
constexpr const char *kLogFileName = "ufprog.log";

// ---------------------------------------------------------------------------

/** @brief SETTING : General / LogLevel. */
constexpr const char *kSettingGeneralLogLevel = "LogLevel";
/** @brief SETTING : General / Window Position. */
constexpr const char *kSettingGeneralWindowPos = "WindowPos";
/** @brief SETTING : General / Window Size. */
constexpr const char *kSettingGeneralWindowSize = "WindowSize";
/** @brief SETTING : General / Language. */
constexpr const char *kSettingGeneralLanguage = "Language";
/** @brief SETTING : General / Last Directory. */
constexpr const char *kSettingGeneralLastDir = "LastDir";

/** @brief SETTING : Programmer / Selected Device. */
constexpr const char *kSettingProgDevice = "Prog/Device";
/** @brief SETTING : Programmer / Device Size. */
constexpr const char *kSettingProgDeviceSize = "Prog/DeviceSize";
/** @brief SETTING : Programmer / tWP. */
constexpr const char *kSettingProgTwp = "Prog/tWP";
/** @brief SETTING : Programmer / tWC. */
constexpr const char *kSettingProgTwc = "Prog/tWC";
/** @brief SETTING : Programmer / VDD to Read. */
constexpr const char *kSettingProgVddRd = "Prog/VDDToRead";
/** @brief SETTING : Programmer / VDD to Prog. */
constexpr const char *kSettingProgVddWr = "Prog/VDDToProg";
/** @brief SETTING : Programmer / VPP. */
constexpr const char *kSettingProgVpp = "Prog/VPP";
/** @brief SETTING : Programmer / VEE. */
constexpr const char *kSettingProgVee = "Prog/VEE";
/** @brief SETTING : Programmer / Skip Prog 0xFF. */
constexpr const char *kSettingProgSkipFF = "Prog/SkipFF";
/** @brief SETTING : Programmer / Fast Prog/Erase. */
constexpr const char *kSettingProgFast = "Prog/FastProg";
/** @brief SETTING : Programmer / Sector Size. */
constexpr const char *kSettingProgSectorSize = "Prog/SectorSize";

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Stores the programmer settings.
 */
typedef struct TProgrammerSettings {
    /** @brief Device name. */
    QString device;
    /** @brief Device size in bytes. */
    uint32_t size;
    /** @brief tWP value in microseconds. */
    uint32_t twp;
    /** @brief tWC value in microseconds. */
    uint32_t twc;
    /** @brief VDD to Read value in volts. */
    float vddRd;
    /** @brief VDD to Prog value in volts. */
    float vddWr;
    /** @brief VPP value in volts. */
    float vpp;
    /** @brief VEE value in volts. */
    float vee;
    /** @brief Skip Prog 0xFF. */
    bool skipFF;
    /** @brief Fast Prog/Erase. */
    bool fastProg;
    /** @brief Sector Size in bytes (0 is byte prog). */
    uint16_t sectorSize;
} TProgrammerSettings;

/**
 * @ingroup Software
 * @brief Stores the application settings.
 */
typedef struct TApplicationSettings {
    /** @brief The Log Level.
     * <ul>
     *  <li>0: Disable (or \< 0) - Default</li>
     *  <li>1: Fatal</li>
     *  <li>2: Critical</li>
     *  <li>3: Warning</li>
     *  <li>4: Info</li>
     *  <li>5: Debug (or \> 5)</li>
     * </ul>
     */
    int logLevel;
    /** @brief The coordinates of the main window. */
    QPoint windowPos;
    /** @brief The size of the main window. */
    QSize windowSize;
    /** @brief The language of the application.
     *    Empty is autodetect (default). */
    QString language;
    /** @brief The last opened directory. */
    QString lastDir;
    /** @brief Programmer settings. */
    TProgrammerSettings prog;
} TApplicationSettings;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Stores the language settings.
 */
typedef struct TLanguageSettings {
    /** @brief Language code. */
    QString code;
    /** @brief Language Description. */
    QString desc;
} TLanguageSettings;

/**
 * @ingroup Software
 * @brief List of supported Languages.
 */
// clang-format off
const TLanguageSettings kAppSupportedLanguages[] = {
    /* translate description */
    {"", "Autodetect"},
    /* no translate description */
    {"en_US", "English (en_US)"},
    {"pt_BR", "Portugu\u00EAs Brasileiro (pt_BR)"},
    {"es_ES", "Español (es_ES)"}
};
// clang-format on

// ---------------------------------------------------------------------------

#endif  // CONFIG_HPP_
