// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
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
#include <QQueue>
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
     * @brief Sends a command via serial port.
     * @param code OpCode of the command.
     */
    void send(kCmdOpCodeEnum code);
    /**
     * @brief Sends a command via serial port.
     * @param code OpCode of the command.
     * @param param Value of the parameter.
     */
    void sendByte(kCmdOpCodeEnum code, uint8_t param);
    /**
     * @brief Sends a command via serial port.
     * @param code OpCode of the command.
     * @param param Value of the parameter.
     */
    void sendWord(kCmdOpCodeEnum code, uint16_t param);
    /**
     * @brief Sends a command via serial port.
     * @param code OpCode of the command.
     * @param param Value of the parameter.
     */
    void sendDWord(kCmdOpCodeEnum code, uint32_t param);
    /**
     * @brief Sends a command via serial port.
     * @param code OpCode of the command.
     * @param param Value of the parameter.
     */
    void sendBool(kCmdOpCodeEnum code, bool param);
    /**
     * @brief Sends a command via serial port.
     * @param code OpCode of the command.
     * @param param Value of the parameter.
     */
    void sendFloat(kCmdOpCodeEnum code, float param);

 signals:
    /**
     * @brief Signal emmited when response of the sent command is ready.
     * @param command Sent command and its response.
     */
    void resultReady(const TRunnerCommand& command);

 private slots:
    /* @brief Receives data via serial port. */
    void onSerial_readyRead();

 private:
    /* @brief Serial port object. */
    QSerialPort serial_;
    /* @brief FIFO of the commands to send. */
    QQueue<TRunnerCommand> wrFifo_;
    /* @brief Buffer to read data from serial port. */
    QByteArray rdBuffer_;
    /* @brief Tickcounter used by alive timer. */
    qint64 aliveTick_;
    /* @brief Stores if is running. */
    bool running_;
    /* @brief Process received data (via serial port). */
    void processData_();
    /* @brief Sends a command via serial port.
     * @param cmd Command to send. */
    void write_(const TRunnerCommand &cmd);
    /* @brief Checks if is alive (or timeout). */
    void checkAlive_();
};

#endif  // BACKEND_RUNNER_HPP_
