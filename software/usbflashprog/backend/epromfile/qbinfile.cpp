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
 * @file backend/epromfile/qbinfile.cpp
 * @brief Implementation of the QBinFile Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QFile>
#include "qbinfile.hpp"

// ---------------------------------------------------------------------------

QBinFile::QBinFile(QObject *parent): QEpromFileBase(parent) {}

bool QBinFile::isReadable(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    if (file.read(1).isEmpty()) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

QByteArray QBinFile::read(const QString &filename, qint64 size) {
    QByteArray result;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return result;
    }
    result = file.read(size);
    if (result.isEmpty()) {
        file.close();
        return result;
    }
    file.close();
    if (result.size() < size) {
        result.append(QByteArray(size - result.size(), 0xFF));
    }
    return result;
}

bool QBinFile::write(QEpromFileType type,
                     const QString &filename, const QByteArray &data) {
    (void)type;
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)) {
        return false;
    }
    qint64 ret = file.write(data);
    file.close();
    filename_ = filename;
    return (ret == data.size());
}
