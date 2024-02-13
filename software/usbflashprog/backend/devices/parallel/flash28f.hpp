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
 * @file backend/devices/parallel/flash28f.hpp
 * @brief Classes of the Parallel Flash 28F.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_DEVICES_PARALLEL_FLASH28F_HPP_
#define BACKEND_DEVICES_PARALLEL_FLASH28F_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

#include "pdevice.hpp"

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Parallel Flash 28F Class
 * @details The purpose of this class is to program Flash 28F Memories.
 * @nosubgrouping
 */
class Flash28F : public ParDevice {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit Flash28F(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~Flash28F();

  protected:
    /* Reimplemented */
    virtual bool eraseDevice();
};

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Parallel Flash SST28SF Class
 * @details The purpose of this class is to program Flash SST28SF Memories.
 * @nosubgrouping
 */
class FlashSST28SF : public Flash28F {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit FlashSST28SF(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~FlashSST28SF();
};

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Parallel Flash Am28F Class
 * @details The purpose of this class is to program Flash Am28F Memories.
 * @nosubgrouping
 */
class FlashAm28F : public Flash28F {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit FlashAm28F(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~FlashAm28F();
};

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Parallel Flash i28F Class
 * @details The purpose of this class is to program Flash i28F Memories.
 * @nosubgrouping
 */
class FlashI28F : public Flash28F {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit FlashI28F(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~FlashI28F();
};

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Parallel Flash Sharp i28F Class
 * @details The purpose of this class is to program Flash Sharp i28F Memories.
 * @nosubgrouping
 */
class FlashSharpI28F : public FlashI28F {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit FlashSharpI28F(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~FlashSharpI28F();
};

// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @brief Parallel Flash i28F (16-bit) Class
 * @details The purpose of this class is to program Flash i28F
 *          (16-bit) Memories.
 * @nosubgrouping
 */
class FlashI28F16Bit : public FlashI28F {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit FlashI28F16Bit(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~FlashI28F16Bit();
};

#endif  // BACKEND_DEVICES_PARALLEL_FLASH28F_HPP_
