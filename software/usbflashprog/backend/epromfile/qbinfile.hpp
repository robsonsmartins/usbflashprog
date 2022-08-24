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
 * @file backend/epromfile/qbinfile.hpp
 * @brief Header of the QBinFile Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_EPROMFILE_QBINFILE_HPP_
#define BACKEND_EPROMFILE_QBINFILE_HPP_

// ---------------------------------------------------------------------------

#include "qepromfilebase.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Raw Binary Files Stream Class
 * @details The purpose of this class is to read and write data from/to a
 *   file, in raw binary format.
 * @nosubgrouping
 */
class QBinFile: public QEpromFileBase {
    Q_OBJECT

 public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object (default = nullptr).
     */
    explicit QBinFile(QObject *parent = nullptr);
    /**
     * @brief Returns if file is readable in this format.
     * @param filename Filename (with path).
     * @return True if success, false otherwise.
     */
    static bool isReadable(const QString &filename);
    /**
     * @brief Reads data from a file.
     * @param filename Filename (with path).
     * @param size Size of data to be read (in bytes). If file is smaller
     *   than this parameter, data is filled with 0xFF.
     * @return Byte array with data, or empty if an error occurs.
     */
    QByteArray read(const QString &filename, qint64 size);
    /**
     * @brief Writes data to a file.
     * @param type Type of the file to write.
     * @param filename Filename (with path).
     * @param data Data to write.
     * @return True if success, false otherwise.
     */
    bool write(QEpromFileType type,
               const QString &filename, const QByteArray &data);
};

#endif  // BACKEND_EPROMFILE_QBINFILE_HPP_
