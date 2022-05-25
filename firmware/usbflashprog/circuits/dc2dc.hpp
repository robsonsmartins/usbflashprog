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
 * @file circuits/dc2dc.hpp
 * @brief Header of the DC To DC Converter Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef CIRCUITS_DC2DC_HPP_
#define CIRCUITS_DC2DC_HPP_

#include "hal/adc.hpp"
#include "hal/pwm.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Defines the configuration fields for a Dc2Dc class.
 */
typedef struct Dc2DcConfig {
    /** 
     * @brief Pin number of the PWM.
     * @details Default: 0xFF (not assigned).
     */
    uint pwmPin;
    /** 
     * @brief Frequency of the PWM, in Hertz.
     * @details Default: Pwm::PWM_DEFAULT_FREQ.
     */
    uint32_t pwmFreq;
    /**
     * @brief Reference voltage of the ADC, in Volts.
     * @details Default: Adc::DEFAULT_VREF.
     */
    float adcVref;
    /**
     * @brief Number of the ADC channel (0 to 3).
     * @details Default: 0xFF (not assigned).
     */
    uint adcChannel;
    /** 
     * @brief Feedback divider in output of DC2DC converter.
     * @details Typical formula: Divider = (R1 + R2) / R2.<br/>
     *  Default: 1.0.
     */ 
    float divider;
    /** 
     * @brief Minimal duty cycle value for the PWM, in %.
     * @details Default: PWM_MIN_DUTY_CYCLE_DEFAULT.
     */
    float pwmMinDuty;
    /** 
     * @brief Maximum duty cycle value for the PWM, in %.
     * @details Default: PWM_MAX_DUTY_CYCLE_DEFAULT.
     */
    float pwmMaxDuty;
    /** 
     * @brief Step of slow increment/decrement for the PWM duty cycle.
     * @details Used when actual voltage is near to the target voltage.<br/>
     *  Default: PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT.
     */
    float pwmSlowStepDuty;
    /** 
     * @brief Step of fast increment/decrement for the PWM duty cycle.
     * @details Used when actual voltage is far to the target voltage.<br/>
     *  Default: PWM_FAST_STEP_DUTY_CYCLE_DEFAULT.
     */
    float pwmFastStepDuty;
    /** 
     * @brief Tolerance between actual and target voltages to consider
     *  them far.
     * @details Used formula:
     *  <code>distance = |actual - targetV * (1.0 +- vTolerance) *
     *  (1.0 +- pwmToleranceToFast)|</code><br/>
     *  Default: PWM_TOLERANCE_TO_FAST_DEFAULT.
     */
    float pwmToleranceToFast;
    /** 
     * @brief Tolerance voltage for DC2DC output.
     * @details The output voltage will vary within tolerance limits.<br/>
     *  Default: DC2DC_VOUT_TOLERANCE_DEFAULT.
     */
    float vTolerance;
    /**
     * @brief Constructor.
     * @details Assumes defaults for all fields.
     */
    Dc2DcConfig();
    /**
     * @brief Constructor.
     * @param pwmPin Pin number of the PWM.
     * @param adcChannel Number of the ADC channel (0 to 3).
     * @param divider Feedback divider in output of DC2DC converter.
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
    explicit Dc2DcConfig(uint pwmPin, uint adcChannel,
                float divider = 1.0f,
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
     * @param src Dc2DcConfig source object.
     * @return Reference for this.
     */
    Dc2DcConfig& operator=(const Dc2DcConfig& src);
    /** @brief Default value for minimal duty cycle value for the PWM, in %. */
    static constexpr float PWM_MIN_DUTY_CYCLE_DEFAULT = 0.0f;
    /** @brief Default value for maximum duty cycle value for the PWM, in %. */
    static constexpr float PWM_MAX_DUTY_CYCLE_DEFAULT = 95.0f;
    /** 
     * @brief Default value for step of slow increment/decrement for the
     *  PWM duty cycle.
     */
    static constexpr float PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT = 0.1f;
    /** 
     * @brief Default value for step of fast increment/decrement for the
     *  PWM duty cycle.
     */
    static constexpr float PWM_FAST_STEP_DUTY_CYCLE_DEFAULT = 1.0f;
    /** 
     * @brief Default value for Tolerance between actual and target voltages
     * to consider them far.
     */
    static constexpr float PWM_TOLERANCE_TO_FAST_DEFAULT = 0.1f;
    /** @brief Default value for tolerance voltage for DC2DC output. */
    static constexpr float DC2DC_VOUT_TOLERANCE_DEFAULT = 0.05f;
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const Dc2DcConfig& a, const Dc2DcConfig& b);
    /**
     * @brief Unequality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain different values, false otherwise.
     */
    friend bool operator!=(const Dc2DcConfig& a, const Dc2DcConfig& b);
} Dc2DcConfig;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief DC To DC Converter Class.
 * @details The purpose of this class is to provide the methods to manipulate
 *  a DC To DC converter via software (using the CPU's PWM and ADC modules).
 * @nosubgrouping
 */
class Dc2Dc {
 public:
    /**
     * @brief Constructor.
     * @details Assumes a default value for the configuration.
     */
    Dc2Dc();
    /**
     * @brief Constructor.
     * @param config Configuration data.
     */
    explicit Dc2Dc(const Dc2DcConfig& config);
    /** @brief Destructor. */
    ~Dc2Dc();
    /**
     * @brief Configures the DC to DC converter.
     * @details If the converter is running, then it's stopped before.
     * @param config Configuration data.
     */
    void configure(const Dc2DcConfig& config);
    /**
     * @brief Gets the current configuration data.
     * @return Copy of the current configuration data.
     */
    Dc2DcConfig getConfig() const;
    /**
     * @brief Sets the calibration value (offset) of the output
     *  of DC2DC converter.
     * @param value Calibration value.
     */
    void setCalibration(float value);
    /**
     * @brief Returns the calibration value of the output
     *  of DC2DC converter.
     * @return Calibration value.
     */
    float getCalibration() const;
    /**
     * @brief Starts the DC to DC converter.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise.
     */
    bool start();
    /**
     * @brief Stops the DC to DC converter.
     * @details If the configuration is invalid, then fails.
     * @return True if success, false otherwise.
     */
    bool stop();
    /**
     * @brief Returns if the converter is running.
     * @return True if converter is running, false otherwise.
     */
    bool isRunning() const;
    /**
     * @brief Adjusts the PWM duty cycle value to the output
     *  reaches the target voltage.
     * @details This method must be called periodically in a loop,
     *  so that the DC to DC converter works properly.
     */
    void adjust();
    /**
     * @brief Sets the target output voltage of the converter.
     * @param v Target output voltage, in Volts.
     */
    void setV(float v);
    /**
     * @brief Gets the current output voltage of the converter.
     * @return Current output voltage, in Volts.
     */
    float getV() const;
    /**
     * @brief Gets the target output voltage of the converter.
     * @return Target output voltage, in Volts.
     */
    float getVTarget() const;
    /**
     * @brief Gets the current duty cycle of the PWM.
     * @return Current duty cycle value, in percent.
     */
    float getDuty() const;

 private:
    /* @brief Configuration data. */
    Dc2DcConfig config_;
    /* @brief Adc instance. */
    Adc* adc_;
    /* @brief Pwm instance. */
    Pwm* pwm_;
    /* @brief Target voltage. */
    float vTarget_;
    /* @brief Actual voltage. */
    float vActual_;
    /* @brief Actual duty cycle. */
    float dutyActual_;
    /* @brief Calibration value (offset) in output of DC2DC converter. */
    float calibration_;
    /*
     * @brief Returns if configuration data is valid.
     * @return True if configuration data is valid, false otherwise.
     */
    bool isValidConfig_() const;
    /*
     * @brief Returns the voltage measured by the ADC.
     * @return Actual voltage measured, in Volts.
     */
    float measureV_() const;
};

#endif  // CIRCUITS_DC2DC_HPP_
