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
 * @file modules/vgenerator.hpp
 * @brief Header of the Voltage Generator Classes.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MODULES_VGENERATOR_HPP_
#define MODULES_VGENERATOR_HPP_

#include "hal/gpio.hpp"
#include "hal/multicore.hpp"
#include "circuits/dc2dc.hpp"
#include "circuits/74hc595.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a VddGenerator class.
 */
typedef struct VddConfig: Dc2DcConfig {
    /** 
     * @brief Pin number of the Control Voltage.
     * @details Default: 0xFF (not assigned).
     */
    uint ctrlPin;
    /** 
     * @brief Pin number of the VDD on VPP signal.
     * @details Default: 0xFF (not assigned).
     */
    uint onVppPin;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    VddConfig();
    /**
     * @brief Assign Operator.
     * @param src VddConfig source object.
     * @return Reference for this.
     */
    VddConfig& operator=(const VddConfig& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const VddConfig& a, const VddConfig& b);
} VddConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a VppGenerator class.
 */
typedef struct VppConfig: Dc2DcConfig {
    /** 
     * @brief Pin number of the Control Voltage.
     * @details Default: 0xFF (not assigned).
     */
    uint ctrlPin;
    /** 
     * @brief Pin number of the VControl Shift Register SIN signal.
     * @details Default: 0xFF (not assigned).
     */
    uint vcSinPin;
    /** 
     * @brief Pin number of the VControl Shift Register CLK signal.
     * @details Default: 0xFF (not assigned).
     */
    uint vcClkPin;
    /** 
     * @brief Pin number of the VControl Shift Register CLR signal.
     * @details Default: 0xFF (not assigned).
     */
    uint vcClrPin;
    /** 
     * @brief Pin number of the VControl Shift Register RCK signal.
     * @details Default: 0xFF (not assigned).
     */
    uint vcRckPin;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    VppConfig();
    /**
     * @brief Assign Operator.
     * @param src VppConfig source object.
     * @return Reference for this.
     */
    VppConfig& operator=(const VppConfig& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const VppConfig& a, const VppConfig& b);
} VppConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a VGenerator class.
 */
typedef struct VGenConfig {
    /** @brief VddGenerator configuration. */
    VddConfig vdd;
    /** @brief VppGenerator configuration. */
    VppConfig vpp;
    /**
     * @brief Assign Operator.
     * @param src VGenConfig source object.
     * @return Reference for this.
     */
    VGenConfig& operator=(const VGenConfig& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const VGenConfig& a, const VGenConfig& b);
    /**
     * @brief Unequality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain different values, false otherwise.
     */
    friend bool operator!=(const VGenConfig& a, const VGenConfig& b);
} VGenConfig;

// ---------------------------------------------------------------------------

// Forward
class VGenerator;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Generic Voltage Generator Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Voltage Generator (based in a DC2DC converter and GPIO pins to control
 *  - on/off - the output).
 * @nosubgrouping
 */
class GenericGenerator {
 public:
    /**
     * @brief Constructor.
     * @param owner Pointer to owner VGenerator object instance.
     */
    explicit GenericGenerator(const VGenerator *owner);
    /**
     * @brief Returns if the Voltage Generator is running.
     * @return True if Voltage Generator is running, false otherwise.
     */
    virtual bool isRunning() const;
    /**
     * @brief Sets the target output voltage of the Voltage Generator.
     * @param value Target output voltage, in Volts.
     */
    virtual void setV(float value);
    /**
     * @brief Gets the target output voltage of the Voltage Generator.
     * @return Target output voltage, in Volts.
     */
    virtual float getVTarget() const;
    /**
     * @brief Gets the current output voltage of the Voltage Generator.
     * @return Current output voltage, in Volts.
     */
    virtual float getV() const;
    /**
     * @brief Returns the calibration value of the output
     *  of the Voltage Generator.
     * @return Calibration value.
     */
    virtual float getCalibration() const;
    /**
     * @brief Gets the current duty cycle of the PWM.
     * @return Current duty cycle value, in percent.
     */
    virtual float getDuty() const;
    /**
     * @brief Starts the calibration process.
     * @details Sets the calibration value to zero, and sets the output voltage
     *  to value of the <i>reference</i> parameter.
     * @param reference Calibration target voltage, in Volts.
     */
    virtual void initCalibration(float reference);
    /**
     * @brief Finishes the calibration process.
     * @details Calculates and saves the calibration value.
     * @param measure Measured voltage, in Volts.
     */
    virtual void saveCalibration(float measure);
    /** @brief Enables the output of the Voltage Generator. */
    virtual void on() = 0;
    /** @brief Disables the output of the Voltage Generator. */
    virtual void off() = 0;
    /** @brief Toggles the output of the Voltage Generator. */
    virtual void toggle();
    /**
     * @brief Returns if output of the Voltage Generator is enabled.
     * @return True if output of the Voltage Generator is enabled,
     *  false otherwise.
     */
    virtual bool isOn() const;

 protected:
    /* @brief Pointer to VGenerator owner object. */
    VGenerator *owner_;
    /* @brief Dc2Dc converter object. */
    Dc2Dc dc2dc_;
    /* @brief Gpio manager object. */
    Gpio gpio_;
    /* @brief Stores output on/off state. */
    bool on_;
    /* @brief Starts the Voltage Generator.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise. */
    virtual bool start_();
    /* @brief Stops the Voltage Generator. */
    virtual void stop_();
    /* @brief Adjusts the PWM duty cycle value to the output
     *  reaches the target voltage.
     * @details This method must be called periodically in a loop,
     *  so that the Voltage Generator works properly. */
    virtual void adjust_();

  /* Friend functions. */
  friend void second_core(MultiCore& core); // NOLINT
  /* Friend classes. */
  friend class VGenerator;
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Voltage (VDD) Generator Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Voltage (VDD) Generator (based in a buck DC2DC converter and GPIO pins
 *  to control - on/off - the output).
 * @nosubgrouping
 */
class VddGenerator: public GenericGenerator {
 public:
    /**
     * @brief Constructor.
     * @param owner Pointer to owner VGenerator object instance.
     */
    explicit VddGenerator(const VGenerator *owner);
    /** @brief Enables the output of the Voltage Generator. */
    void on();
    /** @brief Disables the output of the Voltage Generator. */
    void off();
    /**
     * @brief Starts the calibration process.
     * @details Sets the calibration value to zero, and sets the output voltage
     *  to value of the <i>reference</i> parameter.
     * @param reference Calibration target voltage, in Volts. Default: 5.0.
     */
    void initCalibration(float reference = 5.0f) override;
    /**
     * @brief Sets/Resets the VDD on VPP pin.
     * @param status True to set pin, false to reset.
     */
    void onVpp(bool status = true);
    /**
     * @brief Gets the VDD on VPP pin status.
     * @return The pin status.
     */
    bool isOnVpp() const;

 private:
    /* @brief Stores VDD on VPP pin state. */
    bool onVpp_;
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Voltage (VPP) Generator Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Voltage (VPP) Generator (based in a boost DC2DC converter, GPIO pins
 *  and one shift register to control - on/off - the output).
 * @nosubgrouping
 */
class VppGenerator: public GenericGenerator {
 public:
    /**
     * @brief Constructor.
     * @param owner Pointer to owner VGenerator object instance.
     */
    explicit VppGenerator(const VGenerator *owner);
    /** @brief Enables the output of the Voltage Generator. */
    void on();
    /** @brief Disables the output of the Voltage Generator. */
    void off();
    /**
     * @brief Starts the calibration process.
     * @details Sets the calibration value to zero, and sets the output voltage
     *  to value of the <i>reference</i> parameter.
     * @param reference Calibration target voltage, in Volts. Default: 12.0.
     */
    void initCalibration(float reference = 12.0f) override;
    /**
     * @brief Sets/Resets the VPP on A9 pin.
     * @param status True to set pin, false to reset.
     */
    void onA9(bool status = true);
    /**
     * @brief Sets/Resets the VPP on A18 pin.
     * @param status True to set pin, false to reset.
     */
    void onA18(bool status = true);
    /**
     * @brief Sets/Resets the VPP on CE pin.
     * @param status True to set pin, false to reset.
     */
    void onCE(bool status = true);
    /**
     * @brief Sets/Resets the VPP on OE pin.
     * @param status True to set pin, false to reset.
     */
    void onOE(bool status = true);
    /**
     * @brief Sets/Resets the VPP on WE pin.
     * @param status True to set pin, false to reset.
     */
    void onWE(bool status = true);
    /**
     * @brief Gets the VPP on A9 pin status.
     * @return The pin status.
     */
    bool isOnA9() const;
    /**
     * @brief Gets the VPP on A18 pin status.
     * @return The pin status.
     */
    bool isOnA18() const;
    /**
     * @brief Gets the VPP on CE pin status.
     * @return The pin status.
     */
    bool isOnCE() const;
    /**
     * @brief Gets the VPP on OE pin status.
     * @return The pin status.
     */
    bool isOnOE() const;
    /**
     * @brief Gets the VPP on WE pin status.
     * @return The pin status.
     */
    bool isOnWE() const;

 protected:
    /* @brief Starts the Voltage Generator.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise. */
    virtual bool start_();
    /* @brief Stops the Voltage Generator. */
    virtual void stop_();

 private:
    /* @brief Voltage Control Pins Shift Register. */
    HC595 vcRegister_;

  /* Friend classes. */
  friend class VGenerator;
};

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Voltage (VPP/VDD) Generator Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Voltage Generator (VPP/VDD) Module.
 * @nosubgrouping
 */
class VGenerator {
 public:
    /** @brief VPP Generator. */
    VppGenerator vpp;
    /** @brief VDD Generator. */
    VddGenerator vdd;
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    VGenerator();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit VGenerator(const VGenConfig& config);
    /** @brief Destructor. */
    ~VGenerator();
    /**
     * @brief Configures the Voltage Generator.
     * @details If the Voltage Generator is running, then it's stopped before.
     * @param config Configuration data.
     */
    void configure(const VGenConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    VGenConfig getConfig() const;
    /**
     * @brief Starts the Voltage Generator.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise.
     */
    bool start();
    /** @brief Stops the Voltage Generator. */
    void stop();
    /**
     * @brief Returns if the Voltage Generator is running.
     * @return True if Voltage Generator is running, false otherwise.
     */
    bool isRunning() const;

 private:
    /* @brief Current status flag. */
    MultiCore::CoreStatus status_;
    /* @brief CPU Multicore manager. */
    MultiCore multicore_;
    /* @brief Configuration data. */
    VGenConfig config_;
    /* @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise. */
    bool isValidConfig_() const;
    /* @brief Reads the calibration data from the flash space. */
    void readCalData_();
    /* @brief Writes the calibration data to the flash space. */
    void writeCalData_();
    /* @brief Calculates the checksum of the buffer.
     * @param buf Pointer to a buffer.
     * @param len Size of buffer, in bytes.
     * @return Checksum of the buffer (one byte size). */
    uint8_t checksum_(const uint8_t *buf, size_t len);

  /* Friend functions. */
  friend void second_core(MultiCore& core); // NOLINT
  /* Friend classes. */
  friend class GenericGenerator;
  friend class VddGenerator;
  friend class VppGenerator;
};

#endif  // MODULES_VGENERATOR_HPP_
