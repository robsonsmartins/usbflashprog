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

#include "backend/runner.hpp"

// ---------------------------------------------------------------------------

enum kDeviceTypeEnum { kParallelMemory, kSerialMemory };

// ---------------------------------------------------------------------------

typedef struct TDeviceID {
    uint8_t manufacturer;
    uint8_t device;
} TDeviceID;

// ---------------------------------------------------------------------------

typedef struct TDeviceVoltageConfig {
    float vddProgram;
    float vppProgram;
    float vddRead;
    float vppRead;
    float vddErase;
    float vppErase;
    float vddGetId;
    float vppGetId;
    float vddUnprotect;
    float vppUnprotect;
} TDeviceVoltageConfig;

typedef struct TDeviceCapabilities {
    bool hasProgram;
    bool hasVerify;
    bool hasErase;
    bool hasGetId;
    bool hasRead;
    bool hasBlankCheck;
    bool hasUnprotect;
    bool hasSectorSize;
    bool hasFastProg;
    bool hasSkipFF;
} TDeviceCapabilities;

typedef struct TDeviceInformation {
    kDeviceTypeEnum deviceType;
    QString name;
    TDeviceVoltageConfig voltage;
    TDeviceCapabilities capability;
} TDeviceInformation;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Device Base Class
 * @details The purpose of this class is to xxxxxx.
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

    virtual void setSize(uint32_t value);
    virtual uint32_t getSize() const;

    virtual void setPort(const QString &path);
    virtual QString getPort() const;

    virtual void setTwp(uint32_t us);
    virtual uint32_t getTwp() const;

    virtual void setTwc(uint32_t us);
    virtual uint32_t getTwc() const;

    virtual void setSkipFF(bool value = true);
    virtual bool getSkipFF() const;

    virtual void setFastProg(bool value = true);
    virtual bool getFastProg() const;

    virtual void setSectorSize(uint16_t value);
    virtual uint16_t getSectorSize() const;

    virtual TDeviceInformation getInfo() const;

    virtual bool getId(TDeviceID &result);
    virtual bool read(QByteArray &buffer);
    virtual bool program(const QByteArray &buffer, bool verify = false);
    virtual bool verify(const QByteArray &buffer);
    virtual bool erase(bool check = false);
    virtual bool blankCheck();
    virtual bool unprotect();

  signals:
    void onProgress(uint32_t current, uint32_t total, float percent,
                    bool done = false);
    void onFinish(uint32_t address, bool success = true);

  protected:
    uint32_t size_;
    uint32_t twp_;
    uint32_t twc_;
    bool skipFF_;
    bool fastProg_;
    uint16_t sectorSize_;
    QString port_;
    Runner runner_;
    TDeviceInformation info_;
};

#endif  // BACKEND_DEVICES_DEVICE_HPP_
