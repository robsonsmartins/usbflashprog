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
 * @file backend/epromfile/qsrecfile.cpp
 * @brief Implementation of the QSrecFile Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QFile>
#include <QFileInfo>
#include <cstdio>
#include "qsrecfile.hpp"

extern "C" {
# include "third/libGIS/srecord.h"
}

// ---------------------------------------------------------------------------

QSrecFile::QSrecFile(QObject *parent): QEpromFileBase(parent) {}

bool QSrecFile::isReadable(const QString &filename) {
    FILE *fd = fopen(filename.toStdString().c_str(), "r");
    if (fd == NULL) {
        return false;
    }
    SRecord srec;
    int ret = Read_SRecord(&srec, fd);
    if (ret != SRECORD_ERROR_EOF && ret != SRECORD_OK) {
        fclose(fd);
        return false;
    }
    if (Checksum_SRecord(&srec) != srec.checksum) {
        fclose(fd);
        return false;
    }
    fclose(fd);
    return true;
}

QByteArray QSrecFile::read(const QString &filename, qint32 size) {
    QByteArray result;
    FILE *fd = fopen(filename.toStdString().c_str(), "r");
    if (fd == NULL) {
        return result;
    }
    SRecord srec;
    int ret;
    qint64 address = -1;
    do {
        ret = Read_SRecord(&srec, fd);
        if (ret != SRECORD_ERROR_EOF && ret != SRECORD_OK) { break; }
        if (Checksum_SRecord(&srec) != srec.checksum) {
            ret = SRECORD_ERROR_INVALID_RECORD;
            break;
        }
        if (srec.dataLen) {
            if (address == -1) { address = srec.address; }
            if (srec.address - address + srec.dataLen > result.size()) {
                result.append(QByteArray(
                    static_cast<int>(srec.address - address)
                    + srec.dataLen - result.size(), 0xFF));
            }
            for (int i = 0; i < srec.dataLen; i++) {
                result[i + srec.address -
                       static_cast<quint32>(address)] = srec.data[i];
            }
        }
    } while (ret == SRECORD_OK);
    fclose(fd);
    if (ret != SRECORD_OK && ret != SRECORD_ERROR_EOF) {
        result.clear();
        return result;
    }
    if (result.size() > size) {
        result.resize(size);
    }
    if (result.size() < size) {
        result.append(QByteArray(size - result.size(), 0xFF));
    }
    return result;
}

bool QSrecFile::write(QEpromFileType type,
                      const QString &filename, const QByteArray &data) {
    (void)type;
    SRecord srec;
    quint32 address = 0;
    int total = data.size();
    int len = 34;
    const uint8_t *p = reinterpret_cast<const uint8_t*>(data.data());
    int start = 1;
    int end = 9;
    QString suffix = ".s19";
    if (data.size() > 0xFFFFFF) {
        start = 3;
        end = 7;
        suffix = ".s37";
    } else if (data.size() > 0xFFFF) {
        start = 2;
        end = 8;
        suffix = ".s28";
    }
    if (!QFileInfo(filename).suffix().isEmpty()) {
        suffix = "";
    }
    FILE *fd = fopen((filename + suffix).toStdString().c_str(), "w+");
    if (fd == NULL) {
        return false;
    }
    while (total) {
        if (len > total) { len = total; }
        if (New_SRecord(start, address,
            p + address, len, &srec) != SRECORD_OK) { break; }
        if (Write_SRecord(&srec, fd) != SRECORD_OK) { break; }
        total -= len;
        address += len;
    }
    if (!total) {
        if (New_SRecord(end, 0, NULL, 0, &srec) != SRECORD_OK) { total = -1; }
        if (Write_SRecord(&srec, fd) != SRECORD_OK) { total = -1; }
    }
    fclose(fd);
    if (total) { return false; }
    filename_ = filename + suffix;
    return true;
}
