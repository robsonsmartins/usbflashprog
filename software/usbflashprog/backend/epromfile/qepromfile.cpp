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
 * @file backend/epromfile/qepromfile.cpp
 * @brief Implementation of the QEpromFile Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "qepromfile.hpp"

#include "qbinfile.hpp"
#include "qsrecfile.hpp"
#include "qhexfile.hpp"
#include "qatmelfile.hpp"

// ---------------------------------------------------------------------------

QEpromFile::QEpromFile(QObject *parent)
    : QEpromFileBase(parent), eepromFile_(nullptr) {}

QByteArray QEpromFile::read(const QString &filename, qint32 size) {
    QByteArray result;
    if (QAtmelFile::isReadable(filename)) {
        type_ = EpromFileAtmel;
        eepromFile_.reset(new QAtmelFile());
        result = eepromFile_->read(filename, size);
    } else if (QSrecFile::isReadable(filename)) {
        type_ = EpromFileSRec;
        eepromFile_.reset(new QSrecFile());
        result = eepromFile_->read(filename, size);
    } else if (QHexFile::isReadable(filename)) {
        type_ = EpromFileHex;
        eepromFile_.reset(new QHexFile());
        result = eepromFile_->read(filename, size);
    } else {
        type_ = EpromFileBin;
        eepromFile_.reset(new QBinFile());
        result = eepromFile_->read(filename, size);
    }
    filename_ = eepromFile_->getFilename();
    return result;
}

bool QEpromFile::write(QEpromFile::QEpromFileType type, const QString &filename,
                       const QByteArray &data) {
    switch (type) {
        case EpromFileSRec:
            eepromFile_.reset(new QSrecFile());
            break;
        case EpromFileHex:
            eepromFile_.reset(new QHexFile());
            break;
        case EpromFileAtmel:
            eepromFile_.reset(new QAtmelFile());
            break;
        case EpromFileBin:
        default:
            eepromFile_.reset(new QBinFile());
            break;
    }
    bool result = eepromFile_->write(type, filename, data);
    filename_ = eepromFile_->getFilename();
    type_ = eepromFile_->getType();
    return result;
}

QEpromFile::QEpromFileType QEpromFile::typeFromStr(const QString &src) {
    if (src.toLower().contains("atmel")) {
        return EpromFileAtmel;
    } else if (src.toLower().contains(".bin") ||
               src.toLower().contains(".rom")) {
        return EpromFileBin;
    } else if (src.toLower().contains(".hex") ||
               src.toLower().contains(".eep")) {
        return EpromFileHex;
    } else if (src.toLower().contains(".s19") ||
               src.toLower().contains(".s28") ||
               src.toLower().contains(".s37") || src.toLower().contains(".s") ||
               src.toLower().contains(".s2") || src.toLower().contains(".s3")) {
        return EpromFileSRec;
    } else {
        return EpromFileBin;
    }
}
