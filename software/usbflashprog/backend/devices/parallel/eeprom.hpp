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
 * @file backend/devices/parallel/eeprom.hpp
 * @brief Classes of the Parallel EEPROM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_EEPROM_HPP_
#define BACKEND_DEVICES_PARALLEL_EEPROM_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "pdevice.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EEPROM Class
 * @details The purpose of this class is to program EEPROM Memories.
 * @nosubgrouping
 */
class EEPROM : public ParDevice {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EEPROM(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EEPROM();
    /* Reimplemented */
    virtual void setSize(uint32_t value);

  protected:
    /* Reimplemented */
    virtual bool eraseDevice();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EEPROM 28Cxxx Class
 * @details The purpose of this class is to program EEPROM X28/28Cxxx Memories.
 * @nosubgrouping
 */
class EEPROM28C : public EEPROM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EEPROM28C(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EEPROM28C();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EEPROM AT28Cxxx Class
 * @details The purpose of this class is to program EEPROM AT28Cxxx Memories.
 * @nosubgrouping
 */
class EEPROM28AT : public EEPROM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EEPROM28AT(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EEPROM28AT();
    /* Reimplemented */
    virtual void setSize(uint32_t value);
};

// ---------------------------------------------------------------------------

#endif  // BACKEND_DEVICES_PARALLEL_EEPROM_HPP_
