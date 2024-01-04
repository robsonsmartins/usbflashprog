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
 * @file backend/devices/parallel/sram.hpp
 * @brief Class of a Parallel SRAM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_SRAM_HPP_
#define BACKEND_DEVICES_PARALLEL_SRAM_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "backend/devices/device.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel SRAM Class
 * @details The purpose of this class is to test SRAM Memories.
 * @nosubgrouping
 */
class SRAM : public Device {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit SRAM(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~SRAM();
    /* Reimplemented */
    virtual bool program(const QByteArray &buffer, bool verify = false);

  protected:
    /* @brief Device Operation. */
    enum kDeviceOperation {
        /* @brief Device Read */
        kDeviceOpRead,
        /* @brief Device Prog */
        kDeviceOpProg,
        /* @brief Device Erase */
        kDeviceOpErase,
        /* @brief Device GetId */
        kDeviceOpGetId
    };
    /* @brief Delay after reset bus, in usec */
    uint64_t resetBusDelay_;
    /* @brief Delay after initialize, in usec */
    uint64_t initDelay_;
    /* @brief Tests the SRAM: Pattern Test.
     * @param current[in,out] Current progress, in bytes.
     * @param total Total progress, in bytes.
     * @return True if success, false otherwise. */
    virtual bool doPatternTest_(uint32_t &current, uint32_t total);
    /* @brief Tests the SRAM: Random Test.
     * @param current[in,out] Current progress, in bytes.
     * @param total Total progress, in bytes.
     * @return True if success, false otherwise. */
    virtual bool doRandomTest_(uint32_t &current, uint32_t total);
    /* @brief Resets (and initializes) the bus (address, data, control).
     * @return True if success, false otherwise. */
    virtual bool resetBus_();
    /* @brief Writes one byte to the device at the current address.
     * @param data Data to write.
     * @return True if success, false otherwise. */
    virtual bool write_(uint8_t data);
    /* @brief Reads one byte from the device at the current address.
     * @param data[out] Data read.
     * @return True if success, false otherwise. */
    virtual bool read_(uint8_t &data);
    /* @brief Generates a buffer with random data.
     * @return Buffer with random data. */
    virtual QByteArray generateRandomData_();
    /* @brief Generates a buffer with a specified pattern data.
     * @return Buffer with pattern data. */
    virtual QByteArray generatePatternData_();
    /* @brief Program the SRAM.
     * @param buffer Data to write.
     * @param current[in,out] Current progress, in bytes.
     * @param total Total progress, in bytes.
     * @return True if success, false otherwise. */
    virtual bool program_(const QByteArray &buffer, uint32_t &current,
                          uint32_t total);
    /* @brief Verify the SRAM.
     * @param buffer Data to compare.
     * @param current[in][out] Current progress, in bytes.
     * @param total Total progress, in bytes.
     * @return True if success, false otherwise. */
    virtual bool verify_(const QByteArray &buffer, uint32_t &current,
                         uint32_t total);
    /* @brief Open and init resources.
     * @param operation Device operation.
     * @return True if success, false otherwise. */
    virtual bool initialize_(kDeviceOperation operation);
    /* @brief Close resources and emit signals for status.
     * @param current Current address (default is zero).
     * @param total Total size [last address + 1] (default is zero).
     * @param done True if operation was finished, false otherwise (default).
     * @param success True if success (default), false otherwise.
     * @param canceled True if operation was canceled,
     *   false otherwise (default).
     * @return True if success, false otherwise. */
    virtual bool finalize_(uint32_t current, uint32_t total, bool done = false,
                           bool success = true, bool canceled = false);
    /* @brief Close resources. */
    virtual void finalize_();

  private:
    /* Reimplemented */
    virtual bool getId(TDeviceID &result);
    /* Reimplemented */
    virtual bool read(QByteArray &buffer);
    /* Reimplemented */
    virtual bool verify(const QByteArray &buffer);
    /* Reimplemented */
    virtual bool erase(bool check = false);
    /* Reimplemented */
    virtual bool blankCheck();
    /* Reimplemented */
    virtual bool unprotect();
};

#endif  // BACKEND_DEVICES_PARALLEL_SRAM_HPP_
