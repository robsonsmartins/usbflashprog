// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file backend/devices/parallel/pdevice.hpp
 * @brief Class of a Parallel Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_DEVICE_HPP_
#define BACKEND_DEVICES_PARALLEL_DEVICE_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "backend/devices/device.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel Device Base Class
 * @details The purpose of this class is to provide a base class of a parallel
 *   memory device that can be read and/or written/erased.
 * @nosubgrouping
 */
class ParDevice : public Device {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit ParDevice(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~ParDevice();
    /* Reimplemented */
    virtual bool read(QByteArray &buffer);
    /* Reimplemented */
    virtual bool program(const QByteArray &buffer, bool verify = false);
    /* Reimplemented */
    virtual bool verify(const QByteArray &buffer);
    /* Reimplemented */
    virtual bool erase(bool check = false);
    /* Reimplemented */
    virtual bool blankCheck();
    /* Reimplemented */
    virtual bool getId(TDeviceID &result);
    /* Reimplemented */
    virtual bool unprotect();
    /* Reimplemented */
    virtual bool protect();

  protected:
    /** @brief Device Operation. */
    enum kDeviceOperation {
        /** @brief Device Reset Operation. */
        kDeviceOpReset,
        /** @brief Device Read Operation. */
        kDeviceOpRead,
        /** @brief Device Prog Operation. */
        kDeviceOpProg,
        /** @brief Device Erase Operation. */
        kDeviceOpErase,
        /** @brief Device GetId Operation. */
        kDeviceOpGetId
    };
    /**
     * @brief Program the device.
     * @param buffer Data to write.
     * @return True if success, false otherwise.
     */
    virtual bool programDevice(const QByteArray &buffer);
    /**
     * @brief Verify the device.
     * @param buffer Data to compare.
     * @return True if success, false otherwise.
     */
    virtual bool verifyDevice(const QByteArray &buffer);
    /**
     * @brief Read the device.
     * @param buffer[out] Data to read.
     * @return True if success, false otherwise.
     */
    virtual bool readDevice(QByteArray &buffer);
    /**
     * @brief Get ID of the device.
     * @param deviceId[out] Device ID to get.
     * @return True if success, false otherwise.
     */
    virtual bool getIdDevice(TDeviceID &deviceId);
    /**
     * @brief Erase the device.
     * @return True if success, false otherwise.
     */
    virtual bool eraseDevice();
    /**
     * @brief Blank Check the device.
     * @return True if success, false otherwise.
     */
    virtual bool blankCheckDevice();
    /**
     * @brief Protect/Unprotect the device.
     * @param protect If true, protects the device.
     *   Otherwise, unprotects the device.
     * @return True if success, false otherwise.
     */
    virtual bool protectDevice(bool protect);
    /**
     * @brief Open and init resources.
     * @param operation Device operation.
     * @return True if success, false otherwise.
     */
    virtual bool initDevice(kDeviceOperation operation);
    /**
     * @brief Close resources and emit signals for status.
     * @param current Current address (default is zero).
     * @param total Total size [last address + 1] (default is zero).
     * @param done True if operation was finished, false otherwise (default).
     * @param success True if success (default), false otherwise.
     * @param canceled True if operation was canceled,
     *   false otherwise (default).
     * @return True if success, false otherwise.
     */
    virtual bool finalizeDevice(uint32_t current, uint32_t total,
                                bool done = false, bool success = true,
                                bool canceled = false);
    /** @brief Close resources. */
    virtual void finalizeDevice();

  protected:
    /* @brief Generates a buffer with random data.
     * @return Buffer with random data. */
    virtual QByteArray generateRandomData_();
    /* @brief Generates a buffer with a specified pattern data.
     * @return Buffer with pattern data. */
    virtual QByteArray generatePatternData_();
};

#endif  // BACKEND_DEVICES_PARALLEL_DEVICE_HPP_
