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
 * @file test/emulator/eeprom.hpp
 * @brief Header of EEPROM Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_EEPROM_HPP_
#define TEST_EMULATOR_EEPROM_HPP_

// ---------------------------------------------------------------------------

#include "chip.hpp"

// ---------------------------------------------------------------------------

/** @ingroup UnitTests
    @brief   EEPROM Chip Emulator Class.
    @details Emulates a EEPROM Chip.
    @nosubgrouping
*/
class ChipEEPROM : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipEEPROM();
    /** Destructor. */
    virtual ~ChipEEPROM();
    /* reimplemented */
    virtual void setSize(uint32_t size);

  protected:
    /* stores the step of current special command */
    int f_commandStep;
    /* stores what is the special command */
    TChipCommandOperation f_commandOp;
    /* emulates the chip */
    virtual void emuChip(void);
    /* check if received special command */
    virtual bool specialCommand(void);
};

#endif  // TEST_EMULATOR_EEPROM_HPP_
