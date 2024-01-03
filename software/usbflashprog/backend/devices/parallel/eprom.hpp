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

#include "sram.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM Class
 * @details The purpose of this class is to program EPROM Memories.
 * @nosubgrouping
 */
class EPROM : public SRAM {
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
    /* @brief If true indicates 16 bit mode (default is false). */
    bool mode16bit_;
    /* @brief Maximum attempts to program a byte. */
    int maxAttemptsProg_;
    /* @brief Delay after VPP pulse, in usec */
    uint64_t vppPulseDelay_;
    /* @brief Returns if the VPP and ~OE pins are unified.
     * @return True if the VPP and OE pins are unified (VPP/~OE).
         False if it separate. */
    virtual bool isOeVppPin_();
    /* @brief Returns if the PGM pin writes with a positive pulse.
     * @return True if the PGM pin writes with a positive pulse (HI),
         false otherwise (LO - is the default). */
    virtual bool isPositiveProgPulse_();
    /* @brief Returns if the ~PGM and ~CE pins are unified.
     * @return True if the PGM and CE pins are unified (~PGM/~CE).
         False if it separate. */
    virtual bool isPgmCePin_();
    /* Reimplemented */
    virtual bool initialize_(kDeviceOperation operation);
    /* Reimplemented */
    virtual bool resetBus_();
    /* Reimplemented */
    virtual bool read_(uint16_t &data);
    /* Reimplemented */
    virtual bool write_(uint16_t data);
    /* Reimplemented */
    virtual bool program_(const QByteArray &buffer, uint32_t &current,
                          uint32_t total);
    /* Reimplemented */
    virtual bool verify_(const QByteArray &buffer, uint32_t &current,
                         uint32_t total);
    /* @brief Initialize the control pins.
     * @param read If true indicates Read operation (default).
         False otherwise. */
    virtual void initControlPins_(bool read = true);
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27xxx (NMOS) Class
 * @details The purpose of this class is to program EPROM 27xxx Memories (NMOS).
 * @nosubgrouping
 */
class M27xxx : public EPROM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit M27xxx(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~M27xxx();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27Cxxx (CMOS) Class
 * @details The purpose of this class is to program EPROM 27Cxxx Memories
 *  (CMOS).
 * @nosubgrouping
 */
class M27Cxxx : public EPROM {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit M27Cxxx(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~M27Cxxx();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM 27Cxxx 16Bit (CMOS) Class
 * @details The purpose of this class is to program EPROM 27Cxxx Memories
 *  (CMOS) 16Bit.
 * @nosubgrouping
 */
class M27C16Bit : public M27Cxxx {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit M27C16Bit(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~M27C16Bit();
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Parallel EPROM W27Exxx Class
 * @details The purpose of this class is to program EPROM W27E/SF Memories
 *  (Electrical Erasable).
 * @nosubgrouping
 */
class W27Exxx : public M27Cxxx {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit W27Exxx(QObject *parent = nullptr);
    /** @brief Destructor. */
    virtual ~W27Exxx();
    /* Reimplemented */
    virtual bool erase(bool check = false);
    /* Reimplemented */
    virtual bool blankCheck();

  protected:
    /* @brief Delay of Erase pulse, in msec */
    uint32_t erasePulseDelay_;
    /* Reimplemented */
    virtual bool initialize_(kDeviceOperation operation);
    /* @brief Erase the EPROM.
     * @param current[in,out] Current progress, in bytes.
     * @param total Total progress, in bytes.
     * @return True if success, false otherwise. */
    virtual bool erase_(uint32_t &current, uint32_t total);
};

#endif  // BACKEND_DEVICES_PARALLEL_EPROM_HPP_
