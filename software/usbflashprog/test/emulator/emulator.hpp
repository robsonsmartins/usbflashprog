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
 * @file test/emulator/emulator.hpp
 * @brief Header of Emulator Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef TEST_EMULATOR_EMULATOR_HPP_
#define TEST_EMULATOR_EMULATOR_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QList>

#include "mock/qserialport.hpp"
#include "chip.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief List of serial ports info.
 */
typedef QList<QSerialPortInfo> TSerialPortList;

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Emulator Class.
 * @details The purpose of this class is to run commands and get response
 *   via Chip Emulator (for tests).
 * @nosubgrouping
 */
class Emulator : public QObject {
    Q_OBJECT

  public:
    /** @brief Device settings. */
    typedef struct TDeviceSettings {
        /** @brief Skip Write 0xFF. */
        bool skipFF;
        /** @brief Prog with VPP on. */
        bool progWithVpp;
        /** @brief VPP/~OE Pin. */
        bool vppOePin;
        /** @brief ~PGM/~CE Pin. */
        bool pgmCePin;
        /** @brief PGM positive. */
        bool pgmPositive;
    } TDeviceSettings;

  public:
    /** @copydoc Runner::Runner(QObject*) */
    explicit Emulator(QObject* parent = nullptr);
    /** @copydoc Runner::~Runner() */
    ~Emulator();
    /** @copydoc Runner::list() */
    TSerialPortList list() const;
    /** @copydoc Runner::open(const QString&) */
    bool open(const QString& path);
    /** @copydoc Runner::close() */
    void close();
    /** @copydoc Runner::isOpen() */
    bool isOpen() const;
    /** @copydoc Runner::hasError() */
    bool hasError() const;
    /** @copydoc Runner::getPath() */
    QString getPath() const;
    /** @copydoc Runner::getTimeOut() */
    uint32_t getTimeOut() const;
    /** @copydoc Runner::setTimeOut(uint32_t) */
    void setTimeOut(uint32_t value);
    /** @copydoc Runner::nop() */
    bool nop();
    /** @copydoc Runner::vddCtrl(bool) */
    bool vddCtrl(bool on = true);
    /** @copydoc Runner::vddSet(float) */
    bool vddSet(float value);
    /** @copydoc Runner::vddGet() */
    float vddGet();
    /** @copydoc Runner::vddGetDuty() */
    float vddGetDuty();
    /** @copydoc Runner::vddInitCal() */
    bool vddInitCal();
    /** @copydoc Runner::vddSaveCal(float) */
    bool vddSaveCal(float value);
    /** @copydoc Runner::vddGetCal() */
    float vddGetCal();
    /** @copydoc Runner::vppCtrl(bool) */
    bool vppCtrl(bool on = true);
    /** @copydoc Runner::vppSet(float) */
    bool vppSet(float value);
    /** @copydoc Runner::vppGet() */
    float vppGet();
    /** @copydoc Runner::vppGetDuty() */
    float vppGetDuty();
    /** @copydoc Runner::vppInitCal() */
    bool vppInitCal();
    /** @copydoc Runner::vppSaveCal(float) */
    bool vppSaveCal(float value);
    /** @copydoc Runner::vppGetCal() */
    float vppGetCal();
    /** @copydoc Runner::vddOnVpp(bool) */
    bool vddOnVpp(bool on = true);
    /** @copydoc Runner::vppOnA9(bool) */
    bool vppOnA9(bool on = true);
    /** @copydoc Runner::vppOnA18(bool) */
    bool vppOnA18(bool on = true);
    /** @copydoc Runner::vppOnCE(bool) */
    bool vppOnCE(bool on = true);
    /** @copydoc Runner::vppOnOE(bool) */
    bool vppOnOE(bool on = true);
    /** @copydoc Runner::vppOnWE(bool) */
    bool vppOnWE(bool on = true);
    /** @copydoc Runner::setCE(bool) */
    bool setCE(bool on = true);
    /** @copydoc Runner::setOE(bool) */
    bool setOE(bool on = true);
    /** @copydoc Runner::setWE(bool) */
    bool setWE(bool on = true);
    /** @copydoc Runner::addrClr() */
    bool addrClr();
    /** @copydoc Runner::addrInc() */
    bool addrInc();
    /** @copydoc Runner::addrSet(uint32_t) */
    bool addrSet(uint32_t value);
    /** @copydoc Runner::addrSetB(uint8_t) */
    bool addrSetB(uint8_t value);
    /** @copydoc Runner::addrSetW(uint16_t) */
    bool addrSetW(uint16_t value);
    /** @copydoc Runner::addrGet() */
    uint32_t addrGet() const;
    /** @copydoc Runner::dataClr() */
    bool dataClr();
    /** @copydoc Runner::dataSet(uint8_t) */
    bool dataSet(uint8_t value);
    /** @copydoc Runner::dataSetW(uint16_t) */
    bool dataSetW(uint16_t value);
    /** @copydoc Runner::dataGet() */
    uint8_t dataGet();
    /** @copydoc Runner::dataGetW() */
    uint16_t dataGetW();
    /** @copydoc Runner::deviceSetTwp(uint32_t) */
    bool deviceSetTwp(uint32_t value);
    /** @copydoc Runner::deviceSetTwc(uint32_t) */
    bool deviceSetTwc(uint32_t value);
    /** @copydoc Runner::deviceSetFlags(const TDeviceSettings&) */
    bool deviceSetFlags(const TDeviceSettings& value);
    /** @copydoc Runner::deviceRead() */
    uint8_t deviceRead();
    /** @copydoc Runner::deviceReadW() */
    uint16_t deviceReadW();
    /** @copydoc Runner::deviceWrite(uint8_t) */
    bool deviceWrite(uint8_t value);
    /** @copydoc Runner::deviceWriteW(uint16_t) */
    bool deviceWriteW(uint16_t value);
    /** @copydoc Runner::deviceVerify(uint8_t) */
    bool deviceVerify(uint8_t value);
    /** @copydoc Runner::deviceVerifyW(uint16_t) */
    bool deviceVerifyW(uint16_t value);
    /** @copydoc Runner::usDelay(uint64_t) */
    static void usDelay(uint64_t value);
    /** @copydoc Runner::msDelay(uint32_t) */
    static void msDelay(uint32_t value);
    /** @copydoc Runner::processEvents() */
    static void processEvents();
    /**
     * @brief Sets the chip for emulation (global).
     * @param chip Pointer to instance of the Chip Class.
     */
    static void setChip(BaseParChip* chip);
    /**
     * @brief Fills a buffer with random data.
     * @param buffer Reference to buffer.
     * @param size New size of the buffer.
     */
    static void randomizeBuffer(QByteArray& buffer, uint32_t size);

  private:
    /* @brief Stores the serial port path. */
    QString path_;
    /* @brief Stores the VDD value, in volts. */
    float vdd_;
    /* @brief Stores the VPP value, in volts. */
    float vpp_;
    /* @brief Command timeout, in msec. */
    uint32_t timeout_;
    /* @brief Stores if is running. */
    bool running_;
    /* @brief Stores the last address. */
    uint32_t address_;
    /* @brief Indicates if an error occurred in the last operation. */
    bool error_;
    /* @brief tWP Setting (microseconds). */
    uint32_t twp_;
    /* @brief tWC Setting (microseconds). */
    uint32_t twc_;
    /* @brief Stores device settings. */
    TDeviceSettings flags_;

    uint16_t deviceRead_(bool is16bit);
    bool deviceWrite_(uint16_t value, bool is16bit);
};

#endif  // TEST_EMULATOR_EMULATOR_HPP_
