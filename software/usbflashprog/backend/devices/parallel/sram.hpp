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
 * @details The purpose of this class is to test SRAM 5V Memories.
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

    virtual bool program(const QByteArray &buffer, bool verify = false);

  protected:
    /* @brief Tests the SRAM.
     * @param current[in,out] Current progress, in bytes.
     * @param total Total progress, in bytes.
     * @return True if success, false otherwise. */
    virtual bool testRAM_(uint32_t &current, uint32_t total);
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
};

#endif  // BACKEND_DEVICES_PARALLEL_SRAM_HPP_
