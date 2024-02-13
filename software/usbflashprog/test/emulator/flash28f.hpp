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
 * @file test/emulator/flash28f.hpp
 * @brief Header of Flash 28F Chip Emulation.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_FLASH28F_HPP_
#define TEST_EMULATOR_FLASH28F_HPP_

// ---------------------------------------------------------------------------

#include "chip.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief   Flash 28Fxxx Chip Emulator Class.
 * @details Emulates a Flash 28Fxxx Chip.
 * @nosubgrouping
 */
class ChipFlash28F : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipFlash28F();
    /** Destructor. */
    virtual ~ChipFlash28F();
    /* reimplemented */
    virtual void setSize(uint32_t size);

  protected:
    /* stores the step of current special command */
    int f_commandStep;
    /* stores what is the special command */
    TChipCommandOperation f_operation;
    /* emulates the chip */
    virtual void emuChip(void);
    /* check if received special command */
    virtual bool specialCommand(void);
};

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief   Flash SST 28xFxxx Chip Emulator Class.
 * @details Emulates a Flash SST 28xFxxx Chip.
 * @nosubgrouping
 */
class ChipFlashSST28F : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipFlashSST28F();
    /** Destructor. */
    virtual ~ChipFlashSST28F();
    /* reimplemented */
    virtual void setSize(uint32_t size);
    /* reimplemented */
    virtual void setVDD(bool state);

  protected:
    /* stores the step of current special command */
    int f_commandStep;
    /* write protected */
    bool f_protected;
    /* stores what is the special command */
    TChipCommandOperation f_operation;
    /* reimplemented */
    virtual void read(void);
    /* emulates the chip */
    virtual void emuChip(void);
    /* check if received special command */
    virtual bool specialCommand(void);
};

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief   Flash Intel 28Fxxx Chip Emulator Class.
 * @details Emulates a Flash Intel 28Fxxx Chip.
 * @nosubgrouping
 */
class ChipFlashIntel28F : public BaseParChip {
  public:
    /** Default Constructor. */
    ChipFlashIntel28F();
    /** Destructor. */
    virtual ~ChipFlashIntel28F();
    /* reimplemented */
    virtual void setSize(uint32_t size);
    /* reimplemented */
    virtual uint16_t getDataBus(void);

  protected:
    /* stores the step of current special command */
    int f_commandStep;
    /* stores what is the special command */
    TChipCommandOperation f_operation;
    /* emulates the chip */
    virtual void emuChip(void);
    /* check if received special command */
    virtual bool specialCommand(void);
    /* update the data bus (if necessary) */
    virtual bool updateDataBus(bool readMemory = true);
};

// ---------------------------------------------------------------------------

#endif  // TEST_EMULATOR_FLASH28F_HPP_