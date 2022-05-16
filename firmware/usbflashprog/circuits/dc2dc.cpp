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
 * @file circuits/dc2dc.cpp
 * @brief Implementation of the DC To DC Converter Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "circuits/dc2dc.hpp"

constexpr float DC2DC_DEFAULT_ADC_BUFFER_SIZE = 1000;

// ---------------------------------------------------------------------------

Dc2DcConfig::Dc2DcConfig():
        pwmPin(0xFF),
        pwmFreq(Pwm::PWM_DEFAULT_FREQ),
        adcVref(Adc::DEFAULT_VREF),
        adcChannel(0xFF),
        divider(1.0f),
        calibration(0.0f),
        pwmMinDuty(PWM_MIN_DUTY_CYCLE_DEFAULT),
        pwmMaxDuty(PWM_MAX_DUTY_CYCLE_DEFAULT),
        pwmSlowStepDuty(PWM_SLOW_STEP_DUTY_CYCLE_DEFAULT),
        pwmFastStepDuty(PWM_FAST_STEP_DUTY_CYCLE_DEFAULT),
        pwmToleranceToFast(PWM_TOLERANCE_TO_FAST_DEFAULT),
        vTolerance(DC2DC_VOUT_TOLERANCE_DEFAULT) {}

Dc2DcConfig::Dc2DcConfig(uint pwmPin, uint adcChannel, float divider,
                         float calibration, uint32_t pwmFreq, float adcVref,
                         float pwmMinDuty, float pwmMaxDuty,
                         float pwmSlowStepDuty, float pwmFastStepDuty,
                         float pwmToleranceToFast, float vTolerance):
        pwmPin(pwmPin),
        pwmFreq(pwmFreq),
        adcVref(adcVref),
        adcChannel(adcChannel),
        divider(divider),
        calibration(calibration),
        pwmMinDuty(pwmMinDuty),
        pwmMaxDuty(pwmMaxDuty),
        pwmSlowStepDuty(pwmSlowStepDuty),
        pwmFastStepDuty(pwmFastStepDuty),
        pwmToleranceToFast(pwmToleranceToFast),
        vTolerance(vTolerance) {}

Dc2DcConfig& Dc2DcConfig::operator=(const Dc2DcConfig& src) {
    this->pwmPin             =             src.pwmPin;
    this->pwmFreq            =            src.pwmFreq;
    this->adcVref            =            src.adcVref;
    this->adcChannel         =         src.adcChannel;
    this->divider            =            src.divider;
    this->calibration        =        src.calibration;
    this->pwmMinDuty         =         src.pwmMinDuty;
    this->pwmMaxDuty         =         src.pwmMaxDuty;
    this->pwmSlowStepDuty    =    src.pwmSlowStepDuty;
    this->pwmFastStepDuty    =    src.pwmFastStepDuty;
    this->pwmToleranceToFast = src.pwmToleranceToFast;
    this->vTolerance         =         src.vTolerance;
    return *this;
}

bool operator==(const Dc2DcConfig& a, const Dc2DcConfig& b) {
    return (a.pwmPin             ==             b.pwmPin &&
            a.pwmFreq            ==            b.pwmFreq &&
            a.adcVref            ==            b.adcVref &&
            a.adcChannel         ==         b.adcChannel &&
            a.divider            ==            b.divider &&
            a.calibration        ==        b.calibration &&
            a.pwmMinDuty         ==         b.pwmMinDuty &&
            a.pwmMaxDuty         ==         b.pwmMaxDuty &&
            a.pwmSlowStepDuty    ==    b.pwmSlowStepDuty &&
            a.pwmFastStepDuty    ==    b.pwmFastStepDuty &&
            a.pwmToleranceToFast == b.pwmToleranceToFast &&
            a.vTolerance         ==         b.vTolerance);
}

bool operator!=(const Dc2DcConfig& a, const Dc2DcConfig& b) {
    return !operator==(a, b);
}

// ---------------------------------------------------------------------------

Dc2Dc::Dc2Dc(): adc_(nullptr), pwm_(nullptr),
                vTarget_(0.0f), vActual_(0.0f), dutyActual_(0.0f) {}

Dc2Dc::Dc2Dc(const Dc2DcConfig& config): Dc2Dc() {
    configure(config);
}

Dc2Dc::~Dc2Dc() {
    stop();
    if (adc_) { delete adc_; }
    if (pwm_) { delete pwm_; }
}

void Dc2Dc::configure(const Dc2DcConfig& config) {
    bool configPwmPinEq  = (config.pwmPin  ==  config_.pwmPin);
    bool configAdcVrefEq = (config.adcVref == config_.adcVref);
    if (!configPwmPinEq || !configAdcVrefEq) { stop(); }
    if (!configAdcVrefEq && adc_) {
        delete adc_;
        adc_ = nullptr;
    }
    if (!configPwmPinEq  && pwm_) {
        delete pwm_;
        pwm_ = nullptr;
    }
    config_ = config;
    if (!adc_) { adc_ = new Adc(config_.adcVref); }
    if (!pwm_) { pwm_ = new Pwm(config_.pwmPin);  }
    pwm_->setFreq(config_.pwmFreq);
}

bool Dc2Dc::start() {
    if (isRunning()) { return true; }
    if (!isValidConfig_()) { return false; }
    pwm_->setDuty(config_.pwmMinDuty);
    pwm_->start();
    return true;
}

bool Dc2Dc::stop() {
    if (!isRunning()) { return true; }
    if (!isValidConfig_()) { return false; }
    pwm_->stop();
    return true;
}

bool Dc2Dc::isRunning() const {
    if (!pwm_) { return false; }
    return pwm_->isRunning();
}

void Dc2Dc::adjust() {
    if (!isRunning()) {
        vActual_ = 0.0f;
        dutyActual_ = 0.0f;
        return;
    }
    vActual_ = measureV_();
    float duty = dutyActual_;
    float vTargetMin = vTarget_ * (1.0f - config_.vTolerance);
    float vTargetMax = vTarget_ * (1.0f + config_.vTolerance);
    if (vActual_ < vTargetMin * (1.0f - config_.pwmToleranceToFast)) {
        duty += config_.pwmFastStepDuty;
    } else if (vActual_ > vTargetMax * (1.0f + config_.pwmToleranceToFast)) {
        duty -= config_.pwmFastStepDuty;
    } else if (vActual_ < vTargetMin) {
        duty += config_.pwmSlowStepDuty;
    } else if (vActual_ > vTargetMax) {
        duty -= config_.pwmSlowStepDuty;
    }
    if (duty > config_.pwmMaxDuty) { duty = config_.pwmMaxDuty; }
    if (duty < config_.pwmMinDuty) { duty = config_.pwmMinDuty; }
    if (duty != dutyActual_) {
        dutyActual_ = duty;
        pwm_->setDuty(dutyActual_);
    }
}

void Dc2Dc::setV(float v) {
    if (v < 0.0f) { v = 0.0f; }
    if (vTarget_ == v) { return; }
    vTarget_ = v;
}

float Dc2Dc::getV() const {
    return vActual_;
}

float Dc2Dc::getVTarget() const {
    return vTarget_;
}

float Dc2Dc::getDuty() const {
    return dutyActual_;
}

Dc2DcConfig Dc2Dc::getConfig() const {
    return config_;
}

bool Dc2Dc::isValidConfig_() const {
    return (config_.pwmPin != 0xFF
            && config_.pwmFreq != 0.0f
            && config_.adcChannel != 0xFF);
}

float Dc2Dc::measureV_() const {
    return (adc_->capture(config_.adcChannel, DC2DC_DEFAULT_ADC_BUFFER_SIZE)
        * config_.divider + config_.calibration);
}
