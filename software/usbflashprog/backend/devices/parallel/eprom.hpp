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
 * @file backend/devices/parallel/eprom.hpp
 * @brief Classes of the Parallel EPROM.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_EPROM_HPP_
#define BACKEND_DEVICES_PARALLEL_EPROM_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "pdevice.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM Class
 * @details The purpose of this class is to program EPROM Memories.
 * @nosubgrouping
 */
class EPROM : public ParDevice {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EPROM(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EPROM();
    /* Reimplemented */
    virtual void setSize(uint32_t value);
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27xxx (NMOS) Class
 * @details The purpose of this class is to program EPROM 27xxx Memories (NMOS).
 * @nosubgrouping
 */
class EPROM27 : public EPROM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EPROM27(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EPROM27();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27Cxxx (CMOS) Class
 * @details The purpose of this class is to program EPROM 27Cxxx Memories
 *  (CMOS).
 * @nosubgrouping
 */
class EPROM27C : public EPROM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EPROM27C(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EPROM27C();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27Cxxx 16Bit (CMOS) Class
 * @details The purpose of this class is to program EPROM 27Cxxx Memories
 *  (CMOS) 16Bit.
 * @nosubgrouping
 */
class EPROM27C16Bit : public EPROM27C {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EPROM27C16Bit(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EPROM27C16Bit();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM W27Exxx Class
 * @details The purpose of this class is to program EPROM W27E/SF Memories
 *  (Electrical Erasable).
 * @nosubgrouping
 */
class EPROM27E : public EPROM27C {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit EPROM27E(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~EPROM27E();
};

#endif  // BACKEND_DEVICES_PARALLEL_EPROM_HPP_
