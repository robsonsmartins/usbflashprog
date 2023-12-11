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
 * @file backend/epromfile/qepromfilebase.hpp
 * @brief Header of the QEpromFileBase Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef BACKEND_EPROMFILE_QEPROMFILEBASE_HPP_
#define BACKEND_EPROMFILE_QEPROMFILEBASE_HPP_

// ---------------------------------------------------------------------------

#include <QObject>
#include <QString>
#include <QByteArray>

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief QEpromFileBase Class
 * @details The purpose of this class is to be an interface to all the
 *   EPROM File Manipulator classes.
 * @nosubgrouping
 */
class QEpromFileBase : public QObject {
    Q_OBJECT

  public:
    /**
     * @brief Eprom File Types.
     */
    enum QEpromFileType {
        /** @brief Binary file (raw). */
        EpromFileBin,
        /** @brief Motorola S-REC file. */
        EpromFileSRec,
        /** @brief Intel Hex file. */
        EpromFileHex,
        /** @brief Atmel Generic file. */
        EpromFileAtmel
    };

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object (default = nullptr).
     */
    explicit QEpromFileBase(QObject *parent = nullptr);
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
    virtual QByteArray read(const QString &filename, qint32 size) = 0;
    /**
     * @brief Writes data to a file.
     * @param type Type of the file to write.
     * @param filename Filename (with path).
     * @param data Data to write.
     * @return True if success, false otherwise.
     */
    virtual bool write(QEpromFileType type, const QString &filename,
                       const QByteArray &data) = 0;
    /**
     * @brief Gets the filename used to read or write the file.
     * @return The filename, or empty if file wasn't saved/opened yet.
     */
    virtual QString getFilename(void) const;
    /**
     * @brief Gets the type of the file opened/saved.
     * @return Type of the file.
     */
    virtual QEpromFileType getType(void) const;

  protected:
    /* @brief File type. */
    QEpromFileType type_;
    /* @brief Filename. */
    QString filename_;
};

#endif  // BACKEND_EPROMFILE_QEPROMFILEBASE_HPP_
