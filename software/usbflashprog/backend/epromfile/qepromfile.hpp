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
 * @file backend/epromfile/qepromfile.hpp
 * @brief Header of the QEpromFile Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_EPROMFILE_QEPROMFILE_HPP_
#define BACKEND_EPROMFILE_QEPROMFILE_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QScopedPointer>

#include "qepromfilebase.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief QEpromFile Class
 * @details The purpose of this class is to manage all EPROM File formats.
 * @nosubgrouping
 */
class QEpromFile: public QEpromFileBase {
     Q_OBJECT

 public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object (default = nullptr).
     */
    explicit QEpromFile(QObject *parent = nullptr);
    /**
     * @brief Returns the type of the file, by string (file filter).
     * @param src The source string.
     * @return Type of the file.
     */
    static QEpromFileType typeFromStr(const QString& src);
    /**
     * @brief Reads data from a file.
     * @param filename Filename (with path).
     * @param size Size of data to be read (in bytes). If file is smaller
     *   than this parameter, data is filled with 0xFF.
     * @return Byte array with data, or empty if an error occurs.
     */
    QByteArray read(const QString &filename, qint32 size);
    /**
     * @brief Writes data to a file.
     * @param type Type of the file to write.
     * @param filename Filename (with path).
     * @param data Data to write.
     * @return True if success, false otherwise.
     */
    bool write(QEpromFileType type,
               const QString &filename, const QByteArray &data);

 private:
    /* @brief Pointer to Eprom File manager. */
    QScopedPointer<QEpromFileBase> eepromFile_;
};

#endif  // BACKEND_EPROMFILE_QEPROMFILE_HPP_
