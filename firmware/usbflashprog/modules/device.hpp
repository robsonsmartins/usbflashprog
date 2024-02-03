// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
 * @file modules/device.hpp
 * @brief Header of the Device Operation Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MODULES_DEVICE_HPP_
#define MODULES_DEVICE_HPP_

// ---------------------------------------------------------------------------

#include <vector>

#include "modules/vgenerator.hpp"
#include "modules/bus.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Device Operation Class
 * @details The purpose of this class is to handle the operation with device.
 * @nosubgrouping
 */
class Device {
  public:
    /** @brief Type of byte array. */
    typedef std::vector<uint8_t> TByteArray;

    /** @brief Device Flags type. */
    typedef struct TDeviceFlags {
        /** @brief Skip Write 0xFF. */
        bool skipFF;
        /** @brief Prog with VPP on. */
        bool progWithVpp;
        /** @brief VPP/~OE Pin. */
        bool vppOePin;
        /** @brief ~PGM/~CE Pin. */
        bool pgmCePin;
        /** @brief PGM positive. */
        bool pgmPositive;
    } TDeviceFlags;

    /** @brief Device Settings type. */
    typedef struct TDeviceSettings {
        /** @brief tWP Setting (microseconds). */
        uint32_t twp;
        /** @brief tWC Setting (microseconds). */
        uint32_t twc;
        /** @brief Device flags. */
        TDeviceFlags flags;
    } TDeviceSettings;

    /** @brief Defines a command to send to a device. */
    typedef struct TDeviceCommand {
        /** @brief Address. */
        uint32_t addr;
        /** @brief Data value. */
        uint16_t data;
    } TDeviceCommand;

  public:
    /** @brief Constructor. */
    Device();
    /** @brief Starts the device. */
    void init();

    /**
     * @brief VDD Control.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vddCtrl(bool value = true);
    /**
     * @brief VDD Set Voltage.
     * @param value Value to set.
     */
    void vddSetV(float value);
    /**
     * @brief VDD Get Voltage.
     * @return VDD Voltage (volts).
     */
    float vddGetV(void);
    /**
     * @brief VDD Get Duty Cycle.
     * @return VDD Duty Cycle (percent).
     */
    float vddGetDuty(void);
    /**
     * @brief VDD Get Calibration Value.
     * @return VDD Calibration Value.
     */
    float vddGetCal(void);
    /**
     * @brief VDD Init Calibration.
     */
    void vddInitCal(void);
    /**
     * @brief VDD Save Calibration.
     * @param value Value to save.
     */
    void vddSaveCal(float value);
    /**
     * @brief VDD on VPP.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vddOnVpp(bool value = true);

    /**
     * @brief VPP Control.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vppCtrl(bool value = true);
    /**
     * @brief VPP Set Voltage.
     * @param value Value to set.
     */
    void vppSetV(float value);
    /**
     * @brief VPP Get Voltage.
     * @return VPP Voltage (volts).
     */
    float vppGetV(void);
    /**
     * @brief VPP Get Duty Cycle.
     * @return VPP Duty Cycle (percent).
     */
    float vppGetDuty(void);
    /**
     * @brief VPP Get Calibration Value.
     * @return VPP Calibration Value.
     */
    float vppGetCal(void);
    /**
     * @brief VPP Init Calibration.
     */
    void vppInitCal(void);
    /**
     * @brief VPP Save Calibration.
     * @param value Value to save.
     */
    void vppSaveCal(float value);
    /**
     * @brief VPP on A9.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vppOnA9(bool value = true);
    /**
     * @brief VPP on A18.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vppOnA18(bool value = true);
    /**
     * @brief VPP on CE.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vppOnCE(bool value = true);
    /**
     * @brief VPP on WE.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vppOnWE(bool value = true);
    /**
     * @brief VPP on OE.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void vppOnOE(bool value = true);

    /**
     * @brief CE Control.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void setCE(bool value = true);
    /**
     * @brief WE Control.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void setWE(bool value = true);
    /**
     * @brief OE Control.
     * @param value If true (default), sets the pin. Resets otherwise.
     */
    void setOE(bool value = true);

    /**
     * @brief Address Clear.
     * @return True if success, false otherwise.
     */
    bool addrClr();
    /**
     * @brief Address Increment.
     * @return True if success, false otherwise.
     */
    bool addrInc();
    /**
     * @brief Address Set DWord.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool addrSet(uint32_t value);
    /**
     * @brief Address Set Byte.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool addrSetB(uint8_t value);
    /**
     * @brief Address Set Word.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool addrSetW(uint16_t value);
    /**
     * @brief Address Get (internal value).
     * @return Address value if success, 0xFFFFFFFF otherwise.
     */
    uint32_t addrGet() const;

    /**
     * @brief Data Clear.
     * @return True if success, false otherwise.
     */
    bool dataClr();
    /**
     * @brief Data Set Byte.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool dataSet(uint8_t value);
    /**
     * @brief Data Set Word.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool dataSetW(uint16_t value);
    /**
     * @brief Data Get Byte.
     * @return Data value if success, 0xFF otherwise.
     */
    uint8_t dataGet();
    /**
     * @brief Data Get Word.
     * @return Data value if success, 0xFFFF otherwise.
     */
    uint16_t dataGetW();

    /**
     * @brief Device Set tWP.
     * @param value Value to set.
     */
    void setTwp(uint32_t value);
    /**
     * @brief Device Set tWC.
     * @param value Value to set.
     */
    void setTwc(uint32_t value);
    /**
     * @brief Device Set Flags.
     * @param value Value to set.
     */
    void setFlags(uint8_t value);
    /**
     * @brief Device Setup Bus.
     * @param operation Operation to realize.
     * @return True if success, false otherwise.
     */
    bool setupBus(uint8_t operation);
    /**
     * @brief Device Read Byte at current address.
     * @details Read a buffer (count bytes) at current address, and
     *   increment the address.
     * @param count Number of bytes to read. Default is 64.
     * @return Buffer with read value if success, empty otherwise.
     */
    TByteArray read(size_t count = 64);
    /**
     * @brief Device Read Word at current address.
     * @details Read a buffer (count words) at current address, and
     *   increment the address.
     * @param count Number of words to read. Default is 64.
     * @return Buffer with read value if success (MSB first), empty otherwise.
     */
    TByteArray readW(size_t count = 64);
    /**
     * @brief Device Write Byte at current address.
     * @details Write a buffer (count bytes) at current address, and
     *   increment the address.
     * @param value Buffer to write.
     * @param count Number of bytes to write. Default is 64.
     * @param verify If true (default), verify after write.
     * @return True if success, false otherwise.
     */
    bool write(const TByteArray& value, size_t count = 64, bool verify = true);
    /**
     * @brief Device Write Word at current address.
     * @details Write a buffer (count words) at current address, and
     *   increment the address.
     * @param value Buffer to write (MSB first).
     * @param count Number of words to write. Default is 64.
     * @param verify If true (default), verify after write.
     * @return True if success, false otherwise.
     */
    bool writeW(const TByteArray& value, size_t count = 64, bool verify = true);
    /**
     * @brief Device Write Sector Byte at current address.
     * @details Write a sector (count bytes) at current address, and
     *   increment the address.
     * @param sector Sector to write.
     * @param count Sector size, in bytes. Default is 16.
     * @param verify If true (default), verify after write.
     * @return True if success, false otherwise.
     */
    bool writeSector(const TByteArray& sector, size_t count = 16,
                     bool verify = true);
    /**
     * @brief Device Write Sector Word at current address.
     * @details Write a sector (count words) at current address, and
     *   increment the address.
     * @param sector Sector to write (MSB first).
     * @param count Sector size, in words. Default is 16.
     * @param verify If true (default), verify after write.
     * @return True if success, false otherwise.
     */
    bool writeSectorW(const TByteArray& sector, size_t count = 16,
                      bool verify = true);
    /**
     * @brief Device Verify Byte at current address.
     * @details Verify a buffer (count bytes) at current address, and
     *   increment the address.
     * @param value Buffer to verify.
     * @param count Number of bytes to verify. Default is 64.
     * @return True if success, false otherwise.
     */
    bool verify(const TByteArray& value, size_t count = 64);
    /**
     * @brief Device Verify Word at current address.
     * @details Verify a buffer (count words) at current address, and
     *   increment the address.
     * @param value Buffer to verify (MSB first).
     * @param count Number of words to verify. Default is 64.
     * @return True if success, false otherwise.
     */
    bool verifyW(const TByteArray& value, size_t count = 64);
    /**
     * @brief Device Blank Check Byte at current address.
     * @details Blank Check a buffer (count bytes) at current address, and
     *   increment the address.
     * @param count Number of bytes to check. Default is 64.
     * @return True if success, false otherwise.
     */
    bool blankCheck(size_t count = 64);
    /**
     * @brief Device Blank Check Word at current address.
     * @details Blank Check a buffer (count words) at current address, and
     *   increment the address.
     * @param count Number of words to check. Default is 64.
     * @return True if success, false otherwise.
     */
    bool blankCheckW(size_t count = 64);
    /**
     * @brief Device Get ID.
     * @param id[out] Manufacturer ID (MSB); Device ID (LSB).
     * @return True if success, false otherwise.
     */
    bool getId(uint16_t& id);
    /**
     * @brief Device Erase.
     * @param algo Device algorithm.
     * @return True if success, false otherwise.
     */
    bool erase(uint8_t algo);
    /**
     * @brief Device Unprotect.
     * @param algo Device algorithm.
     * @return True if success, false otherwise.
     */
    bool unprotect(uint8_t algo);
    /**
     * @brief Device Protect.
     * @param algo Device algorithm.
     * @return True if success, false otherwise.
     */
    bool protect(uint8_t algo);

  private:
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
    /* @brief Stores device settings. */
    TDeviceSettings settings_;

  private:
    /*
     * @brief Runs the device erase (EPROM 27E algorithm).
     * @return True if sucessfull. False otherwise.
     */
    bool erase27E_();
    /*
     * @brief Runs the device protect/unprotect (EEPROM 28C algorithm).
     * @param protect If true, protects device. Otherwise, unprotects device.
     * @param is256 If true, uses the 28C256 algorithm.
     *   Otherwise, uses the 28C64 algorithm.
     * @return True if sucessfull. False otherwise.
     */
    bool protect28C_(bool protect, bool is256);

  private:
    /*
     * @brief Device read one byte/word at current address.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return Data if success, 0xFF or 0xFFFF otherwise.
     */
    uint16_t read_(bool is16bit = false);
    /*
     * @brief Device write one byte/word at current address.
     * @param data Data to write.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if success, false otherwise.
     */
    bool write_(uint16_t data, bool is16bit = false);
    /*
     * @brief Device verify one byte/word at current address.
     * @param data Data to verify.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if success, false otherwise.
     */
    bool verify_(uint16_t data, bool is16bit = false);
    /*
     * @brief Write one byte/word into device, at specified address.
     * @param addr Address to write.
     * @param data Data value to write.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if sucessfull. False otherwise.
     */
    bool writeAtAddr_(uint32_t addr, uint16_t data, bool is16bit = false);
    /*
     * @brief Device Write Sector at current address.
     * @details Write a sector (count bytes/words) at current address, and
     *   increment the address.
     * @param sector Sector to write.
     * @param count Sector size, in bytes/words.
     * @param verify If true, verify after write.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if success, false otherwise.
     */
    bool writeSector_(const TByteArray& sector, size_t count, bool verify,
                      bool is16bit = false);
    /*
     * @brief Device Read Buffer at current address.
     * @details Read a buffer (count bytes/words) at current address, and
     *   increment the address.
     * @param count Buffer size, in bytes/words.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return Buffer with read value if success, empty otherwise.
     */
    TByteArray readBuffer_(size_t count, bool is16bit = false);
    /*
     * @brief Device Write Buffer at current address.
     * @details Write a buffer (count bytes/words) at current address, and
     *   increment the address.
     * @param value Buffer to write.
     * @param count Buffer size, in bytes/words.
     * @param verify If true, verify after write.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if success, false otherwise.
     */
    bool writeBuffer_(const TByteArray& value, size_t count, bool verify,
                      bool is16bit = false);
    /*
     * @brief Device Verify Buffer at current address.
     * @details Verify a buffer (count bytes/words) at current address, and
     *   increment the address.
     * @param value Buffer to verify.
     * @param count Buffer size, in bytes/words.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if success, false otherwise.
     */
    bool verifyBuffer_(const TByteArray& value, size_t count,
                       bool is16bit = false);
    /*
     * @brief Device Blank Check Buffer at current address.
     * @details Blank Check a buffer (count bytes/words) at current address, and
     *   increment the address.
     * @param count Buffer size, in bytes/words.
     * @param is16bit If true, indicates a 16-bit device.
     *   False (default) indicates a 8-bit device.
     * @return True if success, false otherwise.
     */
    bool blankCheckBuffer_(size_t count, bool is16bit = false);
};

#endif  // MODULES_DEVICE_HPP_