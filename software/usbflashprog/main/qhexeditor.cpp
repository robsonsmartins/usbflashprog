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
 * @file main/qhexeditor.cpp
 * @brief Implementation of the QHexEditor Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QFile>
#include <QRandomGenerator>
#include <model/buffer/qmemorybuffer.h>
#include <dialogs/hexfinddialog.h>
#include "qhexeditor.hpp"

// ---------------------------------------------------------------------------

QHexEditor::QHexEditor(QWidget *parent)
        : QHexView(parent), changed_(false), filename_(""), size_(0) {
    QFont font = this->font();
    font.setPointSize(10);
    this->setFont(font);
    this->setSizePolicy(QSizePolicy::Policy::Expanding,
        QSizePolicy::Policy::Expanding);
    QHexOptions options;
    options.addresswidth = 5;
    options.asciilabel = "ASCII";
    options.headercolor = Qt::black;
    options.flags = QHexFlags::Styled | QHexFlags::Separators
        | QHexFlags::HighlightAddress | QHexFlags::HighlightColumn;
    this->setOptions(options);
    setSize(1024);
    connect(this, &QHexView::dataChanged, this, &QHexEditor::onDataChanged);
}

bool QHexEditor::open(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray data = file.read(size_);
    if (data.isEmpty()) {
        file.close();
        return false;
    }
    file.close();
    if (data.size() < size_) {
        data.append(QByteArray(size_ - data.size(), 0xFF));
    }
    QHexDocument *document =
        QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    filename_ = filename;
    changed_ = false;
    emit changed(false);
    return true;
}

bool QHexEditor::save(void) {
    if (filename_.isEmpty()) { return false; }
    return saveAs(filename_);
}

bool QHexEditor::saveAs(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)) {
        return false;
    }
    QHexDocument *document = this->hexDocument();
    if (!document->saveTo(&file)) {
        return false;
    }
    file.close();
    filename_ = filename;
    changed_ = false;
    emit changed(false);
    return true;
}

void QHexEditor::fill(quint8 value) {
    QByteArray data = QByteArray(size_, value);
    QHexDocument *document =
        QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    changed_ = true;
    emit changed(true);
}

void QHexEditor::random(void) {
    QByteArray data;
    for (qint64 i = 0; i < size_; i++) {
        data += (QRandomGenerator::global()->bounded(256) & 0xFF);
    }
    QHexDocument *document =
        QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    changed_ = true;
    emit changed(true);
}

void QHexEditor::setSize(qint64 value) {
    if (value == size_) { return; }
    QHexDocument *document = this->hexDocument();
    QByteArray data;
    if (document != nullptr) {
        int len = document->length() < value ? document->length() : value;
        data = document->read(0, len);
    }
    if (data.size() < value) {
        data.append(QByteArray(value - data.size(), 0xFF));
    }
    document = QHexDocument::fromMemory<QMemoryBuffer>(data);
    this->setDocument(document);
    size_ = value;
}

qint64 QHexEditor::size(void) const {
    return size_;
}

const QString& QHexEditor::filename(void) const {
    return filename_;
}

bool QHexEditor::isChanged(void) const {
    return changed_;
}

void QHexEditor::showFindDialog(void) {
    HexFindDialog dialog(HexFindDialog::Type::Find, this);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dialog.exec();
}

void QHexEditor::showReplaceDialog(void) {
    HexFindDialog dialog(HexFindDialog::Type::Replace, this);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dialog.exec();
}

void QHexEditor::onDataChanged(const QByteArray& data, quint64 offset,
                               QHexDocument::ChangeReason reason) {
    changed_ = true;
    emit changed();
}
