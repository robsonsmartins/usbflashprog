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
 * @file backend/devices/parallel/m27xxx.hpp
 * @brief Class of a Parallel EPROM 27xxx.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_M27XXX_HPP_
#define BACKEND_DEVICES_PARALLEL_M27XXX_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "sram.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27xxx Class
 * @details The purpose of this class is to program EPROM 27xxx Memories.
 * @nosubgrouping
 */
class M27xxx : public SRAM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit M27xxx(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~M27xxx();
    /* Reimplemented */
    virtual bool read(QByteArray &buffer);
    /* Reimplemented */
    virtual bool program(const QByteArray &buffer, bool verify = false);
    /* Reimplemented */
    virtual bool verify(const QByteArray &buffer);
    /* Reimplemented */
    virtual bool blankCheck();
    /* Reimplemented */
    virtual bool getId(TDeviceID &result);

  protected:
    bool progWithWE_;      // true use WE / false use CE
    bool progIsInverted_;  // true invert prog pin (positive pulse)
};

#endif  // BACKEND_DEVICES_PARALLEL_M27XXX_HPP_
