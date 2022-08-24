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
 * @file backend/epromfile/qatmelfile.cpp
 * @brief Implementation of the QAtmelFile Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QFile>
#include <QFileInfo>
#include <cstdio>
#include "qatmelfile.hpp"

extern "C" {
# include "third/libGIS/atmel_generic.h"
}

// ---------------------------------------------------------------------------

QAtmelFile::QAtmelFile(QObject *parent): QEpromFileBase(parent) {}

bool QAtmelFile::isReadable(const QString &filename) {
    FILE *fd = fopen(filename.toStdString().c_str(), "r");
    if (fd == NULL) {
        return false;
    }
    AtmelGenericRecord arec;
    int ret = Read_AtmelGenericRecord(&arec, fd);
    if (ret != ATMEL_GENERIC_ERROR_EOF && ret != ATMEL_GENERIC_OK) {
        fclose(fd);
        return false;
    }
    fclose(fd);
    return true;
}

QByteArray QAtmelFile::read(const QString &filename, qint64 size) {
    QByteArray result;
    FILE *fd = fopen(filename.toStdString().c_str(), "r");
    if (fd == NULL) {
        return result;
    }
    AtmelGenericRecord arec;
    int ret;
    qint64 address = -1;
    do {
        ret = Read_AtmelGenericRecord(&arec, fd);
        if (ret != ATMEL_GENERIC_ERROR_EOF &&
            ret != ATMEL_GENERIC_OK) { break; }
        if (address == -1) { address = arec.address; }
        if (arec.address - address + 2 > result.size()) {
            result.append(QByteArray(arec.address - address
                + 2 - result.size(), 0xFF));
        }
        for (int i = 0; i < 2; i++) {
            result[i + arec.address -
                    static_cast<quint32>(address)] =
                        (arec.data >> (8 * i)) & 0xFF;
        }
    } while (ret == ATMEL_GENERIC_OK);
    fclose(fd);
    if (ret != ATMEL_GENERIC_OK && ret != ATMEL_GENERIC_ERROR_EOF) {
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

bool QAtmelFile::write(QEpromFileType type,
                       const QString &filename, const QByteArray &data) {
    (void)type;
    AtmelGenericRecord arec;
    quint32 address = 0;
    int total = data.size();
    int len = 2;
    const uint8_t *p = reinterpret_cast<const uint8_t*>(data.data());
    QString suffix = ".hex";
    if (!QFileInfo(filename).suffix().isEmpty()) {
        suffix = "";
    }
    FILE *fd = fopen((filename + suffix).toStdString().c_str(), "w+");
    if (fd == NULL) {
        return false;
    }
    uint16_t word;
    while (total) {
        if (len > total) { len = total; }
        word = *(p + address + 1);
        word <<= 8;
        word |= *(p + address);
        if (New_AtmelGenericRecord(address,
            word, &arec) != ATMEL_GENERIC_OK) { break; }
        if (Write_AtmelGenericRecord(&arec, fd) != ATMEL_GENERIC_OK) { break; }
        total -= len;
        address += len;
    }
    fclose(fd);
    if (total) { return false; }
    filename_ = filename + suffix;
    return true;
}
