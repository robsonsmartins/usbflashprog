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
#include "circuits/dc2dc.hpp"
#include "circuits/74hc595.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a VGenerator class.
 */
typedef struct VGenConfig: Dc2DcConfig {
    /** 
     * @brief Pin number of the Control Voltage.
     * @details Default: 0xFF (not assigned).
     */
    uint ctrlPin;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    VGenConfig();
    /**
     * @brief Constructor.
     * @param pwmPin Pin number of the PWM.
     * @param adcChannel Number of the ADC channel (0 to 3).
     * @param ctrlPin Pin number of the Control Voltage.
     * @param divider Feedback divider in output of DC2DC converter.
     * @param calibration Calibration value (offset) in output of DC2DC converter.
     * @param pwmFreq Frequency of the PWM, in Hertz.
     * @param adcVref Reference voltage of the ADC, in Volts.
     * @param pwmMinDuty Minimal duty cycle value for the PWM, in %.
     * @param pwmMaxDuty Maximum duty cycle value for the PWM, in %.
     * @param pwmSlowStepDuty Step of slow increment/decrement for the PWM duty cycle.
     * @param pwmFastStepDuty Step of fast increment/decrement for the PWM duty cycle.
     * @param pwmToleranceToFast Tolerance between actual and target voltages to consider
     *  them far.
     * @param vTolerance Tolerance voltage for DC2DC output.
     */
    explicit VGenConfig(uint pwmPin, uint adcChannel, uint ctrlPin,
                       float divider = 1.0f,
                       float calibration = 0.0f,
                       uint32_t pwmFreq = Pwm::PWM_DEFAULT_FREQ,
                       float adcVref = Adc::DEFAULT_VREF,
                       float pwmMinDuty = PWM_MIN_DUTY_CYCLE_DEFAULT,
                       float pwmMaxDuty = PWM_MAX_DUTY_CYCLE_DEFAULT,
                       float pwmSlowStepDuty = PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT,
                       float pwmFastStepDuty = PWM_FAST_STEP_DUTY_CYCLE_DEFAULT,
                       float pwmToleranceToFast = PWM_TOLERANCE_TO_FAST_DEFAULT,
                       float vTolerance = DC2DC_VOUT_TOLERANCE_DEFAULT);
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
} VGenConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a VddGenerator class.
 */
typedef struct VddConfig: VGenConfig {
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
     * @brief Constructor.
     * @param pwmPin Pin number of the PWM.
     * @param adcChannel Number of the ADC channel (0 to 3).
     * @param ctrlPin Pin number of the Control Voltage.
     * @param onVppPin Pin number of the VDD on VPP signal.
     * @param divider Feedback divider in output of DC2DC converter.
     * @param calibration Calibration value (offset) in output of DC2DC converter.
     * @param pwmFreq Frequency of the PWM, in Hertz.
     * @param adcVref Reference voltage of the ADC, in Volts.
     * @param pwmMinDuty Minimal duty cycle value for the PWM, in %.
     * @param pwmMaxDuty Maximum duty cycle value for the PWM, in %.
     * @param pwmSlowStepDuty Step of slow increment/decrement for the PWM duty cycle.
     * @param pwmFastStepDuty Step of fast increment/decrement for the PWM duty cycle.
     * @param pwmToleranceToFast Tolerance between actual and target voltages to consider
     *  them far.
     * @param vTolerance Tolerance voltage for DC2DC output.
     */
    explicit VddConfig(uint pwmPin, uint adcChannel,
                       uint ctrlPin, uint onVppPin,
                       float divider = 1.0f,
                       float calibration = 0.0f,
                       uint32_t pwmFreq = Pwm::PWM_DEFAULT_FREQ,
                       float adcVref = Adc::DEFAULT_VREF,
                       float pwmMinDuty = PWM_MIN_DUTY_CYCLE_DEFAULT,
                       float pwmMaxDuty = PWM_MAX_DUTY_CYCLE_DEFAULT,
                       float pwmSlowStepDuty = PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT,
                       float pwmFastStepDuty = PWM_FAST_STEP_DUTY_CYCLE_DEFAULT,
                       float pwmToleranceToFast = PWM_TOLERANCE_TO_FAST_DEFAULT,
                       float vTolerance = DC2DC_VOUT_TOLERANCE_DEFAULT);
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
    /**
     * @brief Unequality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain different values, false otherwise.
     */
    friend bool operator!=(const VddConfig& a, const VddConfig& b);
} VddConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a VppGenerator class.
 */
typedef struct VppConfig: VGenConfig {
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
     * @brief Constructor.
     * @param pwmPin Pin number of the PWM.
     * @param adcChannel Number of the ADC channel (0 to 3).
     * @param ctrlPin Pin number of the Control Voltage.
     * @param vcSinPin Pin number of the VControl Shift Register SIN signal.
     * @param vcClkPin Pin number of the VControl Shift Register CLK signal.
     * @param vcClrPin Pin number of the VControl Shift Register CLR signal.
     * @param vcRckPin Pin number of the VControl Shift Register RCK signal.
     * @param divider Feedback divider in output of DC2DC converter.
     * @param calibration Calibration value (offset) in output of DC2DC converter.
     * @param pwmFreq Frequency of the PWM, in Hertz.
     * @param adcVref Reference voltage of the ADC, in Volts.
     * @param pwmMinDuty Minimal duty cycle value for the PWM, in %.
     * @param pwmMaxDuty Maximum duty cycle value for the PWM, in %.
     * @param pwmSlowStepDuty Step of slow increment/decrement for the PWM duty cycle.
     * @param pwmFastStepDuty Step of fast increment/decrement for the PWM duty cycle.
     * @param pwmToleranceToFast Tolerance between actual and target voltages to consider
     *  them far.
     * @param vTolerance Tolerance voltage for DC2DC output.
     */
    explicit VppConfig(uint pwmPin, uint adcChannel,
                       uint ctrlPin, uint vcSinPin, uint vcClkPin,
                       uint vcClrPin, uint vcRckPin,
                       float divider = 1.0f,
                       float calibration = 0.0f,
                       uint32_t pwmFreq = Pwm::PWM_DEFAULT_FREQ,
                       float adcVref = Adc::DEFAULT_VREF,
                       float pwmMinDuty = PWM_MIN_DUTY_CYCLE_DEFAULT,
                       float pwmMaxDuty = PWM_MAX_DUTY_CYCLE_DEFAULT,
                       float pwmSlowStepDuty = PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT,
                       float pwmFastStepDuty = PWM_FAST_STEP_DUTY_CYCLE_DEFAULT,
                       float pwmToleranceToFast = PWM_TOLERANCE_TO_FAST_DEFAULT,
                       float vTolerance = DC2DC_VOUT_TOLERANCE_DEFAULT);
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
    /**
     * @brief Unequality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain different values, false otherwise.
     */
    friend bool operator!=(const VppConfig& a, const VppConfig& b);
} VppConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Voltage Generator Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a Voltage Generator (based in a DC2DC converter and GPIO pins to control
 *  - on/off - the output).
 * @nosubgrouping
 */
class VGenerator {
 public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    VGenerator();
    /** @brief Destructor. */
    virtual ~VGenerator();
    /**
     * @brief Starts the Voltage Generator.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise.
     */
    virtual bool start();
    /**
     * @brief Stops the Voltage Generator.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise.
     */
    virtual void stop();
    /**
     * @brief Returns if the Voltage Generator is running.
     * @return True if Voltage Generator is running, false otherwise.
     */
    virtual bool isRunning() const;
    /**
     * @brief Enables the output of the Voltage Generator.
     * @return True if success, false otherwise.
     */
    virtual bool on();
    /**
     * @brief Disables the output of the Voltage Generator.
     */
    virtual void off();
    /**
     * @brief Returns if output of the Voltage Generator is enabled.
     * @return True if output of the Voltage Generator is enabled,
     *  false otherwise.
     */
    virtual bool isOn() const;
    /**
     * @brief Adjusts the PWM duty cycle value to the output
     *  reaches the target voltage.
     * @details This method must be called periodically in a loop,
     *  so that the Voltage Generator works properly.
     */
    virtual void adjust();
    /**
     * @brief Sets the target output voltage of the Voltage Generator.
     * @param v Target output voltage, in Volts.
     */
    virtual void setV(float v);
    /**
     * @brief Gets the current output voltage of the Voltage Generator.
     * @return Current output voltage, in Volts.
     */
    virtual float getV() const;
    /**
     * @brief Gets the target output voltage of the Voltage Generator.
     * @return Target output voltage, in Volts.
     */
    virtual float getVTarget() const;
    /**
     * @brief Gets the current duty cycle of the PWM.
     * @return Current duty cycle value, in percent.
     */
    virtual float getDuty() const;
    /**
     * @brief Starts the calibration process.
     * @details Sets the calibration value to zero, and sets the output voltage
     *  to value of the <i>voltage</i> parameter.
     * @param voltage Calibration target voltage, in Volts.
     * @return True if success, false otherwise.
     */
    virtual bool initCalibration(float voltage);
    /**
     * @brief Finishes the calibration process.
     * @details Calculates the calibration value.
     * @param measure Measured voltage, in Volts.
     * @return True if success, false otherwise.
     */
    virtual bool finishCalibration(float measure);

 protected:
    /* @brief Configuration data. */
    VGenConfig config_;
    /* @brief DC2DC converter. */
    Dc2Dc dc2dc_;
    /* @brief GPIO manager. */
    Gpio gpio_;
    /* @brief Output status flag (On/Off). */
    bool on_;
    /*
     * @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise.
     */
    virtual bool isValidConfig_() const;
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
class VddGenerator: public VGenerator {
 public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    VddGenerator();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit VddGenerator(const VddConfig& config);
    /** @brief Destructor. */
    ~VddGenerator();
    /**
     * @brief Configures the Voltage Generator.
     * @details If the Voltage Generator is running, then it's stopped before.
     * @param config Configuration data.
     */
    void configure(const VddConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    VddConfig getConfig() const;
    /**
     * @brief Enables/Disables VDD on VPP.
     * @param status If true enables VDD on VPP. False otherwise.
     * @return True if success, false otherwise.
     */
    bool onVpp(bool status = true);
    /**
     * @brief Returns if VDD on VPP is enabled.
     * @return True if VDD on VPP is enabled, false otherwise.
     */
    bool isOnVpp() const;
    /**
     * @brief Starts the calibration process.
     * @details Sets the calibration value to zero, and sets the output voltage
     *  to value of the <i>voltage</i> parameter.
     * @param voltage Calibration target voltage, in Volts.
     * @return True if success, false otherwise.
     */
    bool initCalibration(float voltage = 5.0f);
    /**
     * @brief Finishes the calibration process.
     * @details Calculates the calibration value and stops the Voltage Generator.
     * @param measure Measured voltage, in Volts.
     * @return True if success, false otherwise.
     */
    bool finishCalibration(float measure);

 private:
    /* @brief Configuration data. */
    VddConfig config_;
    /* @brief VDD on VPP status flag. */
    bool onVpp_;
    /*
     * @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise.
     */
    bool isValidConfig_() const;
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
class VppGenerator: public VGenerator {
 public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    VppGenerator();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit VppGenerator(const VppConfig& config);
    /** @brief Destructor. */
    ~VppGenerator();
    /**
     * @brief Configures the Voltage Generator.
     * @details If the Voltage Generator is running, then it's stopped before.
     * @param config Configuration data.
     */
    void configure(const VppConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    VppConfig getConfig() const;
    /**
     * @brief Enables/Disables VPP on A9.
     * @param status If true enables VPP on A9. False otherwise.
     * @return True if success, false otherwise.
     */
    bool onA9(bool status = true);
    /**
     * @brief Enables/Disables VPP on A18.
     * @param status If true enables VPP on A18. False otherwise.
     * @return True if success, false otherwise.
     */
    bool onA18(bool status = true);
    /**
     * @brief Enables/Disables VPP on CE.
     * @param status If true enables VPP on CE. False otherwise.
     * @return True if success, false otherwise.
     */
    bool onCE(bool status = true);
    /**
     * @brief Enables/Disables VPP on OE.
     * @param status If true enables VPP on OE. False otherwise.
     * @return True if success, false otherwise.
     */
    bool onOE(bool status = true);
    /**
     * @brief Enables/Disables VPP on WE.
     * @param status If true enables VPP on WE. False otherwise.
     * @return True if success, false otherwise.
     */
    bool onWE(bool status = true);
    /**
     * @brief Returns if VPP on A9 is enabled.
     * @return True if VPP on A9 is enabled, false otherwise.
     */
    bool isOnA9() const;
    /**
     * @brief Returns if VPP on A18 is enabled.
     * @return True if VPP on A18 is enabled, false otherwise.
     */
    bool isOnA18() const;
    /**
     * @brief Returns if VPP on CE is enabled.
     * @return True if VPP on CE is enabled, false otherwise.
     */
    bool isOnCE() const;
    /**
     * @brief Returns if VPP on OE is enabled.
     * @return True if VPP on OE is enabled, false otherwise.
     */
    bool isOnOE() const;
    /**
     * @brief Returns if VPP on WE is enabled.
     * @return True if VPP on WE is enabled, false otherwise.
     */
    bool isOnWE() const;
    /**
     * @brief Starts the calibration process.
     * @details Sets the calibration value to zero, and sets the output voltage
     *  to value of the <i>voltage</i> parameter.
     * @param voltage Calibration target voltage, in Volts.
     * @return True if success, false otherwise.
     */
    bool initCalibration(float voltage = 12.0f);
    /**
     * @brief Finishes the calibration process.
     * @details Calculates the calibration value and stops the Voltage Generator.
     * @param measure Measured voltage, in Volts.
     * @return True if success, false otherwise.
     */
    bool finishCalibration(float measure);

 private:
    /* @brief Configuration data. */
    VppConfig config_;
    /* @brief Voltage Control Shift Register. */
    HC595 vcRegister_;
    /*
     * @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise.
     */
    bool isValidConfig_() const;
};


#endif  // MODULES_VGENERATOR_HPP_
