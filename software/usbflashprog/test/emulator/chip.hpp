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
 * @file test/emulator/chip.hpp
 * @brief Header of Chip Emulation Base Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_CHIP_HPP_
#define TEST_EMULATOR_CHIP_HPP_

// ---------------------------------------------------------------------------

#include <vector>
#include <cstdint>

// ---------------------------------------------------------------------------

/** @ingroup UnitTests
    @brief   Chip Emulator Base Abstract Class.
    @details This is a base class for Chip Emulator.<br>
        Provides all abstract methods need for a chip emulator.
    @nosubgrouping
*/
class BaseChip {
  public:
    /** Default Constructor. */
    BaseChip();
    /**
     * @brief Copy constructor.
     * @param src Another BaseChip object.
     */
    explicit BaseChip(const BaseChip& src);
    /**
     * @brief Assignment operator.
     * @param src Another BaseChip object.
     * @return Reference to this object.
     */
    BaseChip& operator=(const BaseChip& src);
    /** Destructor. */
    virtual ~BaseChip();
    /** Set Memory Size.
        @param[in] size Memory size (number of addressable positions)
     */
    virtual void setSize(uint32_t size);
    /** Set/Reset VDD (main power) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void setVDD(bool state);
    /** Set/Reset VPP (high voltage programming power) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void setVPP(bool state);

  protected:
    /* control signals */
    bool f_vdd, f_vpp;
    /* address bus */
    uint32_t f_addr_bus;
    /* data bus */
    uint16_t f_data_bus;
    /* memory area */
    std::vector<uint16_t> f_memory_area;
    /** Reads data from memory area to Data Bus. */
    virtual void read(void);
    /** Writes data from Data Bus to memory area. */
    virtual void write(void);
    /** Emulates the chip. */
    virtual void emuChip(void) = 0;
    /** Fills the entire memory area with random data. */
    virtual void randomizeData(void);
    /** Fills the entire memory area with a data.
        @param[in] data Data to be filled into memory
     */
    virtual void fillData(uint16_t data);
};

// ---------------------------------------------------------------------------
/** @ingroup UnitTests
    @brief   Parallel Chip Emulator Base Abstract Class.
    @details This is a base class for Parallel Chip Emulator.<br>
        Provides all abstract methods need for emulate a chip
        (with parallel bus).
    @nosubgrouping
*/
class BaseParChip : public BaseChip {
  public:
    /** Default Constructor. */
    BaseParChip();
    /** Destructor. */
    virtual ~BaseParChip();
    /** Set/Reset OE (Output Enable) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void setOE(bool state);
    /** Set/Reset CE (Chip Enable) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void setCE(bool state);
    /** Set/Reset WE (Write Enable) pin.
        @param[in] state If true (default), set pin; else, clear pin
     */
    virtual void setWE(bool state);
    /** Write a Address Bus value.
        @param[in] addr Address to be writed in bus
     */
    virtual void setAddrBus(uint32_t addr);
    /** Write a value to Data Bus.
        @param[in] data Data to be writed in bus
     */
    virtual void setDataBus(uint16_t data);
    /** Read a value from Data Bus.
        @return Data readed from bus. */
    virtual uint16_t getDataBus(void);

  protected:
    /* control signals */
    bool f_oe, f_ce, f_we;
};

#endif  // TEST_EMULATOR_CHIP_HPP_
