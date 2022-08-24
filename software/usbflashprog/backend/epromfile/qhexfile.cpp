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
 * @file backend/epromfile/qhexfile.cpp
 * @brief Implementation of the QHexFile Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QFile>
#include <QFileInfo>
#include <cstdio>
#include "qhexfile.hpp"

extern "C" {
# include "third/libGIS/ihex.h"
}

// ---------------------------------------------------------------------------

QHexFile::QHexFile(QObject *parent): QEpromFileBase(parent) {}

bool QHexFile::isReadable(const QString &filename) {
    FILE *fd = fopen(filename.toStdString().c_str(), "r");
    if (fd == NULL) {
        return false;
    }
    IHexRecord irec;
    int ret = Read_IHexRecord(&irec, fd);
    if (ret != IHEX_ERROR_EOF && ret != IHEX_OK) {
        fclose(fd);
        return false;
    }
    if (Checksum_IHexRecord(&irec) != irec.checksum) {
        fclose(fd);
        return false;
    }
    fclose(fd);
    return true;
}

QByteArray QHexFile::read(const QString &filename, qint64 size) {
    QByteArray result;
    FILE *fd = fopen(filename.toStdString().c_str(), "r");
    if (fd == NULL) {
        return result;
    }
    IHexRecord irec;
    int ret;
    qint64 address = -1;
    do {
        ret = Read_IHexRecord(&irec, fd);
        if (ret != IHEX_ERROR_EOF && ret != IHEX_OK) { break; }
        if (Checksum_IHexRecord(&irec) != irec.checksum) {
            ret = IHEX_ERROR_INVALID_RECORD;
            break;
        }
        if (irec.dataLen) {
            if (address == 0) { address = irec.address; }
            if (irec.address - address + irec.dataLen > result.size()) {
                result.append(QByteArray(irec.address - address
                    + irec.dataLen - result.size(), 0xFF));
            }
            for (int i = 0; i < irec.dataLen; i++) {
                result[i + irec.address -
                       static_cast<quint32>(address)] = irec.data[i];
            }
        }
    } while (ret == IHEX_OK);
    fclose(fd);
    if (ret != IHEX_OK && ret != IHEX_ERROR_EOF) {
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

bool QHexFile::write(QEpromFileType type,
                     const QString &filename, const QByteArray &data) {
    (void)type;
    IHexRecord irec;
    quint32 address = 0;
    int total = data.size();
    int len = 34;
    const uint8_t *p = reinterpret_cast<const uint8_t*>(data.data());
    int start = IHEX_TYPE_00;
    int end = IHEX_TYPE_01;
    QString suffix = ".hex";
    if (!QFileInfo(filename).suffix().isEmpty()) {
        suffix = "";
    }
    FILE *fd = fopen((filename + suffix).toStdString().c_str(), "w+");
    if (fd == NULL) {
        return false;
    }
    while (total) {
        if (len > total) { len = total; }
        if (New_IHexRecord(start, address,
            p + address, len, &irec) != IHEX_OK) { break; }
        if (Write_IHexRecord(&irec, fd) != IHEX_OK) { break; }
        total -= len;
        address += len;
    }
    if (!total) {
        if (New_IHexRecord(end, 0, NULL, 0, &irec) != IHEX_OK) { total = -1; }
        if (Write_IHexRecord(&irec, fd) != IHEX_OK) { total = -1; }
    }
    fclose(fd);
    if (total) { return false; }
    filename_ = filename + suffix;
    return true;
}
