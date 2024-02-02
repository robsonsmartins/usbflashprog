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
 * @file backend/devices/parallel/dummy.hpp
 * @brief Class of a Parallel Dummy Device.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_DUMMY_HPP_
#define BACKEND_DEVICES_PARALLEL_DUMMY_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "backend/devices/device.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel Dummy Device Class
 * @details The purpose of this class is to simulate a parallel device.
 * @nosubgrouping
 */
class Dummy : public Device {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit Dummy(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~Dummy();
    /* Reimplemented */
    virtual void setSize(uint32_t value);
    /* Reimplemented */
    virtual bool getId(TDeviceID &result);
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
    virtual bool unprotect();
    /* Reimplemented */
    virtual bool protect();

  protected:
    /* @brief The internal buffer. */
    QByteArray buffer_;
    /* @brief Indicates if is write protected. */
    bool protected_;
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel Dummy 16 Bit Device Class
 * @details The purpose of this class is to simulate a parallel device
 *   (16 bits).
 * @nosubgrouping
 */
class Dummy16Bit : public Dummy {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit Dummy16Bit(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~Dummy16Bit();
};

#endif  // BACKEND_DEVICES_PARALLEL_DUMMY_HPP_
