// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/emulator/eprom.hpp
 * @brief Header of EPROM Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_EPROM_HPP_
#define TEST_EMULATOR_EPROM_HPP_

// ---------------------------------------------------------------------------

#include "chip.hpp"

// ---------------------------------------------------------------------------

/** @ingroup UnitTests
    @brief   EPROM Chip Emulator Class.
    @details Emulates a EPROM Chip.
    @nosubgrouping
*/
class ChipEPROM : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipEPROM();
    /** Destructor. */
    virtual ~ChipEPROM();
    /* reimplemented */
    virtual void setSize(uint32_t size);

  protected:
    /* count n writes 0xFF to addr zero */
    int f_numWriteFFAddrZero;
    /* count n writes another data */
    int f_numWriteAnother;
    /* reimplemented */
    virtual void write(void);
    /* emulates the chip */
    virtual void emuChip(void);
};

#endif  // TEST_EMULATOR_EPROM_HPP_
