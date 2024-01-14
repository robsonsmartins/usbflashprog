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

#include "pdevice.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel SRAM Class
 * @details The purpose of this class is to test SRAM Memories.
 * @nosubgrouping
 */
class SRAM : public ParDevice {
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
    /* @brief Tests the SRAM: Pattern Test.
     * @return True if success, false otherwise. */
    virtual bool doPatternTest_();
    /* @brief Tests the SRAM: Random Test.
     * @return True if success, false otherwise. */
    virtual bool doRandomTest_();
};

#endif  // BACKEND_DEVICES_PARALLEL_SRAM_HPP_
