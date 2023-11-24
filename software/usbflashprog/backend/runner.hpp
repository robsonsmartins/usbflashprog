// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2023) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup Software
 * @file backend/runner.hpp
 * @brief Header of the Runner Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_RUNNER_HPP_
#define BACKEND_RUNNER_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>

#ifndef TEST_BUILD
#  include <QSerialPort>
#  include <QSerialPortInfo>
#else
#  include "test/mock/qserialport.hpp"
#endif

#include "backend/opcodes.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief List of serial ports info. 
 */
typedef QList<QSerialPortInfo> TSerialPortList;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Defines a command to be run and its response. 
 */
typedef struct TRunnerCommand {
    /** @brief Opcode of the command. */
    TCmdOpCode opcode;
    /** @brief Parameters of the command (raw bytes). */
    QByteArray params;
    /** @brief Response of the command (raw bytes). */
    QByteArray response;
    /**
     * @brief Returns if response is OK.
     * @return True if response is OK, else if response is NOK.
     */
    bool responseIsOk() const;
    /**
     * @brief Returns response value as a float point.
     * @return Response value.
     */
    float responseAsFloat() const;
    /**
     * @brief Returns response value as a byte.
     * @return Response value.
     */
    int responseAsByte() const;
    /**
     * @brief Returns response value as a word.
     * @return Response value.
     */
    int responseAsWord() const;
    /**
     * @brief Returns response value as a double word.
     * @return Response value.
     */
    int responseAsDWord() const;
    /**
     * @brief Returns response value as a boolean.
     * @return Response value.
     */
    bool responseAsBool() const;
    /**
     * @brief Sets the command.
     * @param code OpCode of the command.
     */
    void set(kCmdOpCodeEnum code);
    /**
     * @brief Sets the command and the parameters.
     * @param code OpCode of the command.
     * @param param Value of the param.
     */
    void setFloat(kCmdOpCodeEnum code, float param);
    /**
     * @brief Sets the command and the parameters.
     * @param code OpCode of the command.
     * @param param Value of the param.
     */
    void setByte(kCmdOpCodeEnum code, uint8_t param);
    /**
     * @brief Sets the command and the parameters.
     * @param code OpCode of the command.
     * @param param Value of the param.
     */
    void setWord(kCmdOpCodeEnum code, uint16_t param);
    /**
     * @brief Sets the command and the parameters.
     * @param code OpCode of the command.
     * @param param Value of the param.
     */
    void setDWord(kCmdOpCodeEnum code, uint32_t param);
    /**
     * @brief Sets the command and the parameters.
     * @param code OpCode of the command.
     * @param param Value of the param.
     */
    void setBool(kCmdOpCodeEnum code, bool param);
    /**
     * @brief Assign Operator.
     * @param src TRunnerCommand source object.
     * @return Reference for this.
     */
    TRunnerCommand& operator=(const TRunnerCommand& src);
    /**
     * @brief Equality Operator.
     * @param a One object.
     * @param b Another object.
     * @return True if the objects contain equal values, false otherwise.
     */
    friend bool operator==(const TRunnerCommand& a, const TRunnerCommand& b);
} TRunnerCommand;

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Runner Class
 * @details The purpose of this class is to run commands and get response
 *   via USB-CDC.
 * @nosubgrouping
 */
class Runner: public QObject {
    Q_OBJECT

 public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit Runner(QObject *parent = nullptr);
    /** @brief Destructor. */
    ~Runner();
    /**
     * @brief Lists all serial ports that correspond to the device boards.
     * @return List of serial port info.
     */
    TSerialPortList list() const;
    /**
     * @brief Opens a serial port.
     * @param path Path of the serial port (system dependent).
     * @return True if success, false otherwise.
     */
    bool open(const QString &path);
    /** @brief Closes an opened serial port. */
    void close();
    /**
     * @brief Returns if a serial is opened.
     * @return True if a serial port is opened, false otherwise.
     */
    bool isOpen() const;
    /**
     * @brief Returns the current opened serial port path (if any).
     * @return Path of opened serial port, or empty if none.
     */
    QString getPath() const;
    /**
     * @brief Returns the current timeout value of the serial port
     *   communication.
     * @return Timeout, in msec.
     */
    uint32_t getTimeOut() const;
    /**
     * @brief Sets the timeout value of the serial port
     *   communication.
     * @param value Timeout, in msec.
     */
    void setTimeOut(uint32_t value);
    /**
     * @brief Runs the NOP opcode.
     * @return True if success, false otherwise.
     */
    bool nop();
    /**
     * @brief Runs the VDD Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vddCtrl(bool on = true);
    /**
     * @brief Runs the VDD Set Voltage opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool vddSet(float value);
    /**
     * @brief Runs the VDD Get Voltage opcode.
     * @return VDD Voltage if success, -1.0f otherwise.
     */
    float vddGet();
    /**
     * @brief Runs the VDD Get Duty Cycle opcode.
     * @return VDD Duty Cycle if success, -1.0f otherwise.
     */
    float vddGetDuty();
    /**
     * @brief Runs the VDD Init Calibration opcode.
     * @return True if success, false otherwise.
     */
    bool vddInitCal();
    /**
     * @brief Runs the VDD Save Calibration opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool vddSaveCal(float value);
    /**
     * @brief Runs the VDD Get Calibration opcode.
     * @return VDD Calibration if success, -1.0f otherwise.
     */
    float vddGetCal();
    /**
     * @brief Runs the VPP Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vppCtrl(bool on = true);
    /**
     * @brief Runs the VPP Set Voltage opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool vppSet(float value);
    /**
     * @brief Runs the VPP Get Voltage opcode.
     * @return VPP Voltage if success, -1.0f otherwise.
     */
    float vppGet();
    /**
     * @brief Runs the VPP Get Duty Cycle opcode.
     * @return VPP Duty Cycle if success, -1.0f otherwise.
     */
    float vppGetDuty();
    /**
     * @brief Runs the VPP Init Calibration opcode.
     * @return True if success, false otherwise.
     */
    bool vppInitCal();
    /**
     * @brief Runs the VPP Save Calibration opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool vppSaveCal(float value);
    /**
     * @brief Runs the VPP Get Calibration opcode.
     * @return VPP Calibration if success, -1.0f otherwise.
     */
    float vppGetCal();
    /**
     * @brief Runs the VDD on VPP Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vddOnVpp(bool on = true);
    /**
     * @brief Runs the VPP on A9 Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vppOnA9(bool on = true);
    /**
     * @brief Runs the VPP on A18 Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vppOnA18(bool on = true);
    /**
     * @brief Runs the VPP on CE Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vppOnCE(bool on = true);
    /**
     * @brief Runs the VPP on OE Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vppOnOE(bool on = true);
    /**
     * @brief Runs the VPP on WE Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool vppOnWE(bool on = true);
    /**
     * @brief Runs the CE Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool setCE(bool on = true);
    /**
     * @brief Runs the OE Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool setOE(bool on = true);
    /**
     * @brief Runs the WE Control opcode.
     * @param on If true (default), sets the pin. Resets otherwise.
     * @return True if success, false otherwise.
     */
    bool setWE(bool on = true);
    /**
     * @brief Runs the Address Clear opcode.
     * @return True if success, false otherwise.
     */
    bool addrClr();
    /**
     * @brief Runs the Address Increment opcode.
     * @return True if success, false otherwise.
     */
    bool addrInc();
    /**
     * @brief Runs the Address Set (DWord) opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool addrSet(uint32_t value);
    /**
     * @brief Runs the Address Set Byte opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool addrSetB(uint8_t value);
    /**
     * @brief Runs the Address Set Word opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool addrSetW(uint16_t value);
    /**
     * @brief Runs the Data Clear opcode.
     * @return True if success, false otherwise.
     */
    bool dataClr();
    /**
     * @brief Runs the Data Set Byte opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool dataSet(uint8_t value);
    /**
     * @brief Runs the Data Set (Word) opcode.
     * @param value Value to set.
     * @return True if success, false otherwise.
     */
    bool dataSetW(uint16_t value);
    /**
     * @brief Runs the Data Get Byte opcode.
     * @return Data value if success, 0xFF otherwise.
     */
    uint8_t dataGet();
    /**
     * @brief Runs the Data Get opcode.
     * @return Data value if success, 0xFFFF otherwise.
     */
    uint16_t dataGetW();
    /**
     * @brief Pauses the program execution for a specified time
     *   (microsecond precision).
     * @param value Sleep time, in usec.
     */
    static void usDelay(uint64_t value);
    /**
     * @brief Pauses the program execution for a specified time
     *   (millisecond precision).
     * @param value Sleep time, in msec.
     */
    static void msDelay(uint32_t value);

 private:
    /* @brief Command timeout, in msec. */
    uint32_t timeout_;
    /* @brief Stores if is running. */
    bool running_;
    /* @brief Serial port object. */
    QSerialPort serial_;
    /* @brief Tickcounter used by alive timer. */
    qint64 aliveTick_;
    /* @brief Sends data via serial port.
     * @param data Data to send. 
     * @return True if success, false otherwise. */
    bool write_(const QByteArray &data);
    /* @brief Receives data via serial port.
     * @param data Pointer to QByteArray to receive data.
     * @param size Size of data to receive.
     * @return True if success, false otherwise. */
    bool read_(QByteArray *data, uint32_t size);
    /* @brief Checks if is alive (or timeout). */
    void checkAlive_();
};

#endif  // BACKEND_RUNNER_HPP_
