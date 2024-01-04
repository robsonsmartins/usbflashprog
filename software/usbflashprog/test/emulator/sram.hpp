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
 * @file test/emulator/sram.hpp
 * @brief Header of SRAM Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_SRAM_HPP_
#define TEST_EMULATOR_SRAM_HPP_

// ---------------------------------------------------------------------------

#include "chip.hpp"

// ---------------------------------------------------------------------------

/** @ingroup UnitTests
    @brief   SRAM Chip Emulator Class.
    @details Emulates a SRAM Chip.
    @nosubgrouping
*/
class ChipSRAM : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipSRAM();
    /** Destructor. */
    virtual ~ChipSRAM();
    /* reimplemented */
    virtual void setSize(uint32_t size);
    virtual void setVDD(bool state);

  protected:
    /* emulates the chip */
    virtual void emuChip(void);
};

#endif  // TEST_EMULATOR_SRAM_HPP_
