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
 * @file backend/epromfile/qepromfilebase.cpp
 * @brief Implementation of the QEpromFileBase Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "qepromfilebase.hpp"

// ---------------------------------------------------------------------------

QEpromFileBase::QEpromFileBase(QObject *parent)
    : QObject(parent), type_(EpromFileBin) {}

bool QEpromFileBase::isReadable(const QString &filename) { return false; }

QString QEpromFileBase::getFilename(void) const { return filename_; }

QEpromFileBase::QEpromFileType QEpromFileBase::getType(void) const {
    return type_;
}
