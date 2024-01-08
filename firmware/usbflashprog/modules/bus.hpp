// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
 * @file modules/bus.hpp
 * @brief Header of the Bus Classes.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MODULES_BUS_HPP_
#define MODULES_BUS_HPP_

#include "hal/gpio.hpp"
#include "circuits/74hc595.hpp"
#include "circuits/74hc165.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a Control Bus class.
 */
typedef struct CtrlBusConfig {
    /**
     * @brief Pin number of the Chip Enable.
     * @details Default: 0xFF (not assigned).
     */
    uint cePin;
    /**
     * @brief Pin number of the Output Enable.
     * @details Default: 0xFF (not assigned).
     */
    uint oePin;
    /**
     * @brief Pin number of the Write Enable.
     * @details Default: 0xFF (not assigned).
     */
    uint wePin;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    CtrlBusConfig();
    /**
     * @brief Assign Operator.
     * @param src CtrlBusConfig source object.
     * @return Reference for this.
     */
    CtrlBusConfig& operator=(const CtrlBusConfig& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const CtrlBusConfig& a, const CtrlBusConfig& b);
} CtrlBusConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a Data Bus class.
 */
typedef struct DataBusConfig {
    /**
     * @brief Pin number of the Data Shift Register SIN signal.
     * @details Default: 0xFF (not assigned).
     */
    uint dSinPin;
    /**
     * @brief Pin number of the Data Shift Register CLK signal.
     * @details Default: 0xFF (not assigned).
     */
    uint dClkPin;
    /**
     * @brief Pin number of the Data Shift Register CLR signal.
     * @details Default: 0xFF (not assigned).
     */
    uint dClrPin;
    /**
     * @brief Pin number of the Data Shift Register RCK signal.
     * @details Default: 0xFF (not assigned).
     */
    uint dRckPin;
    /**
     * @brief Pin number of the Data Shift Register SOUT signal (Q7).
     * @details Default: 0xFF (not assigned).
     */
    uint dSoutPin;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    DataBusConfig();
    /**
     * @brief Assign Operator.
     * @param src DataBusConfig source object.
     * @return Reference for this.
     */
    DataBusConfig& operator=(const DataBusConfig& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const DataBusConfig& a, const DataBusConfig& b);
} DataBusConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a Address Bus class.
 */
typedef struct AddrBusConfig {
    /**
     * @brief Pin number of the Address Shift Register SIN signal.
     * @details Default: 0xFF (not assigned).
     */
    uint aSinPin;
    /**
     * @brief Pin number of the Address Shift Register CLK signal.
     * @details Default: 0xFF (not assigned).
     */
    uint aClkPin;
    /**
     * @brief Pin number of the Address Shift Register CLR signal.
     * @details Default: 0xFF (not assigned).
     */
    uint aClrPin;
    /**
     * @brief Pin number of the Address Shift Register RCK signal.
     * @details Default: 0xFF (not assigned).
     */
    uint aRckPin;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    AddrBusConfig();
    /**
     * @brief Assign Operator.
     * @param src AddrBusConfig source object.
     * @return Reference for this.
     */
    AddrBusConfig& operator=(const AddrBusConfig& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const AddrBusConfig& a, const AddrBusConfig& b);
} AddrBusConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Control Bus Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Control Bus Module.
 * @nosubgrouping
 */
class CtrlBus {
  public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    CtrlBus();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit CtrlBus(const CtrlBusConfig& config);
    /** @brief Destructor. */
    ~CtrlBus();
    /**
     * @brief Configures the Control Bus.
     * @param config Configuration data.
     */
    void configure(const CtrlBusConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    CtrlBusConfig getConfig() const;
    /**
     * @brief Sets the CE pin.
     * @param value Value to set.
     */
    void setCE(bool value = true);
    /**
     * @brief Sets the OE pin.
     * @param value Value to set.
     */
    void setOE(bool value = true);
    /**
     * @brief Sets the WE pin.
     * @param value Value to set.
     */
    void setWE(bool value = true);

  private:
    /* @brief Gpio Handler instance. */
    Gpio gpio_;
    /* @brief Configuration data. */
    CtrlBusConfig config_;
    /* @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise. */
    bool isValidConfig_() const;
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Data Bus Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Data Bus Module.
 * @nosubgrouping
 */
class DataBus {
  public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    DataBus();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit DataBus(const DataBusConfig& config);
    /** @brief Destructor. */
    ~DataBus();
    /**
     * @brief Configures the Data Bus.
     * @param config Configuration data.
     */
    void configure(const DataBusConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    DataBusConfig getConfig() const;
    /**
     * @brief Writes a byte to the Data Bus.
     * @param value Value to write.
     * @return True if success. False otherwise.
     */
    bool writeByte(uint8_t value);
    /**
     * @brief Writes a word to the Data Bus.
     * @param value Value to write.
     * @return True if success. False otherwise.
     */
    bool writeWord(uint16_t value);
    /**
     * @brief Reads a byte from the Data Bus.
     * @return Readed value or 0 if an error occurs.
     */
    uint8_t readByte(void);
    /**
     * @brief Reads a word from the Data Bus.
     * @return Readed value or 0 if an error occurs.
     */
    uint16_t readWord(void);

  private:
    /* @brief Stores current address. */
    uint16_t data_;
    /* @brief Data Output Shift Register. */
    HC595 outRegister_;
    /* @brief Data Input Shift Register. */
    HC165 inRegister_;
    /* @brief Configuration data. */
    DataBusConfig config_;
    /* @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise. */
    bool isValidConfig_() const;
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Address Bus Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Address Bus Module.
 * @nosubgrouping
 */
class AddrBus {
  public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    AddrBus();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit AddrBus(const AddrBusConfig& config);
    /** @brief Destructor. */
    ~AddrBus();
    /**
     * @brief Configures the Data Bus.
     * @param config Configuration data.
     */
    void configure(const AddrBusConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    AddrBusConfig getConfig() const;
    /**
     * @brief Writes a byte to the Address Bus.
     * @param value Value to write.
     * @return True if success. False otherwise.
     */
    bool writeByte(uint8_t value);
    /**
     * @brief Writes a word to the Address Bus.
     * @param value Value to write.
     * @return True if success. False otherwise.
     */
    bool writeWord(uint16_t value);
    /**
     * @brief Writes a double word to the Address Bus.
     * @param value Value to write.
     * @return True if success. False otherwise.
     */
    bool writeDWord(uint32_t value);
    /**
     * @brief Increments the Address Bus.
     * @return True if success. False otherwise.
     */
    bool increment();

  private:
    /* @brief Stores current address. */
    uint32_t address_;
    /* @brief Address Output Shift Register. */
    HC595 outRegister_;
    /* @brief Configuration data. */
    AddrBusConfig config_;
    /* @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise. */
    bool isValidConfig_() const;
};

#endif  // MODULES_BUS_HPP_
