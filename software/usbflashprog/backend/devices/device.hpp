// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2023) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/device.hpp
 * @brief Base class of a Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_DEVICE_HPP_
#define BACKEND_DEVICES_DEVICE_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#ifndef TEST_BUILD
#include "backend/runner.hpp"
#else
#include "test/emulator/emulator.hpp"
#endif

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Enumeration of the device types.
 */
enum kDeviceTypeEnum {
    /** @brief Device type: Parallel Memory. */
    kDeviceParallelMemory,
    /** @brief Device type: Serial Memory. */
    kDeviceSerialMemory
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Stores identification data from a device.
 */
typedef struct TDeviceID {
    /** @brief Manufacturer code. */
    uint8_t manufacturer;
    /** @brief Device code. */
    uint8_t device;
    /**
     * @brief Returns the name of the device manufacturer.
     * @return Manufacturer name, or "Unknown".
     */
    QString getManufacturerName(void) const;
    /** @brief Constructor. */
    TDeviceID();
} TDeviceID;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Stores capability flags of a device.
 */
typedef struct TDeviceCapabilities {
    /** @brief Device has Program function. */
    bool hasProgram;
    /** @brief Device has Verify function. */
    bool hasVerify;
    /** @brief Device has Erase function. */
    bool hasErase;
    /** @brief Device has GetId function. */
    bool hasGetId;
    /** @brief Device has Read function. */
    bool hasRead;
    /** @brief Device has Blank Check function. */
    bool hasBlankCheck;
    /** @brief Device has Unprotect function. */
    bool hasUnprotect;
    /** @brief Device has Sector Size configuration. */
    bool hasSectorSize;
    /** @brief Device has Fast Prog/Erase configuration. */
    bool hasFastProg;
    /** @brief Device has Skip Prog 0xFF configuration. */
    bool hasSkipFF;
    /** @brief Device has VDD Adjust configuration. */
    bool hasVDD;
    /** @brief Device has VPP Adjust configuration. */
    bool hasVPP;
    /** @brief Constructor. */
    TDeviceCapabilities();
} TDeviceCapabilities;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Stores information of a device.
 */
typedef struct TDeviceInformation {
    /** @brief Device type. */
    kDeviceTypeEnum deviceType;
    /** @brief Device algorithm name. */
    QString name;
    /** @brief Device capability flags. */
    TDeviceCapabilities capability;
    /** @brief Constructor. */
    TDeviceInformation();
    /**
     * @brief Converts contents to string.
     * @return Device Info as QString.
     */
    QString toString() const;
} TDeviceInformation;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Device Base Class
 * @details The purpose of this class is to provide a base class of a memory
 *   device that can be read and/or written/erased.
 * @nosubgrouping
 */
class Device : public QObject {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit Device(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~Device();
    /**
     * @brief Returns if device is 16-Bit.
     * @return True if device is 16-Bit.
     *   False if device is 8-Bit.
     */
    virtual bool is16Bit() const;
    /**
     * @brief Sets the device size.
     * @param value Device size, in bytes.
     */
    virtual void setSize(uint32_t value);
    /**
     * @brief Returns the device size (in bytes).
     * @return Device size, in bytes.
     */
    virtual uint32_t getSize() const;
    /**
     * @brief Sets the serial port path.
     * @param path Serial port path.
     */
    virtual void setPort(const QString &path);
    /**
     * @brief Returns the serial port path (if any).
     * @return Path of serial port, or empty if none.
     */
    virtual QString getPort() const;
    /**
     * @brief Sets the tWP.
     * @param us tWP value, in microseconds.
     */
    virtual void setTwp(uint32_t us);
    /**
     * @brief Returns the configured tWP (in microseconds).
     * @return tWP value, in microseconds.
     */
    virtual uint32_t getTwp() const;
    /**
     * @brief Sets the tWC.
     * @param us tWC value, in microseconds.
     */
    virtual void setTwc(uint32_t us);
    /**
     * @brief Returns the configured tWC (in microseconds).
     * @return tWC value, in microseconds.
     */
    virtual uint32_t getTwc() const;
    /**
     * @brief Sets the VDD to Read.
     * @param value VDD value, in volts.
     */
    virtual void setVddRd(float value);
    /**
     * @brief Returns the configured VDD to Read (in volts).
     * @return VDD value, in volts.
     */
    virtual float getVddRd() const;
    /**
     * @brief Sets the VDD to Program.
     * @param value VDD value, in volts.
     */
    virtual void setVddWr(float value);
    /**
     * @brief Returns the configured VDD to Program (in volts).
     * @return VDD value, in volts.
     */
    virtual float getVddWr() const;
    /**
     * @brief Sets the VPP.
     * @param value VPP value, in volts.
     */
    virtual void setVpp(float value);
    /**
     * @brief Returns the configured VPP (in volts).
     * @return VPP value, in volts.
     */
    virtual float getVpp() const;
    /**
     * @brief Sets the VEE.
     * @param value VEE value, in volts.
     */
    virtual void setVee(float value);
    /**
     * @brief Returns the configured VEE (in volts).
     * @return VEE value, in volts.
     */
    virtual float getVee() const;
    /**
     * @brief Sets the Skip Prog 0xFF.
     * @param value If true (default), enables skip prog 0xFF, disables
     * otherwise.
     */
    virtual void setSkipFF(bool value = true);
    /**
     * @brief Returns the configured Skip Prog 0xFF.
     * @return If true, skip prog 0xFF is enabled, disabled otherwise.
     */
    virtual bool getSkipFF() const;
    /**
     * @brief Sets the Fast Prog/Erase.
     * @param value If true (default), enables fast prog/erase, disables
     * otherwise.
     */
    virtual void setFastProg(bool value = true);
    /**
     * @brief Returns the configured Fast Prog/Erase.
     * @return If true, fast prog/erase is enabled, disabled otherwise.
     */
    virtual bool getFastProg() const;
    /**
     * @brief Sets the Sector Size.
     * @param value Sector size, in bytes.
     */
    virtual void setSectorSize(uint16_t value);
    /**
     * @brief Returns the configured Sector Size (in bytes).
     * @return Sector size value, in bytes.
     */
    virtual uint16_t getSectorSize() const;
    /**
     * @brief Returns the Device Information.
     * @return Device Information.
     */
    virtual TDeviceInformation getInfo() const;
    /**
     * @brief Cancels the active operation (if any).
     */
    virtual void cancel();
    /**
     * @brief Gets the Device ID.
     * @param[out] result Reference to TDeviceID structure to receive data.
     * @return True if success, false otherwise.
     */
    virtual bool getId(TDeviceID &result);
    /**
     * @brief Read the Device.
     * @param[out] buffer Reference to QByteArray to receive data.
     * @return True if success, false otherwise.
     */
    virtual bool read(QByteArray &buffer);
    /**
     * @brief Program the Device.
     * @param[out] buffer Data to write.
     * @param verify If true, calls Device::verify() after program.
     *   Default is false.
     * @return True if success, false otherwise.
     */
    virtual bool program(const QByteArray &buffer, bool verify = false);
    /**
     * @brief Verifies the Device.
     * @param buffer Data to compare.
     * @return True if success, false otherwise.
     */
    virtual bool verify(const QByteArray &buffer);
    /**
     * @brief Erases the Device.
     * @param check If true, calls Device::blankCheck() after erase.
     *   Default is false.
     * @return True if success, false otherwise.
     */
    virtual bool erase(bool check = false);
    /**
     * @brief Verifies if the Device is blank.
     * @return True if device is blank, false otherwise.
     */
    virtual bool blankCheck();
    /**
     * @brief Unprotects the Device.
     * @return True if success, false otherwise.
     */
    virtual bool unprotect();

  Q_SIGNALS:
    /**
     * @brief Triggered when progress is changed, or operation is
     *   done or canceled.
     * @param current Current address (default is zero).
     * @param total Total size [last address + 1] (default is zero).
     * @param done True if operation was finished, false otherwise (default).
     * @param success True if success (default), false otherwise.
     * @param canceled True if operation was canceled,
     *   false otherwise (default).
     */
    void onProgress(uint32_t current = 0, uint32_t total = 0, bool done = false,
                    bool success = true, bool canceled = false);

  protected:
    /* @brief Indicates if is in 16 bit mode.
         Otherwise is 8 bit mode. */
    bool is16bit_;
    /* @brief Maximum attempts to program a byte. */
    int maxAttemptsProg_;
    /* @brief True if is about the canceling. */
    bool canceling_;
    /* @brief Device size, in bytes. */
    uint32_t size_;
    /* @brief tWP, in microseconds. */
    uint32_t twp_;
    /* @brief tWC, in microseconds. */
    uint32_t twc_;
    /* @brief Device settings. */
#ifndef TEST_BUILD
    Runner::TDeviceSettings flags_;
#else
    Emulator::TDeviceSettings flags_;
#endif
    /* @brief VDD to Read, in volts. */
    float vddRd_;
    /* @brief VDD to Program, in volts. */
    float vddWr_;
    /* @brief VPP, in volts. */
    float vpp_;
    /* @brief VEE (VPP to Erase), in volts. */
    float vee_;
    /* @brief Enables skip prog 0xFF. */
    bool skipFF_;
    /* @brief Enables fast prog/erase. */
    bool fastProg_;
    /* @brief Sector size, in bytes (0 = byte mode). */
    uint16_t sectorSize_;
    /* @brief Serial port path. */
    QString port_;
    /* @brief The Runner instance. */
#ifndef TEST_BUILD
    Runner runner_;
#else
    Emulator runner_;
#endif
    /* @brief Device information. */
    TDeviceInformation info_;
};

#endif  // BACKEND_DEVICES_DEVICE_HPP_
